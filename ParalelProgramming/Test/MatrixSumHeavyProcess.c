#include <stdio.h>
#include <mpi.h>

void rellenarMatriz(int alto, int ancho, int matriz[alto][ancho])
{
//Rellena todos los vectores de la matriz con numeros desde 1 hasta "ancho".
        for(int i = 0; i < alto; i++)
        {
                for(int j = 0; j < ancho; j++)
                {
                        matriz[i][j] = j + 1;
                }
        }
}
/*
Escribir un programa que realice la suma de los elementos de cada fila de una matriz, cada
valor obtenido debe ser almacenado en un vector que debe tener tantos e elementos como fila
tenga la matriz.
Ejemplo:
___________     ____
| 3|4| 7|5|  => |19|
| 1|6| 9|8|  => |24|
|-3|6|10|3|  => |16|
-----------     ----
*/
int main(int argc, char **argv)
{
        //Voy a hardcodear el largo y ancho de la matriz. Si lo quiero cambiar, solo cambio estas variables.
        int alto = 3;
        int ancho = 3;
        int matriz[alto][ancho];
        int rank, size;

        //El resultado es un vector que tiene que tener la sumatoria de cada una de las filas de la matriz
        int resultado[alto];

        //Esta variable la uso para determinar si el proceso tiene que trabajar o no, en caso de tener mas workers que carga para repartir
        int workerTrabajador = 0;

        //Inicializo MPI, seteo las variables rank y size
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        //Si estoy en el proceso principal
        if(rank == 0)
        {
                //Llamo una funcion que rellene la matriz arbitrariamente. Tambien podria hardcodear los numeros.
                rellenarMatriz(alto, ancho, matriz);

/*
Como repartir la carga en los procesos: El proceso root le va a pasar a los demas procesos un vector, fila por fila, de la matriz, para que calculen su sumatoria.
Para distribuirlos, le damos la primera fila al primer proceso. La segunda al segundo y asi.
--> Que pasa si tengo mas carga que procesos disponibles?
Si nos quedamos sin procesos y todavia quedan filas que asignar, entregamos la fila que queda por asignar al proceso 1, la siguiente al proceso 2 y asi vamos repitiendo hasta asignar todos los procesos.

--> Que pasa si tengo mas procesos disponibles que carga para repartir? Los workers se quedan esperando mensajes que nunca recibieran.
Por eso despues de mandar a los workers necesarios a trabajar, al resto les mando un mensaje con un tag que les diga "vos no tenes trabajo, recibi tu mensaje y corta"

*/
                //Contador para balancear la asignacion de cada vector a cada proceso
                int nroDeProceso = 1;
                //El mensaje que le doy a todos los workers para que sepan si tienen que trabajar o no.En este caso, voy a enviar trabajo a tantos workers como filas tenga la matriz
                workerTrabajador = 1;
                for(int i = 0; i < alto; i++)
                {
                        //A todos los workers que van a trabajar, les envio el numero 1
                        MPI_Send(&workerTrabajador, 1, MPI_INT, nroDeProceso, 0, MPI_COMM_WORLD);

                        //A cada proceso le envio una fila de la matriz, su cantidad de elementos, el tipo de dato de cada uno, el nombre del proceso, un tag y el comunicador.
                        MPI_Send(matriz[i], ancho, MPI_INT, nroDeProceso, 0, MPI_COMM_WORLD);

                        nroDeProceso++;

                        //Ponele que tengo una matriz de 6 filas y solo 3 procesos, bueno la cuarta fila la envio nuevamente al primer proceso, la quinta fila al segundo proceos, y asi hasta que no queden vectores sin asignar.
                        if(nroDeProceso > size)
                        {
                                nroDeProceso = 1;
                        }
                }

                //Aca le envio la señal "0" a todos los workers que no hace falta que trabajen si ese es el caso. Si no es el caso, por ejemplo tengo mas trabajo que procesos, esto nunca se da.
                workerTrabajador = 0;

                for(int i = alto; i < size; i++)
                {
                        MPI_Send(&workerTrabajador, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }

                //En esta parte voy recibiendo el resultado de todos los procesos worker que trabajron
                nroDeProceso = 1;

                for(int i = 0; i < alto; i++)
                {
                        //Primero vacio el vector de resultados de cualquier cosa que pudiese tener la memoria al iniciar el programa si no C lloriquea
                        resultado[i] = 0;

                        //Las sumatorias que voy recibiendo las guardo en el vector resultado
                        MPI_Recv(&resultado[i], 1, MPI_INT, nroDeProceso, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                        //Utilizo esta variable para rotar por todos los procesos sacando sus resultados. Primero obtengo el del primer proceso, despues el segundo, y asi. Si estoy en el caso de que tenia mas sumatorias que calcular, que procesos disponibles, vuelvo a recibir la sumatoria del 1, despues el 2, y asi.
                        nroDeProceso++;

                        if(nroDeProceso > size)
                        {
                                nroDeProceso = 1;
                        }
                }

                //Muestro el resultado
                for(int i = 0; i < alto; i++)
                {
                        printf("Sumatoria fila nro %d: %d\n", i, resultado[i]);
                }
        }else
        {
                //A partir de aca, es el trabajo de los workers.
                int vectorPorSumar[ancho];
                //Inicializo la sumatoria en 0
                int sumatoria = 0;

                //Primero recibo la señal indicando si tengo que trabajar o no
                MPI_Recv(&workerTrabajador, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                //Si a mi worker le dieron trabajo, se pone a recibirlo. Si no, sencillamente no hace nada y finaliza.
                if(workerTrabajador == 1)
                {
                        //Recibo el vector a realizar la sumatoria del proceso root
                        MPI_Recv(vectorPorSumar, ancho, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                        for(int i = 0; i < ancho; i++)
                        {
                                //Realizo la sumatoria de todos los numeros del vector
                                sumatoria += vectorPorSumar[i];
                        }
                        //Envio al sumatoria al proceso root.
                        MPI_Send(&sumatoria, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
        }

        MPI_Finalize();
        return 0;
}