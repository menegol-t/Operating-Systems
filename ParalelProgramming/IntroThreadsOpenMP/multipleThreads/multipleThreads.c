#include <stdio.h>
#include <omp.h>
int main(){
        int total = omp_get_thread_num();
        printf("Hilo: %d en region secuencial. \n", total);

        #pragma omp parallel //inicio área paralela
        {
                #pragma omp sections //inicio secciones
                {
                        #pragma omp section
                        {
                                int hilo = omp_get_thread_num();
                                printf("Section 0 ejecutada por hilo: %d\n", hilo);
                        }
                        #pragma omp section
                        {
                                int hilo = omp_get_thread_num();
                                printf("Section 1 ejecutada por hilo: %d\n", hilo);
                        }
                        #pragma omp section
                        {
                                int hilo = omp_get_thread_num();
                                printf("Section 2 ejecutada por hilo: %d\n", hilo);
                        }
                }
        }
        total = omp_get_thread_num();
        printf("Thread: %d en region secuencial.\n", total);
        return 0;
}