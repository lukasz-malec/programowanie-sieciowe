#include <stdio.h>
#include <stdbool.h>


bool is_printable_str(const char *str) {
    while (*str) { 
        if (*str < 32 || *str > 126) {
            return false;
        }
        str++;
    }
    return true;
}

int main() {
    const char *test1 = "Dobry tekst";
    const char *test2 = "\n";
    const char *test3 = "znaki \t";
    
    bool wynik1 = is_printable_str(test1);
    bool wynik2 = is_printable_str(test2);
    bool wynik3 = is_printable_str(test3);

    printf("Test 1: %d\n", wynik1);
    printf("Test 2: %d\n", wynik2);
    printf("Test 3: %d\n", wynik3);
    
    return 0;
}