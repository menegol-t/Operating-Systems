# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <string.h>

int m = 3; //Cant de mozos
int n = 4; //Cant de invitados
int comenzalesSentados = 0; //Cant de comenzales sentados al inicio

//Semaforos a utilizar
sem_t manuchoSentado;
sem_t manuchoComiendo;
sem_t todosComenzalesSentados;
sem_t manuchoLanzoPregunta;
sem_t comenzalLanzaRespuesta;
sem_t mozosDisponibles;
sem_t unComensalLibre;

void servirComida(int id)
{
        sem_wait(&manuchoSentado);      //La primera vez se espera a que marucho se siente para empzar a servir.
        sem_post(&manuchoSentado);

        char comenzal[20];
        if(id == 0){
                strcpy(comenzal, "Manucho");    //El string comenzal vale "Manucho" si el Id que me llego es 0
        }else{
                snprintf(comenzal, sizeof(comenzal), "invitado %d", id);
        }

        sem_wait(&mozosDisponibles);    // Espera hasta que un mozo este disponible (disminuye el semaforo)
        printf("Mozo esta sirviendo a %s.\n", comenzal);
        sleep(3);                       // Simula el tiempo que el mozo tarda en servir al invitado
        sem_post(&mozosDisponibles);    // Una vez que se sirvio el plato libero al mozo.
        printf("%s esta comiendo.\n", comenzal);
        sleep(5);                       // Simulamos que el invitado esta comiendo
        printf("%s ha terminado de comer.\n", comenzal);

        if(id != 0){                    // Una vez el invitado termino de comer, lo libero para que haga la pregunta.
                sem_post(&unComensalLibre);
        }else{                          //Si el que termino de comer es marucho
                sem_post(&manuchoComiendo);
        }
}

void* levantarse(int id)
{
        sem_wait(&manuchoSentado);
        printf("Comensal %d se levanto de la mesa.\n", id);
        sem_post(&todosComenzalesSentados); //????
        sem_post(&manuchoSentado);
}

void* sentarse(int id)
{
        comenzalesSentados++;
        printf("Invitado %d se sento.\n", id);
        if(comenzalesSentados == n){
                sem_post(&todosComenzalesSentados);
        }
}

void* lanzar_pregunta()
{
        //Manucho es el unico que lanza pregunta
        sem_wait(&manuchoComiendo);     //Cuando termine de comer manucho
        printf("Manucho: Quien consideran que es el proximo campeon del mundo???\n");
        sem_post(&manuchoLanzoPregunta);
}

void* lanzar_respuesta(int id)
{
        sem_wait(&manuchoLanzoPregunta);//Si ya se hizo la pregunta
        sem_wait(&unComensalLibre);
        printf("Comenzal %d: Francia tiene potencial.\n", id); //Se deberia decir el id del comenzal que responde? quedaria mas bonito? aporta algo?
        sem_post(&comenzalLanzaRespuesta);
        sem_post(&unComensalLibre);
}

void* sentarse_manucho()
{
        sem_wait(&todosComenzalesSentados);     //Necesito saber si todos los comenzales se sentaron //Quiero ese recurso //Bueno se entiende
        printf("Manucho se sento.\n");
        sem_post(&manuchoSentado);
}

void* enojarse()
{
        //El unico que se enoja en nuestro problema es manucho Manucho se enoja cuando se lanza la respuesta
        sem_wait(&comenzalLanzaRespuesta);
        printf("Manucho: -Se enoja por la respuesta!-\n");
        printf("Manucho: -Se levanta de la mesa!-\n");
        sem_post(&manuchoSentado);      //Manucho se levanto de mesa!
}

//Comportamiento invitados.
void* invitados(void* arg)
{
        int invitado_id = *(int*)arg;   //Busco el ID de cada thread
        sentarse(invitado_id);
        servirComida(invitado_id);
        lanzar_respuesta(invitado_id);
        levantarse(invitado_id);
        pthread_exit(NULL);             //Thread termina su laburo
}

//Comportamiento de Manucho.
void* manucho(void* arg)
{
        sentarse_manucho();
        servirComida(0);
        lanzar_pregunta();
        enojarse();
        pthread_exit(NULL);
}

int main () {
        //Defino los threads
        pthread_t manuchot;

        pthread_t hilos_invitados[n];   //Array de mozos
        int id_invitados[n];            //Array que va a contener los ID de los threads de mozos

        //Inicializo semaforos y les defino su primer estado
        sem_init(&manuchoSentado, 0, 0);//Este semaforo se va a usar para que los mozos esperen a que marucho se siente para servir
        sem_init(&todosComenzalesSentados, 0 ,0);
        sem_init(&manuchoComiendo,0,0);
        sem_init(&manuchoLanzoPregunta,0,0);
        sem_init(&comenzalLanzaRespuesta,0,0);
        sem_init(&mozosDisponibles, 0, m);      //Mozos disponibles dado por M
        sem_init(&unComensalLibre,0,0);

        //Hilo para manucho
        pthread_create(&manuchot, NULL, *manucho, NULL);

        //Hilos para los invitados
        for (int i = 0; i < n; i++)     //Instancio los mozos y los mando a hacer su funcion "principal"
        {
                id_invitados[i] = i + 1;//Conforme se van asignando threads, voy armando sus IDs desde 1 a N
                pthread_create(&hilos_invitados[i], NULL, *invitados, &id_invitados[i]); //Inicializo los invitados, los mando a hacer su funcion, y como parametro le paso a cada uno su ID
        }

        for (int i = 0; i < n; i++)     //Mato a los invitados (Solo se da cuando salieron de la funcion principal "invitados")
        {
                pthread_join(hilos_invitados[i], NULL);
        }

        pthread_join(manuchot,NULL);    //Mato a manucho

        printf("Nos vemos! \n");

        return 0;
}