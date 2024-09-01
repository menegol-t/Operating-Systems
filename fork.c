#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
        int n = 3;int i;
        pid_t pid;
        printf("Fork: 0: proceso hijo, 0<: proceso padre\n");
        for(i = 0; i<n; i++){
                pid = fork();
                if(pid == 0 ){
                        printf("Soy hijo %d, mi padre es %d\n", getpid(), getppid());
                }else if(pid>0){
                        printf("Soy el padre %d, mi hijo es %d\n", getpid(), pid);
                }else{
                        printf("Error\n");
                }
        }
        return 0;
}