#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
void onSIGINT(){
    printf("Jestem nieśmiertelny! PID=%d\n", getpid());
}
void onSIGHUP(){
    signal(SIGINT, SIG_IGN);
}

int main(){
    signal(SIGINT, onSIGINT);
    signal(SIGHUP, onSIGHUP);

    while(1==1){
        
    }
    return 0;
}