#include <stdio.h>
#include <omp.h>
static long num_steps=10;
int main(){
        int i;
        int sum=0;

        #pragma omp parallel for reduction(+:sum)
        for (i=0;i<=num_steps;i++)
        {
                int num = omp_get_thread_num();
                printf("Numero de hilo: %d\n suma previa %d numero de iteracion %d.\n", num, sum, i);
                sum=sum+i;
                printf("Numero de hilo: %d\n suma actualizada %d. \n", num, sum);
        }

        printf("suma: %d\n", sum);
        return 0;
}