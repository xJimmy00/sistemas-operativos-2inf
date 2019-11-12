#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

void numerosAleatorios(void* arg){
	float* numeros;
	if ((numeros = (float*)malloc(2*sizeof(float))) == NULL){
		printf("Error en la reserva de memoria");
		exit(-1);
	}
	srand(time(NULL));
	numeros[0] = (rand() % 2000) / 100;
	numeros[1] = (rand() % 2000) / 100;
	printf("V[0] = %f\n", numeros[0]);
	printf("V[1] = %f\n", numeros[1]);
	pthread_exit((void*)numeros);
}

int main(void){
	int n;
	printf("Numero de hebras: \n");
	scanf("%d", &n);

	pthread_t threads[n];
	int rc;
	long i;

	float sumatotal = 0;
	float* v;
	float* v2;
	if ((v2 = (float*)malloc(2*sizeof(float))) == NULL){
		printf("Error en la reserva de memoria");
		exit(-1);
	}

	for (i = 0; i < n; i++){
		printf("Creado hebra %ld\n", i);
		rc = pthread_create(&threads[i], NULL, (void*)numerosAleatorios, (void*)NULL);
		if (rc){
			printf("Error: valor devuelto pthread_create(): %d\n", rc);
			exit(EXIT_FAILURE);
		}
		rc = pthread_join(threads[i], (void**)&v);
		if (rc){
			printf("Error, valior devuelto pthread_join(): %d\n", rc);
			exit(EXIT_FAILURE);
		}
		v2 = (float*)v;
		sumatotal = sumatotal + v2[0] + v2[1];
		printf("Suma = %f\n", sumatotal);
	}
	pthread_exit(NULL);
}
