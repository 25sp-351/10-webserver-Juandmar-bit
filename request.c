#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "response.h"
#include "helper.h"

#define HEADER_SIZE 1024
#define BUFFER_SIZE 40
#define OPT_L 4

void cal_path(const int accepted_socket, const char *path_file) {
    char header[HEADER_SIZE];
    char buffer[BUFFER_SIZE];
    int header_length;
    char op[OPT_L];
    int first, second, result;

    if (sscanf(path_file, "/cal/%3[^/]/%d/%d", op, &first, &second) == 3){
        result = cal(op, first, second);
        int body_length = snprintf(buffer, sizeof buffer, "%d\n", result);
        header_length = snprintf(header, sizeof header,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %d\r\n"
                "\r\n",
                body_length);
        write(accepted_socket, header, header_length);
        write(accepted_socket, buffer, body_length);
    }
}

void static_path(const int accepted_socket, const char *path_file, const char *real_path) {
    char header[HEADER_SIZE];
    char buffer[BUFFER_SIZE];
    int header_length;
    const char *extension = strrchr(path_file, '.');
    const char *content_type = get_mime_type(extension);
    
    if(extension == NULL ) {
        return;
    }
    
    int file_fd = open(real_path, O_RDONLY);
    if (file_fd < 0) {
        return;
    }   
    //Get file size
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    off_t file_size = file_stat.st_size;
    
    header_length = snprintf(header, sizeof header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n",
        content_type, (long)file_size);
            
        write(accepted_socket, header, header_length);
    
        //Copy file to response buffer
        ssize_t bytes_read;
        while((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
            write(accepted_socket, buffer, bytes_read);
        }  
    close(file_fd);
}

void sec_path (const int accepted_socket, const char *path_file) {
    char header[HEADER_SIZE];
    char buffer[BUFFER_SIZE];
    int header_length;
    int seconds;

    if (sscanf(path_file, "/sec/%d", &seconds) != 1) {
        return;
    }
    sleep(seconds);

    int body_length = snprintf(buffer, sizeof buffer, "Waited %d seconds%s\n", seconds, seconds == 1 ? "" : "s");

    header_length = snprintf(header, sizeof header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %d\r\n"
        "\r\n",
        body_length);
    write(accepted_socket, header, header_length);
    write(accepted_socket, buffer, body_length);
}