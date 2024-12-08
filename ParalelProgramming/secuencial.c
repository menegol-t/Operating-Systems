#include <stdio.h>
int main(int argc, char **argv)
{
        int left, right;
        int number, sum; //usar number como indice del for

        left=1;
        right=1000;
	sum = 0;

        for(number = left; number<right; number++){
                sum = number + sum;
        }

        printf("Suma: %d\n", sum);
        return 0;
}