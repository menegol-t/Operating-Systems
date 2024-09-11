# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
int m = 3; //Cant de mozos
int n = 4; //Cant de invitados

//Semaforos a utilizar
sem_t manuchoSentado;
sem_t manuchoComiendo;
sem_t comenzalesSentados;
sem_t manuchoLanzoPregunta;
sem_t comenzalLanzaRespuesta;
sem_t mozosDisponibles;
sem_t unComensalLibre;

void servirComida(int id)
{ //id de invitado

        //METER A MANUCHO COMIENDO POR ACA;

        // Espera hasta que un mozo este disponible (disminuye el semaforo)
        sem_wait(&mozosDisponibles);
        // Simula el tiempo que el mozo tarda en servir al invitado
        printf("Mozo esta sirviendo al invitado \n");
        sleep(2); // El mozo sirve al invitado
         // El invitado comienza a comer
        printf("Invitado esta comiendo.\n");
        sleep(5); // Simulamos que el invitado esta comiendo
        // El mozo ha terminado de servir al invitado y queda disponible
        printf("Invitado ha terminado de comer.\n");
        sem_post(&unComensalLibre); //lo libero para que este disponible para responder
        sem_post(&mozosDisponibles); // El mozo queda disponible nuevamente
}

void* levantarse(int id)
{
        sem_wait(&manuchoSentado);
        printf("Comensal se levanto de la mesa.\n");
        sem_post(&comenzalesSentados);
        sem_post(&manuchoSentado);
}

void* sentarse(int id)
{
        sem_post(&comenzalesSentados);
        printf("Invitado se sento\n");
}

void* lanzar_pregunta()
{
        //Manucho es el unico que lanza pregunta
        sem_wait(&manuchoComiendo);     //Cuando termine de comer manucho
        printf("Manucho: Quien consideran que es el proximo campeon del mundo???\n");
        sem_post(&manuchoLanzoPregunta);
}

void* lanzar_respuesta(int comenzal_id)
{
        sem_wait(&manuchoLanzoPregunta);//Si ya se hizo la pregunta
        sem_wait(&unComensalLibre);
        //int comenzal= comenzal_id; //ATENCION:::::Esto me da violacion de segmento revisar despues de dormir
        printf("Comenzal: Francia tiene potencial.\n"); //Se deberia decir el id del comenzal que responde? quedaria mas bonito? aporta algo?
        sem_post(&comenzalLanzaRespuesta);
        sem_post(&unComensalLibre);
}

void* sentarse_manucho()
{
        sem_wait(&comenzalesSentados);  //Necesito saber si todos los comenzales se sentaron //Quiero ese recurso //Bueno se entiende
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

void* comer_manucho()
{ //ESTO esta mal debe comer cuando ya este atendido por un mozo, es solo para probar
        sem_wait(&manuchoSentado);
        printf("Manucho esta comiendo\n");
        sem_post(&manuchoComiendo);
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
void* manucho(void* par)
{
        sentarse_manucho();
        comer_manucho();                //Este no esta terminado porque aun no esta mozos ni cuando le sirven
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
        sem_init(&comenzalesSentados, 0 ,n);
        sem_init(&manuchoComiendo,0,0);
        sem_init(&manuchoLanzoPregunta,0,0);
        sem_init(&comenzalLanzaRespuesta,0,0);
        sem_init(&mozosDisponibles, 0, m);
        sem_init(&unComensalLibre,0,0);

        //Hilo para manucho
        pthread_create(&manuchot, NULL, *manucho, NULL);

        //Hilos para los invitados
        for (int i = 0; i < n; i++)     //Instancio los mozos y los mando a hacer su funcion "principal"
        {
                id_invitados[i] = i + 1;//Conforme se van asignando threads, voy guardando sus ID
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
