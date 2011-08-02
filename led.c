#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "led.h"

void *led_thread_handler(void *arg)
{
	int fd;

	if (0 > (fd = open("/dev/LED", O_RDWR)))
	{
		perror("Failed to open LED");
		exit(-1);
	}

	ioctl(fd, LED_ON, 0);
	close(fd);
}
