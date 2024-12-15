#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])//cantidad de argumentos, los argumentos que le mando
{
        int rank, size;         //Inicializa las variables: Rank: El "numero del proceso" dentro de un grupo de procesos. Size: La cantidad de procesos totales
        MPI_Init(&argc, &argv); //inicializa el Message Passing Interface con todos los argumentos que le pasamos al programa

//MPI_COMM_WORLD es el comunicador default, que dentro tiene a todos los procesos. 

        MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Setea rank como el ID del proceso individual dentro del grupo.
        MPI_Comm_size(MPI_COMM_WORLD, &size); //Se tea size como el numero total de procesos dentro de size.

        if(rank == 0)   //Si yo soy el prcoeso 0, es decir el root, el primer y principal procesos.
        {
                printf("Numero total de procesos: %d\n", size);
        }

        printf("Hola desde el proceso %d\n", rank);     //Que todos los procesos indiquen su ID. 

        MPI_Finalize();
        return 0;
}