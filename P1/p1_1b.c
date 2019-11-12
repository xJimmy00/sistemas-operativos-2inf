#include <sys/types.h> //Para tipo pid_t
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Para fork()
#include <errno.h>
#include <sys/wait.h>


int main(int argc, char** argv){
	if (argc != 2){
		printf("Debe introducir 2 argumentos");
		exit(EXIT_FAILURE);
	}

	pid_t pid;
	int status;
	pid_t flag;

	for(int i = 0; i < atoi(argv[1]); i++){

		pid = fork();

		if (pid == -1){ // Caso de error al crear proceso
			perror("error");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		}
		else if (pid == 0){ // HIJO
			printf("soy el hijo %d, pid %d, mi padre pid %d\n", i+1, getpid(), getppid());
			// NO PONEMOS EXIT, SI NO, TODOS LOS HIJOS LOS CREARIA EL MISMO PADRE
		}
		else{
			/* Código para esperar a los hijos */
			while ((flag = waitpid(-1, &status, WUNTRACED | WCONTINUED)) > 0){
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
				exit(EXIT_SUCCESS); // Ponemos exit ya que el padre no va a crear mas hijos, son los procesos hijos los que a su vez crearán más
			}
			else{
				printf("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal.\n");
				exit(EXIT_FAILURE);
			}
			/* Fin código espera de hijos */
		}
	}

}