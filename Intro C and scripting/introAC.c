#include <stdio.h>

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

/*
Demostracion, ejecutar este programa como:
$ ./introAC Hello World
*/
	printf("Numero de argumentos: %d\n", argc);

    	for (int i = 0; i < argc; i++) {
        	printf("Argumento numero %d: %s\n", i, argv[i]);
    	}

    	return 0;
}