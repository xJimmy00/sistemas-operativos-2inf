#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NPROCESOS 5

typedef struct Params{
    char nombre[8];
    // Se introducen
    int t_ejec;
    int t_lleg;
    // Se calculan
    int t_com;
    int t_fin;
    int t_ret;
    int t_esp;
    // Info adicional
    int ciclos_ejecutados;
} Parametros;

void main(){
    Parametros procesos[NPROCESOS];
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
    
    int ciclo = 0;
    int actual = -1; // valdrá -1 si no hay ningún proceso ejecutándose
    int procesos_ejecutados = 0;
    
    while (procesos_ejecutados < NPROCESOS){
        if (actual == -1){
            // Elige el proceso
            for (int i = 0; i < NPROCESOS; i++) {

                if (procesos[i].ciclos_ejecutados != procesos[i].t_ejec){ // comprueba si el proceso no es uno que ya haya finalizado
                    if (actual == -1){
                        actual = i;
                    }
                    else if (procesos[i].t_ejec < procesos[actual].t_ejec && ciclo >= procesos[i].t_lleg) {
                        actual = i;
                    }
                }
            }
            procesos[actual].t_com = ciclo;
        }

        procesos[actual].ciclos_ejecutados++;

        if (procesos[actual].ciclos_ejecutados == procesos[actual].t_ejec){ // comprobamos ciclos restantes
            procesos[actual].t_fin = ciclo + 1;
            procesos[actual].t_ret = procesos[actual].t_fin - procesos[actual].t_lleg;
            procesos[actual].t_esp = procesos[actual].t_ret - procesos[actual].t_ejec;

            actual = -1; 
            procesos_ejecutados++;
        }

        ciclo++; // Siguiente ciclo
    }

    printf("\nProceso | t_comienzo | t_final | t_retorno | t_espera\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < NPROCESOS; i++) {
        printf("   %s \t    %d \t\t %d \t   %d \t      %d\n", procesos[i].nombre, procesos[i].t_com, procesos[i].t_fin, procesos[i].t_ret, procesos[i].t_esp);
    }

}