#include <stdio.h>
#include <omp.h>
int main(){

        omp_set_num_threads(4);         //Setea la cantidad de threads que se utilizaran

        int num = omp_get_thread_num(); //Cuenta la cantidad de thrads

//Hasta este momento, el codigo se viene ejecutando de manera secuencial, 1 solo thread. 

        printf("Numero de hilo: %d en región secuencial.\n", num);

//Lo pragma es una "directiva" para el compilador. "omp" es OpenMP API. "Parallel" indica que el siguiente bloque de codigo debe ser ejecutado en paralelo por multiples threads.
        #pragma omp parallel
        {
                int thread = omp_get_thread_num();  //Me da el ID del thread actual
                printf("Numero de hilo: %d en región paralela.\n", thread);
        }
//A partir de aca, mueren los threads creados y vuelve a ejecutarse todo de manera secuencial, continua el mismo thread padre que en la linea 9. 

        num = omp_get_thread_num();
        printf("Numero de hilo: %d en región secuencial.\n", num);
}