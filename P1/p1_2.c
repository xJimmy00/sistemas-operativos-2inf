#include <sys/types.h> //Para tipo pid_t
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Para fork()
#include <errno.h>
#include <sys/wait.h>


int main(){

	pid_t childpid;
	int status;
	pid_t pid = fork();
	switch(pid){

		case -1:
			perror("error");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);

		case 0: /* hijo */
			printf("soy el hijo pid %d, mi padre pid %d\n", getpid(), getppid());
			exit(EXIT_SUCCESS);

		default: /* padre */
			sleep(20);
			exit(EXIT_SUCCESS);
			childpid = wait(&status);
			if(childpid > 0){
				if (WIFEXITED(status)){
					printf("child %d exited, status=%d\n",childpid, WEXITSTATUS(status));
				}
				else if (WIFSIGNALED(status)){
					printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
				}
				else if (WIFSTOPPED(status)){
					printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
				}
			}
			else{
				printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
				exit(EXIT_FAILURE);
			}

	}

}