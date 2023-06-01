#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#define NUM_THREADS 2

int distance[10];
int is_horizontal_green = 0;
int laneUpPID;
int laneDownPID;
int laneRightPID;
int laneLeftPID;
int carIndex = 0;
static volatile int isAccident = 1;
int car_under_4_vertical = 1;
int car_under_4_horizontal = 1;

//Funkcje do zarzadzania otrzymanym sygnalem
void handleSingal(int sig)
{
    if (sig == SIGUSR1)
    {
        for(int i = 0; i < carIndex; i++)
        {
            if(distance[i] == 10)
            {
                printf("Otrzymano sygnal SIGUSR1");
                kill(getppid(), SIGINT);
                sleep(2);
            }
        }
        distance[carIndex] = 10;
        carIndex++;
    }
}

void handleSignalSigint(int sig){
    if (sig == SIGINT) isAccident = 0;
}

void handleSignalSigusr1(int sig){
    if (sig == SIGUSR1)
    {
        car_under_4_vertical = 0;
    }
}

void handleSignalSigusr2(int sig){
    if (sig == SIGUSR2)
    {
        car_under_4_horizontal = 0;
    }
}
// koniec funkcji zarzadzajacymi sygnalem.
void *lights_change_thread(void *arg){
    while(1) 
    {
        is_horizontal_green = 1;
        printf("Swiatlo na drodze poziomej jest zielone, na pionowej czerwone\n");
        fflush(stdout);
        sleep(3);
        is_horizontal_green = 0;
        printf("Swiatlo na drodze poziomej jest czerwone, na pionowej zielone\n");
        fflush(stdout);
        sleep(3);
    }

}

void *vertical_car_traffic_thread(void *arg){
    while(1) 
    {
        for(int i = 0; i < carIndex; i++)
        {
            sleep(1);
            if (distance[i] <= 0)
            {
                continue;
            }   
            else if(is_horizontal_green && distance[i] <= 5)
            {
                if(distance[i] <= 3 && distance[i] > 0) {
                    kill(getppid(), SIGUSR1);
                }

                printf("Zatrzymanie aut na drodze pionowej\n");
                break;
            }
            else if(!is_horizontal_green) 
            {
                distance[i]--;
                printf("Zmniejszanie pozycji auta o indeksie %d i wartosci %d na drodze pionowej, "
                "poniewaz swiatlo na tej drodze jest zielone\n", i, distance[i]);
            }
            else if(is_horizontal_green && distance[i] > 5)
            {
                printf("Zmniejszanie pozycji auta o indeksie %d i wartosci %d na drodze pionowej, "
                "poniewaz swiatlo na tej drodze jest czerwone, ale zadne auto nie ma pozycji mniejszej niz 5\n", i, distance[i]);
                distance[i]--;
            }
        }    
    }
}

void *horizontal_car_traffic_thread(void *arg){
    while(1) 
    {

        for(int i = 0; i < carIndex; i++)
        {
            sleep(1);
            if (distance[i] <= 0)
            {
                continue;
            }  
            else if(!is_horizontal_green && distance[i] <= 5)
            {
                if(distance[i] <= 3 && distance[i] > 0)
                {
                    kill(getppid(), SIGUSR2);
                } 
                
                printf("Zatrzymanie aut na drodze poziomej\n");
                break;
            }
            else if(is_horizontal_green) 
            {
                    distance[i]--;
                    printf("Zmniejszanie pozycji auta o indeksie %d i wartosci %d na drodze poziomej, " 
                    "poniewaz swiatlo na tej drodze jest zielone\n", i, distance[i]);
            } 
            else if(!is_horizontal_green && distance[i] > 5)
            {
                    distance[i]--;
                    printf("Zmniejszanie pozycji auta o indeksie %d i wartosci %d na drodze poziomej, "
                    "poniewaz swiatlo na tej drodze jest czerwone, ale zadne auto nie ma pozycji mniejszej niz 5\n", i, distance[i]);
            }
        }
    }
}

void vertical_traffic(){
    laneUpPID = fork();

    if (laneUpPID == 0)
    {
        /* Child A code */
        signal(SIGUSR1, handleSingal);

        laneUpPID = getpid();

        for(int i = 0; i < 10; i++)
        {
            distance[i] = 0;
        }

        pthread_t threads_ul[NUM_THREADS];

        // Tworzenie watkow
        pthread_create(&threads_ul[0], NULL, lights_change_thread, NULL);
        pthread_create(&threads_ul[1], NULL, vertical_car_traffic_thread, NULL);

        while (1)
        {
            sleep(1); // Proces potomny skończył swoje zadanie, czekamy na sygnał SIGUSR1
        }
    } 
    else 
    {
        laneDownPID = fork();

        if (laneDownPID == 0)
        {
            /* Child B code */
            signal(SIGUSR1, handleSingal);

            laneDownPID = getpid();

            for(int i = 0; i < 10; i++) 
            {
                distance[i] = 0;
            }

            pthread_t threads_ll[NUM_THREADS];

            // Tworzenie watkow
            pthread_create(&threads_ll[0], NULL, lights_change_thread, NULL);
            pthread_create(&threads_ll[1], NULL, vertical_car_traffic_thread, NULL);

            while (1)
            {
                sleep(1); // Proces potomny skończył swoje zadanie, czekamy na sygnał SIGUSR1
            }
        } 
    }
}

void horizontal_traffic(){
    // pid_t left_lane, right_lane;

    laneLeftPID = fork();

    if (laneLeftPID == 0)
    {
        /* Child A code */
        signal(SIGUSR1, handleSingal);

        laneLeftPID = getpid();
        for(int i = 0; i < 10; i++) 
        {
            distance[i] = 0;
        }

        pthread_t threads_lftl[NUM_THREADS];

        // Tworzenie watkow
        pthread_create(&threads_lftl[0], NULL, lights_change_thread, NULL);
        pthread_create(&threads_lftl[1], NULL, horizontal_car_traffic_thread, NULL);

        while (1)
        {
            sleep(1); // Proces potomny skończył swoje zadanie, czekamy na sygnał SIGUSR1
        }
    } 
    else 
    {
        laneRightPID = fork();

        if (laneRightPID == 0)
        {
            /* Child B code */
            signal(SIGUSR1, handleSingal);

            laneRightPID = getpid();

            for(int i = 0; i < 10; i++) 
            {
                distance[i] = 0;
            }

            pthread_t threads_rl[NUM_THREADS];

            // Tworzenie watkow
            pthread_create(&threads_rl[0], NULL, lights_change_thread, NULL);
            pthread_create(&threads_rl[1], NULL, horizontal_car_traffic_thread, NULL);

            while (1)
            {
                sleep(1); // Proces potomny skończył swoje zadanie, czekamy na sygnał SIGUSR1
            }
        } 
    }
}


int main(){
    signal(SIGINT, handleSignalSigint);
    signal(SIGUSR1, handleSignalSigusr1);
    signal(SIGUSR2, handleSignalSigusr2);
    vertical_traffic();
    horizontal_traffic();    

    int child_pids[4] = {laneUpPID, laneDownPID, laneLeftPID, laneRightPID};
    srand(time(NULL)); // Inicjalizacja generatora liczb losowych

    while (isAccident) {
        int random_child = rand() % 4; // Wybór losowego procesu potomnego
        printf("Wysylanie sygnalu SIGUSR1 do procesu potomnego o PID: %d\n", child_pids[random_child]);
        kill(child_pids[random_child], SIGUSR1); // Wysłanie sygnału SIGUSR1
        fflush(stdout);
        sleep(10); // Oczekiwanie przed kolejnym wysłaniem sygnału

        if(!car_under_4_horizontal && !car_under_4_vertical) {
            printf("Wypadek, na obu pasach znajduja sie auta o pozycji mnijeszej niz 4\n");
            break;
        }

        car_under_4_vertical = 1;
        car_under_4_horizontal = 1;
    }

    //zabicie procesow przed zakonczeniem pracy programu
    kill(laneDownPID, SIGKILL);
    kill(laneUpPID, SIGKILL);
    kill(laneLeftPID, SIGKILL);
    kill(laneRightPID, SIGKILL);

    return 0;
}