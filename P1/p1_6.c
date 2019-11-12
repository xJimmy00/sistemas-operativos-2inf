#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int i = 0;

void sig_alrm(int signo)
{
	printf("RING\n");
	printf("Value of signo:%d\n",signo);
	i++;
	if (i >= 4){
		kill(getpid(), 9);
	}
	return;	/* nothing to do, just return to wake up the pause */
}

unsigned int f1(unsigned int nsecs)
{
	alarm(nsecs);		/* start the timer */
	pause();			/* pause - suspend the thread until a signal is received */
}

int main()
{

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
	{
		perror("Signal error");
		printf("errno value= %d\n", errno);
		exit(EXIT_FAILURE);
	}
	while(1){
		f1(5);
		f1(3);
		f1(1);
		f1(1);
	}
	printf ("Fin del programa\n");
}