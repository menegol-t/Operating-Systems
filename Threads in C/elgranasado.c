# include  <stdio.h>
# include <pthread.h>
# include <semaphore.h>

int m = 3;              //Cant de mozos
int n = 4;              //Cant de invitados
int platosServidos = 0;

//Semaforos a utilizar
sem_t maruchoSentado;

void servirComida(int mozo_id){
        sem_wait(&maruchoSentado);

        platosServidos++;
        printf("Mozo %d sirvio un plato.\n", mozo_id);

        sem_post(&maruchoSentado);
}

//Esta seria la funcion "principal" de mozos, que deberia hacer llamadas a otras funciones y a semaforos.
void* mozo(void* arg){

        int mozo_id = *(int*)arg;                       //Busco el ID de cada thread
        printf("Mozo %d llego a la fiesta.\n", mozo_id);//Imprimo el ID de cada thread

        servirComida(mozo_id);

        pthread_exit(NULL);                             //Thread termina su laburo
}

int main (){

        //Defino los threads de los mozos
        pthread_t mozos[m];                             //Array de mozos
        int mozos_id[m];                                //Array que va a contener los ID de los threads

        //Inicializo semaforos y les defino su primer estado
        sem_init(&maruchoSentado, 0, 0);                //Este semaforo se va a usar para que los mozos esperen a que marucho se siente para servir



/* ATENCION: LA LIBERACION DE ESTE SEMAFORO LA DEBERIA HACER MARUCHO, SOLO ESTOY PROBADNO QUE ANDE.CUANDO HAGAMOS LA FUNCION MARUCHO, BORRAR LA SIGUIENTE LINEA Y METERLA EN MARUCHO */
        sem_post(&maruchoSentado);



        for(int i = 0; i<m; i++){                       //Instancio los mozos y los mando a hacer su funcion "principal"
                mozos_id[i] = i;                        //Conforme se van asignando threads, voy guardando sus ID
                pthread_create(&mozos[i], NULL, *mozo, &mozos_id[i]); //Inicializo los mozos, los mando a hacer su funcion, y como parametro le paso a cada uno su ID
        }

        for(int i = 0; i<m; i++){                       //Mato a los mozos
                pthread_join(mozos[i], NULL);
        }

        printf("Todos los threads deberian haber acabado.\n");

        return 0;
}

/*
void* ejemplo(void* arg){

        //Esta seria la funcion de ejemplo, que deberia hacer llamadas a otras funciones y a semaforos.

        int mozo_id = *(int*)arg;                       //Busco el ID de cada thread
        printf("Thread ID: %d creado! \n", mozo_id);    //Imprimo el ID de cada thread
        pthread_exit(NULL);                             //Thread termina su laburo
}
*/