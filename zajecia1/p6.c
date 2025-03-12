#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Nie podałeś nazw pliku wejściowego oraz wyjściowego\n");
        return 1;
    }

   
    int input_desc = open(argv[1], O_RDONLY);
    if (input_desc == -1) {
        perror("Błąd otwierania pliku wejściowego");
        return 1;
    }

    
    int output_desc = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_desc == -1) {
        perror("Błąd otwierania pliku wyjściowego");
        close(input_desc);
        return 1;
    }

    char buffer[10];  
    ssize_t bytes_read;
    bytes_read = read(input_desc, buffer, sizeof(buffer));


    while (bytes_read  > 0) {
        ssize_t bytes_written_total = 0;

        
        while (bytes_written_total < bytes_read) {
            ssize_t bytes_written = write(output_desc, buffer + bytes_written_total, bytes_read - bytes_written_total);
            if (bytes_written == -1) {
                perror("Błąd zapisu do pliku");
                close(input_desc);
                close(output_desc);
                return 1;
            }
            bytes_written_total += bytes_written;
        }
    }



    if (bytes_read == -1) {
        perror("Błąd odczytu pliku");
    }

    close(input_desc);
    close(output_desc);

    return 0;
}
