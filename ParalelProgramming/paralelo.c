#include <stdio.h>
#include <mpi.h>
int main(int argc, char **argv)
/*
	argc: Argument Count, es el numero de argumentos que le pasas al programa por CLI al invocarlo, incluyendo su propio nombre.
	Por ejemplo si ejecuto en la CLI:
	 $ ./paralelo.c arg1 arg2
	Entonces "argc" valdria 3, porque al programa le paso primero su mismo nombre y despues 2 argumentos mas:
	argc[0] = "./paralelo.c"
	argc[1] = arg1
	argc[2] = arg2

	**argv: Argument Vector, un puntero a un array de Strings que contiene los argumentos pasados por CLI, incluyendo el nombre del programa
	Siguiendo el ejemplo anterior, argv[0] = "./paralelo.c"
*/
{
        int left, right;
        int number, sum;
        int rank, size,root;
        int interval, mystart, myend,GrandTotal;

        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        root=0;
        left=1;
        right=1000;
        if (rank==root)
        {

        }

        MPI_Bcast(&left,1,MPI_INT,root,MPI_COMM_WORLD);
        MPI_Bcast(&right,1,MPI_INT,root,MPI_COMM_WORLD);
        //Calculo limites
        if (((right-left+1) % size) !=0){
                interval=(right-left+1) / size +1;
        }else{
                interval=(right-left+1)/size;
        }

        mystart=left + rank*interval;

        myend=mystart+interval;

        for(number = mystart; number<myend; number++)
        {
                sum = number + sum;
        }

        MPI_Reduce(&sum,&GrandTotal,1,MPI_INT,MPI_SUM,root,MPI_COMM_WORLD);
        /* 
		MPI_Reduce: Se buscan los valores distribuidos en los distintos procesos y se les aplica una operacion a todos.
		Se le pasa como parametro, la variable en todos los procesos distribuidos, la variable donde va a ir el resultado, la cantidad de veces a realizar la operacion, el tipo de variable, el tipo de operacion, con que usuario realizarla, a que procesos realziarla.

	        &sum: Es la variable que se tomara de todos los subprocesos (input).
        	&grandTotal: Es la variable final a dondeira resultado (output).
	        1: Es cuantas veces se va a realizar la operacion indicada.
        	MPI_INT: int es el tipo de variable
	        MPI_SUM: sum es la operacion a realizar (vas a hacer la sumatoria)
        	root: es para que haga todo en el root principal
	        MPI_COMM_WORLD: Le indica que realice la operacion con todos los procesos
	*/

        MPI_Barrier(MPI_COMM_WORLD);//Esperame a que terminen de ejecutar todos los proceso
        if (rank==root){
                printf("Total: %d\n",GrandTotal);
        }

        MPI_Finalize();
        return 0;
}