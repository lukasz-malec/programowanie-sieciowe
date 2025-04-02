#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_BUFFER 65507 


bool is_palindrome(const char *word) {
    int len = strlen(word);
    for (int i = 0; i < len / 2; i++) {
        if (tolower(word[i]) != tolower(word[len - 1 - i])) {
            return false; 
        }
    }
    return true; 
}


bool is_letter(char c){
    return  (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int check_for_Error(const char *message, int recv_len) { 

    int len = strlen(message);

    // Pusta wiadomość
    if (len == 0) {
        return 2;
    }

    // Na początku lub końcu jest spacja
    if (message[0] == ' ' || message[len - 1] == ' ') {
        return 1; 
    }

    for (int i = 0; i < recv_len-1; i++) {
        if (message[i] == '\0') { // Wykrywanie znaku NULL w środku wiadomości
            return 1;
        }
    }


    // Sprawdzanie znaków
    for (int i = 0; i < len; i++) {
        if (message[i] == '\0') { // Wykrywanie znaku NULL w środku wiadomości
            return 1;
        }
        if (!is_letter(message[i]) && message[i] != ' ') { // Akceptujemy tylko litery i spacje
            return 1;
        }
        if (i < len - 1 && message[i] == ' ' && message[i + 1] == ' ') { // Podwójna spacja
            return 1;  
        }
        if (message[i] == '\n' || message[i] == '\r') { // Nie dopuszczamy Enterów
            return 1; 
        }
    }

    return 0;  
}



void separate_words(char *sentence, char *response) {
    int word_count = 0, palindrome_count = 0;
    char *word;

    word = strtok(sentence, " "); // dzielenie na słowa
    while (word) {
        word_count++;
        if (is_palindrome(word)) {
            palindrome_count++;
        }
        word = strtok(NULL, " "); // przetwarzanie następnego
    }

    snprintf(response, MAX_BUFFER, "%d/%d", palindrome_count, word_count);

}


int main(int argc, char *argv[]) {
    int sock;
    int port = 2020;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_BUFFER], response[MAX_BUFFER];
    socklen_t client_len;
    ssize_t recv_len;

    // Tworzenie gniazda UDP
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Błąd tworzenia gniazda");
        return EXIT_FAILURE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // Powiązanie gniazda z adresem i portem
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Błąd bindowania gniazda");
        close(sock);
        return EXIT_FAILURE;
    }

    while (1) {
        client_len = sizeof(client_addr);
        memset(buffer, 0, MAX_BUFFER);

        // Odbiór zapytania od klienta
        recv_len = recvfrom(sock, buffer, MAX_BUFFER - 1, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len == -1) {
            perror("Błąd odbierania");
            continue;
        }

        buffer[recv_len] = '\0'; // Zakończenie stringa


        // Usunięcie \n lub \r\n na końcu
        if (buffer[recv_len - 1] == '\n') {
            buffer[recv_len - 1] = '\0';
            if (recv_len > 1 && buffer[recv_len - 2] == '\r') {
                buffer[recv_len - 2] = '\0';
            }
        }

        
            int check_result = check_for_Error(buffer,recv_len); // sprawdzanie błędów


            if (check_result == 1) {
                strcpy(response, "ERROR");
            } else if (check_result == 2) {
                strcpy(response, "0/0");
            } else {
                separate_words(buffer, response);  // obsluga
            }
    

        // Wysłanie odpowiedzi do klienta
        if (sendto(sock, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len) == -1) {
            perror("Błąd wysyłania odpowiedzi");
        }
    }
    
    
    int rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }
    return 0;
}
