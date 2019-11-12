#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global = 0;

void *hiloFuncion(void* arg){
	for (int i = 0; i < 50; i++){
		global++;
	}
	pthread_exit(NULL);
}

int main(){
	pthread_t threads[2];

	for (int i = 0; i < 2; i++){
		pthread_create(&threads[i], NULL, (void*)hiloFuncion, (void*)NULL);
		pthread_join(threads[i], NULL);
	}
	printf("%d\n", global);
}