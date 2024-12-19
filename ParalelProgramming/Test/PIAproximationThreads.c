#include <stdio.h>
#include <omp.h>
static long pasos = 100000000;  //Cantidad de pasos para el calculo, cuanto mas alto, mas precision

/*
Paralelizar un programa sencillo para determinar el valor de pi.
Primero se determina en cuantos pasos se va a realizar el cálculo, cuanto mayor es este número, mayor precisión.

*/

int main(){
        //El valor en cada iteracion del ciclo
        int i;

        //Para almacenar el resultado
        double resultado = 0;

        //Para acortar la expresion "1/pasos". Pongo 1.0 en lugar de 1 porque C si no divide ints en vez de doubles y la division entre uno y lo que sea me da 0.
        double h = 1.0/pasos;

        #pragma omp parallel for reduction(+:resultado)
        for(int i = 0; i < pasos; i++){

        //Formula que me dieron para calcular pi. Cada thread va a ejecutar esta formula, y al final "resultado" sera la sumatoria de todos los resultados
                resultado += 4.0 / ( 1 + ( (i + 0.5) * h ) * ( (i + 0.5) * h ) ) ;
        }

        //Posterior a la sumatoria, resultado se multiplica por H.
        resultado = resultado * h;

        printf("Aproximacion de PI con %d pasos: %f\n", pasos, resultado);
        return 0;
}