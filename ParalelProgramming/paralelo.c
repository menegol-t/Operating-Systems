#include <stdio.h>
#include <mpi.h>

//Calcula la sumatoria entre 2 numeros. Por ejemplo la sumatoria entre 1 y 10 es: 1 + 2 + 3... + 10 = 55

int main(int argc, char **argv)
{
        int left = 1;           //Left es desde donde empieza la sumatoria (1)
        int right = 1000;       //Right donde termina la sumatoria (1000)
        int number = 0;         //Number es el numero actual que hay que sumar (por ej 6, si es que ya sumamos hasta el numero 5)
        int sum = 0;            //Sum es la sumatoria hasta ese momento (por ej ya llevamos sumando 1+2+3+4+5 = 15. Sum seria 15)
        int rank, size;         //Rank es un int unico para cada subproceso, size es la cantidad de procesos. Ambos se definen despues
        int root = 0;           //Root va a ser el rank del proceso principal = 0, o sea el root process, que se va a encargar de coordinar a los demas
        int interval, mystart, myend, GrandTotal;
//Va a funcionar asi, vamos a divir la sumatoria en multiples sumatorias chiquititas, y dividir esas en los procesos.
//Por ej si tengo 2 procesos, uno va a calcular la sumatoria desde 1 hasta 5, y otro desde 5 hasta  10.
//Para esto, vamos a usar la variable interval que va a decir "bueno vos calcula desde el 1 hasta el 5" y "vos desde el 5 hasta el 10"
//MyStart va a ser como left, pero para cada proceso individual. MyEnd lo mismo.
//GrandTotal va a ser manipulada por el root, va a ser quien "sume las sumatorias"

        MPI_Init(&argc, &argv);  //Inicio Message Passing Interface con todos los argumentos que le haya pasado al programa por cli.
//MPI_COMM_WORLD: Es un comunicador de MPI que representa a todos los procesos en el programa

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);   //Settea la variable Rank con un int que representa unicamente al proceso actual que ejecuta el codigo.
        MPI_Comm_size(MPI_COMM_WORLD, &size);   //Settea la variable size con un int que representa el total de procesos en el comunicador

        MPI_Bcast(&left, 1, MPI_INT, root, MPI_COMM_WORLD);
/*
        Hace un broadcast (enviar data) desde el proceso root hacia todos los procesos. Parametros:
        &left: dato a enviar
        1: la cantidad de datos que se envian (en este caso solo el valor de la variable left)
        MPI_INT: Especifica el tipo de dato siendo enviado, en este caso, un INT
        root: El rank del proceso desde el cual se hara broadcast de los datos
        MPI_COMM_WORLD: El comunicador, en este caso es el comunicador global, que enviara ese dato a todos los procesos.
*/
        MPI_Bcast(&right, 1, MPI_INT, root, MPI_COMM_WORLD);

        if (((right - left + 1) % size) !=0)
        {
                interval = (right - left + 1) / size + 1;
        }else{
                interval = (right - left + 1) / size;
        }

        mystart = left + rank * interval;

        myend = mystart + interval;

        if (myend > right)
        {
                myend = right + 1 ;
                sum = root; // Suma localmente en cada proceso MPI 
        }
        if (mystart <= right)
        {
                for(number = mystart; number < myend; number++)
                {
                        sum = number + sum;
                }
        }

        MPI_Reduce(&sum, &GrandTotal, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
        /*
                MPI_Reduce: Se buscan los valores distribuidos en los distintos procesos y se les aplica una operacion a todos.
Se le pasa como parametro: La variable en todos los procesos distribuidos, la variable donde va a ir el resultado, la cantidad de veces a realizar la operacion, el tipo de variable, el tipo de operacion, con que usuario realizarla, a que procesos realziarla.

                &sum: Es la variable que se tomara de todos los subprocesos (input).
                &grandTotal: Es la variable final a dondeira resultado (output).
                1: Es cuantas veces se va a realizar la operacion indicada.
                MPI_INT: int es el tipo de variable
                MPI_SUM: sum es la operacion a realizar (vas a hacer la sumatoria)
                root: es para que haga todo en el root principal
                MPI_COMM_WORLD: Le indica que realice la operacion con todos los procesos
        */

        MPI_Barrier(MPI_COMM_WORLD);    //Esperame a que terminen de ejecutar todos los procesos

        if (rank == root){
                printf("Total: %d\n", GrandTotal);
        }

        MPI_Finalize();
        return 0;
}


// #include "mpi.h"
// #include <stdio.h>
// int main(int argc, char **argv)
// {
//         int rank, size;
//         int root ;
//         int left, right, interval ;
//         int number, start, end, sum, GrandTotal;
//         int mystart, myend;
//         MPI_Init(&argc, &argv);
//         MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//         MPI_Comm_size(MPI_COMM_WORLD, &size);
//         root = 0;
//         left =1;
//         right=1000;
//         if (rank == root)
//         {
//                 printf("Prcoeso 0: valor inicial %d, final : %d\n", left, right);
//         }
//         MPI_Bcast(&left, 1, MPI_INT, root, MPI_COMM_WORLD); //Envia los limites a todos los procesos
//         MPI_Bcast(&right, 1, MPI_INT, root, MPI_COMM_WORLD);
//         if (((right - left + 1) % size) != 0)
//                 interval = (right - left + 1) / size + 1 ; //Fija límites locales de suma
//         else
//                 interval = (right - left + 1) / size;
//         mystart = left + rank*interval ;
//         myend = mystart + interval ;
//         // establece los límites de los intervalos correctamente 
//         if (myend > right)
//                 myend = right + 1 ;
//                 sum = root; // Suma localmente en cada proceso MPI 
//         if (mystart <= right)
//         for (number = mystart; number < myend; number++)
//                 printf("Proceso: %d Suma acumulada: %d Suma del proceso: %d\n",rank,sum,number);
//         // Hace la reducción en el proceso raíz 
//         MPI_Reduce(&sum, &GrandTotal, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD) ;
//         MPI_Barrier(MPI_COMM_WORLD);
//         // El proceso raíz retorna los resultados 
//         if(rank == root)
//                 printf("Suma total: Grand total = %d \n", GrandTotal);
//         MPI_Finalize();
// }