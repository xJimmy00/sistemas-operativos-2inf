#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void tratarSennal(int signo)
{
	printf("Mensaje\n");
	printf("Value of signo:%d\n",signo);
	return;	/* nothing to do, just return to wake up the pause */
}

int main ()
{
	pid_t pid;
	pid_t flag;
	int status;
	pid = fork();
	if (pid == -1){
		perror("Error");
	}
	else if (pid == 0){
		while(1){
			if(signal(SIGUSR1, tratarSennal) == SIG_ERR){
				printf("Signal error");
			}
			pause();
		}
	}
	else{
		int i = 0;
		while(1){
			sleep(1);
			kill(pid, SIGUSR1);
			i++;
			if(i >= 5){
				kill(getpid(), SIGKILL);
			}
		}
		while ((flag = waitpid(-1, &status, WUNTRACED | WCONTINUED)) > 0) {
			if (WIFEXITED(status)){
				printf("Proceso Padre, Hijo con PID %ld finalizado, status = %d\n", (long int)flag, WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status)){
				printf("Proceso Padre, Hijo con PID %ld finalizado al recibir la señal %d\n", (long int)flag, WTERMSIG(status));
			}
			else if (WIFSTOPPED(status)){
				printf("Proceso Padre, Hijo con PID %ld parado al recibir la señal %d\n", (long int)flag,WSTOPSIG(status));
			}
			else if (WIFCONTINUED(status)){
				printf("Proceso Padre, Hijo con PID %ld reanudado\n",(long int) flag);
			}
		}
		if (flag == (pid_t)-1 && errno == ECHILD){
			printf("Proceso Padre, valor de errno = %d, definido como: %s\n", errno, strerror(errno));
		}
		else{
			printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
			exit(EXIT_FAILURE);
		}
	}
}