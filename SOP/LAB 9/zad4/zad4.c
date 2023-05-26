#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

void childProcess(int processNumber) {
    while (1) {
        printf("Proces potomny %d - ID: %d, ID procesu nadrzędnego: %d\n",
               processNumber, getpid(), getppid());
        usleep(500000); // Odczekaj 0.5 sekundy
    }
}

int main() {
    pid_t pid1, pid2;
    int status;
    srand(time(NULL));

    pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Błąd podczas tworzenia pierwszego procesu potomnego\n");
        return 1;
    } else if (pid1 == 0) {
        // Pierwszy proces potomny
        childProcess(1);
        exit(0);
    }

    pid2 = fork();

    if (pid2 < 0) {
        fprintf(stderr, "Błąd podczas tworzenia drugiego procesu potomnego\n");
        return 1;
    } else if (pid2 == 0) {
        // Drugi proces potomny
        childProcess(2);
        exit(0);
    }

    // Proces nadrzędny
    int random_number;
    while (1) {
        random_number = rand() % 50 + 1;
        printf("Proces nadrzędny - ID: %d, Wylosowana liczba: %d\n", getpid(), random_number);
        if (random_number < 10) {
            kill(pid1, SIGKILL);
            break;
        } else if (random_number > 40) {
            kill(pid2, SIGKILL);
            break;
        }
        usleep(500000); // Odczekaj 0.5 sekundy
    }

    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    return 0;
}
