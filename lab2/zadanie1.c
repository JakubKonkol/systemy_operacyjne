#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    printf("Pierwszy napis\n");
    fork();
    wait(NULL);
    printf("drugi napis\n");
    fork();
    wait(NULL);
    printf("trzeci napis\n");

    return 0;
}