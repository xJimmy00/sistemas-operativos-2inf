#include <stdbool.h>
#include <stdio.h>
#define N 5

bool eligiendo[N];
int turno[N];

void Proceso(int i){
    extern bool eligiendo[N];
    extern int turno[N];
    while (true){
        int max = 0;
        eligiendo[i] = true;
        for (int k = 0; k < (N - 1); k++){
            if (turno[k] > max){
                max = turno[k];
            }
        }
        turno[i] = max;
        eligiendo[i] = false;
        for(int j = 0; j < N; j++){
            while(eligiendo[j]);
            while((turno[j] != 0) && ((turno[j] < turno[i]) || (turno[j] == turno[i] && j < i)));
        }
        //--
        // seccion critica
        //--
        turno[i] = 0;
        // seccion residual
    }
}

int main(){
    for (int i = 0; i < N; i++){
    eligiendo[i] = false;
    turno[i] = 0;
}
}