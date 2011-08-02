#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void init_daemon(void)
{
	pid_t pid;
	//int i;

	if (-1 == (pid = fork()))
	{
		perror("Failed to fork first child in init_daemon()");
		exit(-1);
	}
	else if (0 < pid)
	{
		exit(0);
	}

	setsid();

	if (-1 == (pid = fork()))
	{
		perror("Failed to fork second child in init_daemon()");
		exit(-1);
	}
	else if (0 < pid)
	{
		exit(0);
	}

#if 0
	for (i = 0; i < getdtablesize(); i++)
	{
		close(i);
	}
#endif

	chdir("/tmp");
	umask(0);
}
