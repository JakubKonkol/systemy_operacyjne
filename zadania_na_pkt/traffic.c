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
int isVerticalCar = 1;
int isHorizontalCar = 1;
static volatile int isAccident = 1; //flaga?

//Funkcje do obslugi otrzymanego sygnalu
void handleSingal(int sig)
{
    if (sig == SIGUSR1)
    {
        for(int i = 0; i < carIndex; i++)
        {
            if(distance[i] == 10)
            {
                printf("Koncze proces..");
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
    if (sig == SIGUSR1) isVerticalCar = 0;
}

void handleSignalSigusr2(int sig){
    if (sig == SIGUSR2) isHorizontalCar = 0;
}
void *changeLightTread(void *arg){ //Proces do obslugi zmiany swiatel
    while(1){
        is_horizontal_green = 1;
        printf("vertical:green, horizontal: red\n");
        sleep(3);
        is_horizontal_green = 0;
        printf("vertical:red, horizontal: green\n");
        sleep(3);
    }
}

void *verticalTread(void *arg){ //WATEK DO PASA RUCHU (PIONOWEGO)
    while(1){
        for(int i = 0; i < carIndex; i++){
            sleep(1);
            if (distance[i] <= 0){
                continue;
            }else if(is_horizontal_green && distance[i] <= 5){
                if(distance[i] <= 3 && distance[i] > 0) kill(getppid(), SIGUSR1);
                break; //zatrzymanie aut
            }else if(!is_horizontal_green){
                distance[i]--;
                printf("auto na drodze pionowej porusza sie\n");
            }else if(is_horizontal_green && distance[i] > 5){
                distance[i]--;
                printf("auto na drodze pionowej porusza sie\n");
            }
        }    
    }
}

void *horizontalTread(void *arg){ //WATEK DO PASA RUCHU (POZIOMEGO)
    while(1){
        for(int i = 0; i < carIndex; i++){
            sleep(1);
            if (distance[i] <= 0){
                continue;
            }  
            else if(!is_horizontal_green && distance[i] <= 5){
                if(distance[i] <= 3 && distance[i] > 0) //dystans (0;3> )
                {
                    kill(getppid(), SIGUSR2); //zabijam proces czyli zatrzymuje auta
                } 
                
                printf("Zatrzymuje auta\n");
                break;
            }
            else if(is_horizontal_green){
                    distance[i]--; //zmiejszam dystans 
                    printf("auto na drodze poziomej porusza sie\n");
            } 
            else if(!is_horizontal_green && distance[i] > 5){
                    distance[i]--;
                    printf("Auto na drodze poziomej porusza sie\n");
            }
        }
    }
}
// 2 procesy do poziomego i pionowego pasa ruchu, w nich tworzone jeszcze po dwa do gory i dolu
void vertical_traffic(){
    laneUpPID = fork(); 
    if (laneUpPID == 0){
        signal(SIGUSR1, handleSingal);
        laneUpPID = getpid();
        for(int i = 0; i < 10; i++){
            distance[i] = 0; //Ustawiam dystans na zero 
        }
        pthread_t threads_ul[NUM_THREADS]; 
        pthread_create(&threads_ul[0], NULL, changeLightTread, NULL); //Tworze watek
        pthread_create(&threads_ul[1], NULL, verticalTread, NULL); //Tworze watek

        while (1){
            sleep(1);
        }
    }else{
        laneDownPID = fork();
        if (laneDownPID == 0){
            signal(SIGUSR1, handleSingal);
            laneDownPID = getpid();
            for(int i = 0; i < 10; i++){
                distance[i] = 0;
            }
            pthread_t threads_ll[NUM_THREADS];
            pthread_create(&threads_ll[0], NULL, changeLightTread, NULL); //Tworze watek
            pthread_create(&threads_ll[1], NULL, verticalTread, NULL); //Tworze watek
            while (1){
                sleep(1); 
            }
        } 
    }
}

void horizontal_traffic(){
    laneLeftPID = fork();
    if (laneLeftPID == 0){
        signal(SIGUSR1, handleSingal);
        laneLeftPID = getpid();
        for(int i = 0; i < 10; i++){
            distance[i] = 0;
        }
        pthread_t threads_lftl[NUM_THREADS];
        pthread_create(&threads_lftl[0], NULL, changeLightTread, NULL); //Tworze watek
        pthread_create(&threads_lftl[1], NULL, horizontalTread, NULL); //Tworze watek

        while (1){
            sleep(1); 
        }
    }else {
        laneRightPID = fork();
        if (laneRightPID == 0){
            signal(SIGUSR1, handleSingal);
            laneRightPID = getpid();
            for(int i = 0; i < 10; i++){
                distance[i] = 0;
            }
            pthread_t threads_rl[NUM_THREADS];
            pthread_create(&threads_rl[0], NULL, changeLightTread, NULL); //Tworze watek
            pthread_create(&threads_rl[1], NULL, horizontalTread, NULL); //Tworze watek

            while (1){
                sleep(1); 
            }
        } 
    }
}

int main(){
    //Ustawiam obsluge sygnalow
    signal(SIGINT, handleSignalSigint);
    signal(SIGUSR1, handleSignalSigusr1);
    signal(SIGUSR2, handleSignalSigusr2);
    //uruchamiam ruch na pasach
    vertical_traffic();
    horizontal_traffic();    
    //tablica PID do procesow.
    int child_pids[4] = {laneUpPID, laneDownPID, laneLeftPID, laneRightPID};
    srand(time(NULL));
    while (isAccident) {
        int random_child = rand() % 4; // Wybieram losowy proces potomny
        kill(child_pids[random_child], SIGUSR1); // Zabijam proces
        sleep(10); //Czekam 10 sekund, zeby bledu nie bylo ? XD
        if(!isHorizontalCar && !isVerticalCar) {
            printf("WYPADEK!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"); //jesli jest wypadek to mamy wypadek :)
            break;
        }
        isVerticalCar = 1;
        isHorizontalCar = 1;
    }
    //zabicie procesow przed zakonczeniem pracy programu
    kill(laneDownPID, SIGKILL);
    kill(laneUpPID, SIGKILL);
    kill(laneLeftPID, SIGKILL);
    kill(laneRightPID, SIGKILL);
    return 0;
}