# include <stdio.h>

int main(){

        //Operaciones aritmeticas random
        int i;
        while (i< 2147483647){
                i++;
        }

        //Operaciones de I/O con aritmetica
        printf("Ingresa un numero del 1 al 4 para seleccionar la operacion:\n 1: Sumar\n 2: Restar\n 3: Multiplicar\n 4: Dividir \n Operacion: ");

        int operacion; int primer; int segundo; int result; int resto;

        scanf("%d", &operacion);

        switch (operacion){
                case 1:
                        printf("Ingresa el primer sumando: ");
                        scanf("%d", &primer);
                        printf("Ingresa el segundo sumando: %d + ", primer);
                        scanf("%d", &segundo);

                        result = primer + segundo;
                        printf("El resultado es: %d + % d = %d \n", primer, segundo, result);
                        break;
                case 2:
                        printf("Ingresa el minuendo: ");
                        scanf("%d", &primer);
                        printf("Ingresa el sustraendo: %d - ", primer);
                        scanf("%d", &segundo);

                        result = primer - segundo;
                        printf("Diferencia: %d - % d = %d \n", primer, segundo, result);
                        break;
                case 3:
                        printf("Ingresa el multiplicando: ");
                        scanf("%d", &primer);
                        printf("Ingresa el multiplicador: %d * ", primer);
                        scanf("%d", &segundo);

                        result = primer * segundo;
                        printf("El producto es: %d * % d = %d \n", primer, segundo, result);
                        break;
                case 4:
                        printf("Ingresa el dividendo: ");
                        scanf("%d", &primer);
                        printf("Ingresa el divisor: %d / ", primer);
                        scanf("%d", &segundo);

                        if(segundo == 0){
                                printf("No dividimos por 0\n");
                                break;
                        }

                        result = primer / segundo;
                        resto = primer % segundo;
                        printf("Cociente, resto: %d / %d = %d,%d  \n", primer, segundo, result, resto);
                        break;
                default:
                        printf("Operacion invalida.");
                        break;
        }

        return 0;
}