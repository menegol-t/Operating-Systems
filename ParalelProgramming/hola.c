#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) //cantidad de argumentos, los argumentos que le mando
{
        int rank, size; //Numero de proceso y cant total d procesos
        MPI_Init(&argc, &argv); //inicializa
        MPI_Comm_rank(MPI_COMM_WORLD, &rank); //consulta el numero de proceso
        MPI_Comm_size(MPI_COMM_WORLD, &size); //cantidad de procesos

        if(rank == 0)//proeso 0, se le pudiera pedir tareaws especiales
        {
                printf("Numero total de procesos: %d\n", size);
        }

        printf("Hola desde el proceso %d\n", rank);

        MPI_Finalize();
        return 0;
}