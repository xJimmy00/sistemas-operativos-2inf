#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NPROCESOS 5
#define Q 3

// structs necesarias
typedef struct Params{
    char nombre[14];
    int t_ejec;
    int t_lleg;
    int t_com;
    int t_fin;
    int t_ret;
    int t_esp;

    int ciclos_ejecutados;
} Parametros;

typedef struct Cola{
    int elemento;
    struct Cola* siguiente;
} Cola;

// Función que introduce un proeso en la cola
void entrarCola(Cola** inicio, int p){
    Cola* nodo = (Cola*)malloc(sizeof(Cola));
    if (nodo != NULL){
        nodo -> elemento = p;
        nodo -> siguiente = NULL;
    }else{
        printf("Error en la reserva de memoria\n");
        exit(EXIT_FAILURE);
    }

    if (*inicio == NULL){
        *inicio = nodo;
    }else{
        Cola* aux = *inicio;
        while (aux -> siguiente != NULL){
            aux = aux -> siguiente;
        }
        aux -> siguiente = nodo;
    }
}

// Función que saca un proceso de la cola
int salirCola(Cola** inicio){
    if (*inicio != NULL){
        int p = (*inicio) -> elemento;
        Cola* aux = *inicio;
        *inicio = aux -> siguiente;
        free(aux);
        return p;
    }else{
        printf("La cola esta vacia\n");
        exit(EXIT_FAILURE);
    }
}

void main(){
    Parametros procesos[NPROCESOS]; // inicialización
    strcpy(procesos[0].nombre, "A");
    procesos[0].t_ejec = 8;
    procesos[0].t_lleg = 0;
    procesos[0].ciclos_ejecutados = 0;

    strcpy(procesos[1].nombre, "B");
    procesos[1].t_ejec = 4;
    procesos[1].t_lleg = 1;
    procesos[1].ciclos_ejecutados = 0;

    strcpy(procesos[2].nombre, "C");
    procesos[2].t_ejec = 9;
    procesos[2].t_lleg = 2;
    procesos[2].ciclos_ejecutados = 0;

    strcpy(procesos[3].nombre, "D");
    procesos[3].t_ejec = 5;
    procesos[3].t_lleg = 3;
    procesos[3].ciclos_ejecutados = 0;

    strcpy(procesos[4].nombre, "E");
    procesos[4].t_ejec = 2;
    procesos[4].t_lleg = 4;
    procesos[4].ciclos_ejecutados = 0;
    
    int actual = -1; // valdrá -1 si no hay ningún proceso ejecutándose
    int procesos_terminados = 0;
    int ciclo = 0;
    int q = 0; // cuanto

    Cola* cola = NULL; // creamos la cola

    for (int i = 0; i < NPROCESOS; i++){
        entrarCola(&cola, i); // introduce procesos en la cola
    }
    while (procesos_terminados < NPROCESOS) {
        // algoritmo de turno rotatorio
        if (actual == -1) {
            actual = salirCola(&cola);
            procesos[actual].t_com = ciclo;
        }

        procesos[actual].ciclos_ejecutados++;
        q++;

        if (procesos[actual].ciclos_ejecutados == procesos[actual].t_ejec) { // comprobamos si se ha acabado el tiempo de ejecución
            procesos[actual].t_fin = ciclo + 1;
            procesos[actual].t_ret = procesos[actual].t_fin - procesos[actual].t_lleg;
            procesos[actual].t_esp = procesos[actual].t_ret - procesos[actual].t_ejec;

            actual = -1; 
            q = 0;
            procesos_terminados++;
        }
        else if (q == Q){ // comprobamos el cuanto de tiempo
            entrarCola(&cola, actual);
            q = 0;
            actual = -1;
        }

        ciclo++; // pasamos al siguiente ciclo y repetimos
    }

    printf("\nProceso | t_comienzo | t_final | t_retorno | t_espera\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < NPROCESOS; i++) {
        printf("   %s \t    %d \t\t %d \t   %d \t      %d\n", procesos[i].nombre, procesos[i].t_com, procesos[i].t_fin, procesos[i].t_ret, procesos[i].t_esp);
    }
}
