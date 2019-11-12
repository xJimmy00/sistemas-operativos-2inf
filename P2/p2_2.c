#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
struct Fichero{
	int indice;
	char nombre_fichero[100];
};

void * cuentaLineas(void *p){
	struct Fichero *aux = (struct Fichero *) p;
	FILE* f;
	int contador = 0;
	char linea[1000];
	if ((f = fopen(aux -> nombre_fichero, "r")) == NULL){
		printf("Error al abrir el fichero %s \n", aux -> nombre_fichero);
		exit(EXIT_FAILURE);
	}
	while(fgets(linea, 1000, f)){
		contador++;
	}
	fclose(f);

	printf("Hilo %d, numero de lineas %ld\n", aux->indice, contador);
	pthread_exit((void*)contador);
}


int main(int argc, char **argv){
	if (argc == 1){
		printf("Debe especificar argumentos");
		exit(EXIT_FAILURE);
	}
	int n,rc;
	long i;

	int sumaFinal = 0;
	int contador;
	struct Fichero * fich;

	if((fich = (struct Fichero *)malloc(sizeof(struct Fichero))) == NULL){
		printf("Error en la reserva de memoria");
		exit(EXIT_FAILURE);
	}

	pthread_t threads[argc - 1];
	for (i = 1; i < argc; i++){
		fich[i].indice = i;
		strcpy(fich[i].nombre_fichero, argv[i]);
		rc = pthread_create(&threads[i], NULL, (void*)cuentaLineas, (void*) &fich[i]);
	}

	for (i = 1; i < argc; i++){
		pthread_join(threads[i], (void**) &contador);
		printf("Hilo %d devuelve suma %ld\n", i, contador);
		sumaFinal += contador;
	}

	printf("\n");

	printf("suma final es:%d\n", sumaFinal);
}