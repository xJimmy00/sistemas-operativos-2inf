#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct Dato{
	int *v;
	int **M;
	int i;
};

int ** reservaMatriz(){
	int **M;
	if((M = (int**)malloc(3 * sizeof(int*))) == NULL){
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < 3; i++){
		if ((M[i] = (int*)malloc(3 * sizeof(int))) == NULL){
			exit(EXIT_FAILURE);
		}
	}
	return M;
}

void *multiplicaMatrizVector(void *dato){
	int *suma = malloc(sizeof(int));
	*suma = 0;
	struct Dato dato1 = *(struct Dato*)dato;
	for (int j = 0; j < 3; j++){
		*suma = *suma + dato1.M[dato1.i][j] * dato1.v[j];
	}
	pthread_exit((void*) suma);
}

int main(){
	int i,j;
	int rc;
	int **M = reservaMatriz();
	int *retorno;
	int suma[3];
	int suma1 = 0;
	int *v;
	if ((v = malloc(3*sizeof(int))) == NULL){
		exit(EXIT_FAILURE);
	}

	pthread_t threads[3];
	struct Dato *dato1;
	if ((dato1 = malloc(sizeof(struct Dato))) == NULL){
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < 3; i++){
		v[i] = 2;
	}

	for (i = 0; i < 3; i++){
		for (j = 0; j < 3; j++){
			M[i][j] = 2;
		}
	}
	dato1 -> v = v;
	dato1 -> M = M;

	for (i = 0; i < 3; i++){
		dato1 -> i = i;
		rc = pthread_create(&threads[i], NULL, (void*)multiplicaMatrizVector, (void*)dato1);
		if (rc){
			perror("Error al crear la hebra\n");
			exit(EXIT_FAILURE);
		}
		pthread_join(threads[i], (void**)&retorno);
		suma1 = *(int*)retorno;
		suma[i] = suma1;
	}

	for (i = 0; i < 3; i++){
		printf("Total fila %d: %d\n", i, suma[i]);
	}
}