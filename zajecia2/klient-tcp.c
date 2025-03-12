// Prosty program klienta korzystajacy z TCP/IPv4 odbierajacy komunikat od serwera
// Program dzialacy na sytemaach Unix/Linux ( w tym macOS) ze względu na systemowe biblioteki 
// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <sys/socket.h>

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Podałeś złą liczbę argumeneów\n");
        return 1;
    }

    char *address = argv[1];
    int port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Nieprawidłowy numer portu: %d\n", port);
        return EXIT_FAILURE;
    }

    int sock;
    struct sockaddr_in addr;
    ssize_t cnt;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Błąd tworzenia gniazda");
        return EXIT_FAILURE;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, address, &addr.sin_addr) <= 0) {
        perror("Błąd w adresie IP");
        close(sock);
        return EXIT_FAILURE;
    }


    
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Błąd połączenia");
        close(sock);
        return EXIT_FAILURE;
    }

    unsigned char buf[16];
    

    cnt = read(sock, buf, sizeof(buf));
    if (cnt == -1) {
        perror("Błąd odczytu");
        close(sock);
        return EXIT_FAILURE;
    }


    for (int i = 0; i < cnt - 2; i++) {
        if (!isprint(buf[i])) {
            fprintf(stderr, "Odebrane znaki nie są drukowalne.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    if (cnt < 2 || buf[cnt - 2] != '\r' || buf[cnt - 1] != '\n') {
        fprintf(stderr, "Odebrano błędne dane.\n");
        exit(EXIT_FAILURE);
    }
    


    printf("%s\n", buf);

    if (close(sock) == -1) {
        perror("Błąd zamykania gniazda");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
