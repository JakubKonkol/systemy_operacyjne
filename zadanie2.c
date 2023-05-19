#include <stdio.h>
#include <stdlib.h>
int m, n, k= 0;
int main(){

    printf("Podaj liczbe n: ");
    scanf("%d", &n);
    printf("Podaj liczbe m ");
    scanf("%d", &m);
    int arrN[n];
    int arrM[m];
    for(int i = 0; i<n; i++){
        printf("tablica N: ");
        scanf("%d", &arrN[i]);
    }
    for(int i= 0; i<m; i++){
        printf("tablica M: ");
        scanf("%d", &arrM[i]);
    }
    int *c = (int*)malloc(n* sizeof(int));
    for(int i = 0; i<n; i++){
        for(int j = 0; j<m; j++){
            if(arrN[i] == arrM[j]){
                c[k++] = arrN[i];
            }
        }
    }
    printf("czesc wspolna: ");
    for(int i = 0; i<k; i++){
        printf("%d \n", c[i]);
    }


    return 0;
}