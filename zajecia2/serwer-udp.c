// Prosty program serwera korzystajacy z UDP/IPv4 wysyłacyy komunikat do klienta
// Program dzialacy na sytemaach Unix/Linux ( w tym macOS) ze względu na systemowe biblioteki 
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <sys/socket.h>

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Podałeś złą liczbę argumentów\n");
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Nieprawidłowy numer portu: %d\n", port);
        return EXIT_FAILURE;
    }

    int sock;
    struct sockaddr_in server_addr, client_addr;
    char *text = "Hello from UDP!\r\n";
    int rc;

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
        socklen_t client_len = sizeof(client_addr);
        char buf[1]; // pusty datagram
        ssize_t recv_len;

        // Odbiór pustego datagramu od klienta
        recv_len = recvfrom(sock, buf, sizeof(buf), 0,
                            (struct sockaddr *)&client_addr, &client_len);
        if (recv_len == -1) {
            perror("Błąd odbierania");
            continue;
        }

        

        
        if (sendto(sock, text, strlen(text), 0,
                   (struct sockaddr *)&client_addr, client_len) == -1) {
            perror("Błąd wysyłania odpowiedzi");
        }
    }
    
    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }
    return 0;
}
