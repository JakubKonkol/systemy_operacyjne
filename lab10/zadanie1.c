#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    FILE *file = fopen("text.txt", "r");
    if(fopen("text.txt", "r") == NULL) return -1;

    printf("Plik istnieje i zostal odczytany\n");

    return 0;
}