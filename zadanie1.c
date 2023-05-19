#include <stdio.h>
int n;
int sum = 0;
int m = 0;
int main(){
    printf("Podaj liczbe n: ");
    scanf("%d", &n);
    while(sum<=n){
        m++;
        sum+=m;
    }
    printf("wartosc m: %d", m-1);
    printf("\n");


    return 0;
}