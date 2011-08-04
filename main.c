#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>

#include "gprs.h"
#include "daemon.h"
#include "beep.h"
#include "led.h"
#include "adc.h"
#include "cgi.h"
#include "main.h"

#define COMMAND_NUM 4
#define ALARM_NUM 3

#define NORMAL 0
#define FIRE 1
#define THIEF 2

#define MAXBUFSIZE 256
#define MAXSHMSIZE 8192

const char *gprs_init_commands[COMMAND_NUM] = {
	"ate0\r",
	"at\r",
	"at+cmgf=0\r",
	"at+csca=8613800100500,145\r",
};

const char *gprs_alarm[ALARM_NUM] = {
	"家里一切正常！",
	"家里着火了！",
	"家里来小偷了！",
};

char gprs_src[MAXBUFSIZE];
char gprs_dst[MAXBUFSIZE];
char gprs_msg[MAXBUFSIZE];
char gprs_cmd[MAXBUFSIZE];
char prefix[] = "0011000D9168";
char phone[] = "8157910894F9";
char postfix[] = "000801";

int main(int argc, char *argv[])
{
	int shmid;
	pid_t pid;
	int fd_serial, fd_phone, fd_ada;
	int fd_keyboard, fd_led, fd_beep, fd_adc;
	struct termios uart;

	pthread_mutexattr_t gprs_mutex_start_attr;
	pthread_condattr_t gprs_cond_start_attr;

	pthread_mutexattr_t gprs_mutex_end_attr;
	pthread_condattr_t gprs_cond_end_attr;

	pthread_mutexattr_t beep_mutex_start_attr;
	pthread_condattr_t beep_cond_start_attr;

	pthread_mutexattr_t beep_mutex_status_attr;

	pthread_mutexattr_t beep_mutex_end_attr;
	pthread_condattr_t beep_cond_end_attr;

	pthread_mutexattr_t led_mutex_start_attr;
	pthread_condattr_t led_cond_start_attr;

	pthread_mutexattr_t led_mutex_end_attr;
	pthread_condattr_t led_cond_end_attr;

	init_daemon();

	key_t key = 0x34567890;

	if (-1 == (shmid = shmget(key, MAXSHMSIZE, IPC_CREAT | IPC_EXCL | 0666)))
	{
		if (EEXIST == errno)
		{
			shmid = shmget(key, MAXSHMSIZE, 0666); 
			SHM = shmat(shmid, NULL, 0);
		}
		else
		{
			perror("fail to shmget");
			exit(-1);
		}
	}
	else
	{
		SHM = shmat(shmid, NULL, 0);
	}

	SHM->gprs_emit_start = 0;
	SHM->gprs_emit_end = 0;

	SHM->led_emit_start = 0;
	SHM->led_emit_end = 0;
	SHM->led_emit_status = 0;

	SHM->beep_emit_start = 0;
	SHM->beep_emit_end = 0;
	SHM->beep_emit_status = 0;

	SHM->cgi_emit_start = 0;

	pthread_mutexattr_init(&gprs_mutex_start_attr);
	pthread_mutexattr_setpshared(&gprs_mutex_start_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&SHM->gprs_mutex_start, &gprs_mutex_start_attr);
	pthread_mutexattr_destroy(&gprs_mutex_start_attr);

	pthread_condattr_init(&gprs_cond_start_attr);
	pthread_condattr_setpshared(&gprs_cond_start_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&SHM->gprs_cond_start, &gprs_cond_start_attr);
	pthread_condattr_destroy(&gprs_cond_start_attr);

	pthread_mutexattr_init(&gprs_mutex_end_attr);
	pthread_mutexattr_setpshared(&gprs_mutex_end_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&SHM->gprs_mutex_end, &gprs_mutex_end_attr);
	pthread_mutexattr_destroy(&gprs_mutex_end_attr);

	pthread_condattr_init(&gprs_cond_end_attr);
	pthread_condattr_setpshared(&gprs_cond_end_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&SHM->gprs_cond_end, &gprs_cond_end_attr);
	pthread_condattr_destroy(&gprs_cond_end_attr);

	pthread_mutexattr_init(&beep_mutex_start_attr);
	pthread_mutexattr_setpshared(&beep_mutex_start_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&SHM->beep_mutex_start, &beep_mutex_start_attr);
	pthread_mutexattr_destroy(&beep_mutex_start_attr);

	pthread_condattr_init(&beep_cond_start_attr);
	pthread_condattr_setpshared(&beep_cond_start_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&SHM->beep_cond_start, &beep_cond_start_attr);
	pthread_condattr_destroy(&beep_cond_start_attr);

	pthread_mutexattr_init(&beep_mutex_end_attr);
	pthread_mutexattr_setpshared(&beep_mutex_end_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&SHM->beep_mutex_end, &beep_mutex_end_attr);
	pthread_mutexattr_destroy(&beep_mutex_end_attr);

	pthread_condattr_init(&beep_cond_end_attr);
	pthread_condattr_setpshared(&beep_cond_end_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&SHM->beep_cond_end, &beep_cond_end_attr);
	pthread_condattr_destroy(&beep_cond_end_attr);

	pthread_mutexattr_init(&beep_mutex_status_attr);
	pthread_mutexattr_setpshared(&beep_mutex_status_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&SHM->beep_mutex_status, &beep_mutex_status_attr);
	pthread_mutexattr_destroy(&beep_mutex_status_attr);

	pthread_mutexattr_init(&led_mutex_start_attr);
	pthread_mutexattr_setpshared(&led_mutex_start_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&SHM->led_mutex_start, &led_mutex_start_attr);
	pthread_mutexattr_destroy(&led_mutex_start_attr);

	pthread_condattr_init(&led_cond_start_attr);
	pthread_condattr_setpshared(&led_cond_start_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&SHM->led_cond_start, &led_cond_start_attr);
	pthread_condattr_destroy(&led_cond_start_attr);

	pthread_mutexattr_init(&led_mutex_end_attr);
	pthread_mutexattr_setpshared(&led_mutex_end_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&SHM->led_mutex_end, &led_mutex_end_attr);
	pthread_mutexattr_destroy(&led_mutex_end_attr);

	pthread_condattr_init(&led_cond_end_attr);
	pthread_condattr_setpshared(&led_cond_end_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&SHM->led_cond_end, &led_cond_end_attr);
	pthread_condattr_destroy(&led_cond_end_attr);

	if (0 > (fd_adc = open("/dev/ADC", O_RDWR)))
	{
		perror("Failed to open ADC");
		exit(-1);
	}

	if (0 > (fd_beep = open("/dev/BEEP", O_RDWR)))
	{
		perror("Failed to open BEEP");
		exit(-1);
	}

	if (0 > (fd_led = open("/dev/LED", O_RDWR)))
	{
		perror("Failed to open LED");
		exit(-1);
	}

	if (-1 == (fd_serial = open("/dev/s3c2410_serial1", O_RDWR | O_NOCTTY | O_NONBLOCK)))
	{
		perror("Failed to open serial port");
		exit(-1);
	}

	tcflush(fd_serial, TCIFLUSH);

	if (!termios_init(fd_serial, &uart, B115200))
	{
		printf("Failed to init serial port\n");
		exit(-1);
	}

	if (-1 == (fd_phone = open("/gprs.phone", O_RDONLY)))
	{
		perror("Failed to open gprs.phone");
		exit(-1);
	}

	if (-1 == (fd_ada = open("/gprs.ada", O_RDONLY)))
	{
		perror("Failed to open gprs.ada");
		exit(-1);
	}

	if (-1 == (fd_keyboard = open("/dev/KEYSCAN", O_RDWR)))
	{
		perror("Failed to open KEYSCAN");
		exit(-1);
	}

	if (-1 == (pid = fork()))
	{
		perror("Failed to fork alarm process");
		exit(-1);
	}
	else if (0 == pid) //alarm process
	{
		pthread_t beep_thread;
		pthread_t led_thread;

		if (0 != pthread_create(&beep_thread, NULL, beep_thread_handler, (void *)&fd_beep))
		{
			perror("Failed to create beep thread");
			exit(-1);
		}

		if (0 != pthread_create(&led_thread, NULL, led_thread_handler, (void *)&fd_led))
		{
			perror("Failed to create led thread");
			exit(-1);
		}

		while (1)
		{
			char *buff = NULL;
			int msglen = 0;
			int gprs_flag_end = 0;

			pthread_mutex_lock(&SHM->gprs_mutex_start);

			while (0 == SHM->gprs_emit_start)
			{
				pthread_cond_wait(&SHM->gprs_cond_start, &SHM->gprs_mutex_start);
			}

			SHM->gprs_emit_start--;
			pthread_mutex_unlock(&SHM->gprs_mutex_start);

			if (gprs_init(fd_serial, gprs_init_commands, COMMAND_NUM))
			{
				//printf("Init succeed\n");
			}
			else
			{
				//printf("Init failed\n");
				//exit(-1);
			}

			memset(phone, 0, sizeof(phone));
			read(fd_phone, phone, MAXBUFSIZE);
			phone[11] = 'F';
			order_change(phone, strlen(phone));

			memset(gprs_src, 0, sizeof(gprs_src));
			memset(gprs_dst, 0, sizeof(gprs_dst));
			read(fd_ada, gprs_src, MAXBUFSIZE);
			buff = (char *)gprs_dst;
			utf8_to_unicode(gprs_src, strlen(gprs_src), &buff, &msglen);
			order_change(buff, msglen-2);
			hex_to_asc(gprs_msg, buff, msglen-2);

			sprintf(gprs_cmd, "at+cmgs=%d\r", strlen(gprs_msg)/2+15);
			sprintf(buff, "%s%s%s%02X%s\x1a", prefix, phone, postfix,
					strlen(gprs_msg)/2, gprs_msg);

			if (message_send(fd_serial, gprs_cmd, strlen(gprs_cmd), 
						buff, strlen(buff)))
			{
				//printf("Send succeed\n");
			}
			else
			{
				//printf("Send failed\n");
				//exit(-1);
			}

			pthread_mutex_lock(&SHM->gprs_mutex_end);
			gprs_flag_end = (0 == SHM->gprs_emit_end);
			SHM->gprs_emit_end++;
			pthread_mutex_unlock(&SHM->gprs_mutex_end);

			if (1 == gprs_flag_end)
			{
				pthread_cond_broadcast(&SHM->gprs_cond_end);
			}

			usleep(100000);
		}
	}
	else if (-1 == (pid = fork()))
	{
		perror("Failed to fork script process");
		exit(-1);
	}
	else if (0 == pid) //autoload modules
	{
		if (-1 == execl("./install.sh", "./install.sh", NULL))
		{
			perror("Failed to load modules");
			exit(-1);
		}
		else
		{
			exit(0);
		}
	}
	else //listen process
	{
		int keyno = 0;
		int gprs_flag_start = 0;
		int led_flag_start = 0;
		int beep_flag_start = 0;

		signal(SIGCHLD, SIG_IGN);

		pthread_t adc_thread;
		pthread_t cgi_thread;

		if (0 != pthread_create(&adc_thread, NULL, adc_thread_handler, (void *)&fd_adc))
		{
			perror("Failed to create adc thread");
			exit(-1);
		}

		if (0 != pthread_create(&cgi_thread, NULL, cgi_thread_handler, NULL))
		{
			perror("Failed to create cgi thread");
			exit(-1);
		}

		while (1)
		{
			read(fd_keyboard, &keyno, sizeof(keyno));
			//printf("keyno=%d\n", keyno);

			if (0 != keyno)
			{
				pthread_mutex_lock(&SHM->gprs_mutex_start);
				gprs_flag_start = (0 == SHM->gprs_emit_start);
				SHM->gprs_emit_start++;
				pthread_mutex_unlock(&SHM->gprs_mutex_start);

				if (1 == gprs_flag_start)
				{
					pthread_cond_signal(&SHM->gprs_cond_start);
				}

				pthread_mutex_lock(&SHM->gprs_mutex_end);

				while (0 == SHM->gprs_emit_end)
				{
					pthread_cond_wait(&SHM->gprs_cond_end, &SHM->gprs_mutex_end);
				}

				if (0 < SHM->gprs_emit_end)
				{
					SHM->gprs_emit_end--;
				}

				pthread_mutex_unlock(&SHM->gprs_mutex_end);

				pthread_mutex_lock(&SHM->led_mutex_start);
				led_flag_start = (0 == SHM->led_emit_start);
				
				if (0 == SHM->led_emit_status)
				{
					SHM->led_emit_status++;
				}

				if (0 == SHM->led_emit_start)
				{
					SHM->led_emit_start++;
				}

				pthread_mutex_unlock(&SHM->led_mutex_start);

				if (1 == led_flag_start)
				{
					pthread_cond_signal(&SHM->led_cond_start);
				}

				pthread_mutex_lock(&SHM->led_mutex_end);

				while (0 == SHM->led_emit_end)
				{
					pthread_cond_wait(&SHM->led_cond_end, &SHM->led_mutex_end);
				}

				if (0 < SHM->led_emit_end)
				{
					SHM->led_emit_end--;
				}

				pthread_mutex_unlock(&SHM->led_mutex_end);

				pthread_mutex_lock(&SHM->beep_mutex_status);

				if (0 == SHM->beep_emit_status)
				{
					SHM->beep_emit_status++;
				}

				pthread_mutex_unlock(&SHM->beep_mutex_status);

				pthread_mutex_lock(&SHM->beep_mutex_start);
				beep_flag_start = (0 == SHM->beep_emit_start);

				if (0 == SHM->beep_emit_start)
				{
					SHM->beep_emit_start++;
				}

				pthread_mutex_unlock(&SHM->beep_mutex_start);

				if (1 == beep_flag_start)
				{
					pthread_cond_signal(&SHM->beep_cond_start);
				}

				pthread_mutex_lock(&SHM->beep_mutex_end);

				while (0 == SHM->beep_emit_end)
				{
					pthread_cond_wait(&SHM->beep_cond_end, &SHM->beep_mutex_end);
				}

				if (0 < SHM->beep_emit_end)
				{
					SHM->beep_emit_end--;
				}

				pthread_mutex_unlock(&SHM->beep_mutex_end);

				keyno = 0;

				if (-1 == write(fd_keyboard, &keyno, sizeof(keyno)))
				{
					perror("Failed to write");
					exit(-1);
				}
			}

			usleep(100000);
		}
	}

	return 0;
}
