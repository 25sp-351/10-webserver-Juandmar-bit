#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define HEADER_SIZE 1024

const char *get_mime_type(const char *extension) {
    if (strcmp(extension, ".html") == 0) return "text/html";
    if (strcmp(extension, ".css") == 0) return "text/css";
    if (strcmp(extension, ".js") == 0) return "application/javascript";
    if (strcmp(extension, ".png") == 0) return "image/png";
    if (strcmp(extension, ".jpg") == 0) return "image/jpg";
    if (strcmp(extension, "jpeg") == 0) return "image/jpeg";
    if (strcmp(extension, ".gif") == 0) return "image/gif";
    if (strcmp(extension, ".pdf") == 0) return "application/pdf";
    return NULL; 
}

int cal(const char *operand, const int first, const int second) {
    if (strcmp(operand, "add") == 0) return first + second;
    if (strcmp(operand, "mul") == 0) return first * second;
    if (strcmp(operand, "div") == 0) {
        if(second == 0) {
            return -1;
        }
        return first / second;
    }
    return -1;
}

void not_found (const int accepted_socket) {
    char header[HEADER_SIZE];
    int header_length;
    const char *nf = "404 Not Found";
    int nf_length = strlen(nf);
    header_length = snprintf(header, sizeof header,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n",
            nf_length);
        
    write(accepted_socket, header, header_length);
    write(accepted_socket, nf, nf_length);
}