#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "beep.h"
#include "main.h"

Note MumIsTheBestInTheWorld[]={
	//6.	  		  //_5		 //3		 //5	
	{LA,TIME+TIME/2}, {SOL,TIME/2},{MI,TIME},{SOL,TIME},
	//1^		   //6_		  //_5		//6-
	{DO*2,TIME},{LA,TIME/2},{SOL,TIME/2} ,{LA,2*TIME},
	// 3      //5_		//_6		   //5
	{MI,TIME},{SOL,TIME/2},{LA,TIME/2},{SOL,TIME},
	// 3	 	//1_		//_6,
	{MI,TIME},{DO,TIME/2},{LA/2,TIME/2},
	//5_		//_3		//2-		   //2.
	{SOL,TIME/2},{MI,TIME/2},{RE,TIME*2},{RE,TIME+TIME/2},
	//_3 	//5			//5_			//_6
	{MI,TIME/2},{SOL,TIME},{SOL,TIME/2},{LA,TIME/2},
	// 3		//2			//1-			//5.
	{MI,TIME},{RE,TIME},{DO,TIME*2},{SOL,TIME+TIME/2},
	//_3		//2_		//_1 		//6,_
	{MI,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME/2},
	//_1		//5,--
	{DO,TIME/2},{SOL/2,TIME*3}
};

void *beep_thread_handler(void *arg)
{
	int i, div;

	while (1)
	{
		pthread_mutex_lock(&SHM->beep_mutex_start);

		while (0 == SHM->beep_emit_start)
		{
			pthread_cond_wait(&SHM->beep_cond_start, &SHM->beep_mutex_start);
		}

		SHM->beep_emit_start--;
		pthread_mutex_unlock(&SHM->beep_mutex_start);

		for(i = 0; i < sizeof(MumIsTheBestInTheWorld)/sizeof(Note); i++)
		{
			pthread_mutex_lock(&SHM->beep_mutex_status);

			if (0 == SHM->beep_emit_status)
			{
				ioctl(*((int *)arg), BEEP_OFF);
				break;
			}

			pthread_mutex_unlock(&SHM->beep_mutex_status);

			ioctl(*((int *)arg), BEEP_PRE, 255);

			div = (PCLK/256/4) / (MumIsTheBestInTheWorld[i].pitch);
			ioctl(*((int *)arg), BEEP_CNT, div);
			usleep(MumIsTheBestInTheWorld[i].dimation * 100); 
		}

		usleep(100000);
	}
}
