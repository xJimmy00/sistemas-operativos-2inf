#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <math.h>
#define SLOTS 100
#define ELEMENTOS 1000

int v[SLOTS]; // Vector de buffer
int indice; // Último elemento disponible
sem_t mutex, full, empty; // Semáforos

int sacarDato(int *v){
	if (indice == 0){
		return 0;
	}
	else{
		indice--;
		int dato = v[indice];
		return dato;
	}
}

void *producir(){
	int* suma;
	if ((suma = malloc(sizeof(int))) == NULL){
		printf("Error en la reserva de memoria\n");
		exit(EXIT_FAILURE);
	}
	*suma = 0;

	for(int i = 0; i < ELEMENTOS; i++){
		int dato = rand() % 11; // producir Dato

		sem_wait(&empty);
		sem_wait(&mutex);

		// entrar Dato
		v[indice] = dato;
		indice++;

		sem_post(&mutex);
		sem_post(&full);

		*suma += dato;
	}

	pthread_exit((void*)suma);
}

void *consumir(void* arg){
	int *elementos_consumir = (int*)arg;
	int *suma;
	if ((suma = malloc(sizeof(int))) == NULL){
		printf("Error en la reserva de memoria\n");
		exit(EXIT_FAILURE);
	}
	*suma = 0;

	for(int i = 0; i < (*elementos_consumir); i++){
		sem_wait(&full);
		sem_wait(&mutex);
		int dato = sacarDato(v);
		sem_post(&mutex);
		sem_post(&empty);
		*suma += dato; // consumir Dato
	}

	pthread_exit((void*)suma);
}

int main(){
	// Inicialización de variables
	for (int i = 0; i < SLOTS; i++){
		v[i] = 0;
	}
	indice = 0;

	srand(time(NULL));
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, SLOTS);

	int nProductores, nConsumidores;

	printf("Introduce numero de productores\n");
	scanf("%d", &nProductores);

	printf("Introduce numero de consumidores\n");
	scanf("%d", &nConsumidores);

	// Creamos los hilos
	pthread_t productores[nProductores];
	pthread_t consumidores[nConsumidores];

	// Suma de cada uno de los productores y consumidores
	int *suma_productor;
	int *suma_consumidor;

	int suma_total_p = 0, suma_total_c = 0; // Sumas totales

	if ((suma_productor = malloc(sizeof(int))) == NULL){
		printf("Error en la reserva de memoria (main)");
		exit(EXIT_FAILURE);
	}
	if ((suma_consumidor = malloc(sizeof(int))) == NULL){
		printf("Error en la reserva de memoria (main)");
		exit(EXIT_FAILURE);
	}

	*suma_productor = 0;
	*suma_consumidor = 0;

	int elementos_consumidos = 0;
	int elementos_consumir[nConsumidores]; // Vector de argumentos para los hilos

	int i;
	for(i = 0; i < nProductores; i++){
		int rc = pthread_create(&productores[i], NULL, (void *)producir, NULL);
		if (rc){
			printf("Error al crear hilo %d de los productores\n", i);
			exit(EXIT_FAILURE);
		}
	}
	for(i = 0; i < nConsumidores; i++){
		if (i == (nConsumidores - 1)){
			elementos_consumir[i] = nProductores * ELEMENTOS - elementos_consumidos; // División de los elementos para cada consumidor
		}
		else{
			elementos_consumir[i] = (nProductores * ELEMENTOS) / nConsumidores; // Cantidad restante si la división no es exacta
		}

		int rc = pthread_create(&consumidores[i], NULL, (void *)consumir, (void*)&elementos_consumir[i]);
		if (rc){
			printf("Error al crear hilo %d de los consumidores\n", i);
			exit(EXIT_FAILURE);
		}

		elementos_consumidos += elementos_consumir[i]; // Sumamos los elementos que se han consumido
	}

	for(i = 0; i < nProductores; i++){
		if (pthread_join(productores[i], (void**)&suma_productor) != 0){
			printf("Error en pthread_join del productor %d\n", i);
			exit(EXIT_FAILURE);
		}
		printf("Suma productor %d, %d\n", i, *suma_productor);
		suma_total_p += *suma_productor;
	}

	for(i = 0; i < nConsumidores; i++){
		if (pthread_join(consumidores[i], (void**)&suma_consumidor) != 0 ){
			printf("Error en pthread_join del consumidor %d\n", i);
			exit(EXIT_FAILURE);
		}
		printf("Suma consumidor %d, %d\n", i, *suma_consumidor);
		suma_total_c += *suma_consumidor;
	}

	printf("Suma total productores: %d\n", suma_total_p);
	printf("Suma total consumidores: %d\n", suma_total_c);

}