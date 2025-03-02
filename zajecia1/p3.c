#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LENGTH 30


bool is_printable_buf(const void *buf, int len) {
    const char *buf_char = (const char *) buf; 

    for (int i = 0; i < len; i++, buf_char++) {
        if (*buf_char < 32 || *buf_char > 126) { 
            return false;
        }
    }
    return true; 
}



int main() {
    const char test1[MAX_LENGTH] = "Dobry tekst";
    const char test2[MAX_LENGTH] = "\n";
    const char test3[MAX_LENGTH] = "znaki \t";
    
    bool wynik1 =  is_printable_buf(test1, strlen(test1));
    bool wynik2 =  is_printable_buf(test2, strlen(test2));
    bool wynik3 =  is_printable_buf(test3, strlen(test3));

    printf("Test 1: %d\n",wynik1);
    printf("Test 2: %d\n",wynik2);
    printf("Test 3: %d\n",wynik3);
    



    return 0;
}

