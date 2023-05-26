#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main() {
    pid_t pid;
    int status;
    srand(time(NULL));
    pid = fork();
        // Proces potomny
        int random_number;
        while (1) {
            random_number = rand() % 50 + 1;
            printf("Proces potomny - ID: %d, ID procesu nadrzędnego: %d, Wylosowana liczba: %d\n",
                   getpid(), getppid(), random_number);
            if (random_number % 5 == 0)
                break;
        }
        exit(0);
        // Proces nadrzędny
        wait(&status);
        printf("Proces nadrzędny - ID: %d\n", getpid());
    return 0;
}





