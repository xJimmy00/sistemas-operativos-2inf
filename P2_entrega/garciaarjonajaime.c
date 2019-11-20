#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define V_SIZE 10

struct argumento{
    int *v; // Puntero al elemento del vector donde debe empezar a sumar
    int n; // Número de elementos que tiene que sumar
};

void* sumaParcial(void* arg){
    struct argumento *aux = (struct argumento*)arg;
    int *sumaret;
    if ((sumaret = (int*)malloc(sizeof(int))) == NULL){
        printf("Error en la reserva de memoria");
    }
    for (int i = 0; i < aux -> n; i++){
        *sumaret += aux -> v[i];
    }
    pthread_exit((void*)sumaret);
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Cantidad de argumentos invalida\n");
        exit(EXIT_FAILURE);
    }

    int nthreads = atoi(argv[1]);
    if (nthreads < 2 || nthreads > 10){
        printf("Argumento invalido, debe estar entre 2 y 10\n");
        exit(EXIT_FAILURE);
    }
    
    int v[V_SIZE];

    for (int i = 0; i < V_SIZE; i++){
        v[i] = i; // También pueden ser números aleatorios
    }

    int* sumaret;
    int sumatotal = 0;
    int rc;

    pthread_t threads[nthreads]; // Vector de threads
    struct argumento args[nthreads]; // Vector de argumentos, a cada hilo se le pasará uno distinto
    int comienzo = 0; // indicará al hilo por donde debe empezar a sumar

    long i;
    for (i = 0; i < nthreads; i++){

        int n = V_SIZE / nthreads; // cantidad de elementos a sumar por el hilo
        if (i == nthreads - 1){ // cantidad restante si la división no es exacta
            n = V_SIZE - comienzo;
        }
        args[i].n = n;
        args[i].v = v + comienzo; // posición del vector por donde el hilo empezará a sumar

        rc = pthread_create(&threads[i], NULL, (void*)sumaParcial, (void*)&args[i]);
        if (rc){
            perror("Error al crear hilo\n");
            exit(EXIT_FAILURE);
        }
        comienzo += n;
    }

    for (i = 0; i < nthreads; i++){
        if (pthread_join(threads[i], (void**)&sumaret) != 0){
            perror("Error en pthread_join\n");
            exit(EXIT_FAILURE);
        }
        printf("Hilo %ld devuelve suma %d\n", i, *sumaret);
        sumatotal += *((int*)sumaret);
    }
    printf("Suma total = %d\n", sumatotal);
}