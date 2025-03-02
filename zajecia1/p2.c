#include <stdio.h>
#define MAX_LENGTH 50


void drukuj(int *tablica, int liczba_elementow) {
    printf("Funkcja drukuj: ");
    for (int i = 0; i < liczba_elementow; i++, tablica++) { 
        if (*tablica > 10 && *tablica < 100) {
            printf("%d ", *tablica);
        }
    }
    printf("\n");
}



int main() {
    int liczby[MAX_LENGTH];
    int counter = 0;
    printf("Podaj liczby ktore beda umieszczone w tablicy: ");
    while (counter < MAX_LENGTH) {
        scanf("%d", &liczby[counter]);

        if (liczby[counter] == 0) {
            printf("Przerywam wpisywanie liczb do tablicy.\n");
            break;
        }
        counter++;
    }

    drukuj(liczby, counter);
    return 0;
}
