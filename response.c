#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include "response.h"
#include "helper.h"
#include "request.h"

#define BUFFER_SIZE 1024
#define PATH_SIZE 32
#define EXTENSION_SIZE 5
#define METHOD_SIZE 4
#define VERSION_SIZE 10 // size version for HTTP
int print_val = 0;

void http_response(const char *path_file, int accepted_socket) {
    // Use for the filesystem path
    char real_path[PATH_SIZE + 2];
    snprintf(real_path, sizeof(real_path), ".%s", path_file);

    if (strncmp(path_file, "/static", 7) == 0) {
        static_path(accepted_socket, path_file, real_path);
        return;
    }

    if (strncmp(path_file, "/cal", 4) == 0) {
        cal_path(accepted_socket, path_file);
        return;
    }

    if (strncmp(path_file, "/sleep/", 4) == 0) {
        sec_path(accepted_socket, path_file);
        return;
    }
    not_found(accepted_socket);
    return;
}

void* handle_connection(int* client_socket) {
    int accepted_socket = *client_socket;
    free(client_socket);
    printf("Accepted socket %d\n", accepted_socket);

    if (accepted_socket < 0) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }
    printf("Client connected.\n");

    // Receive data from the client
    char buffer[BUFFER_SIZE];
    int bytes_total;
    int bytes_received;
    bool keep_alive = true;
    while (keep_alive) {
        bytes_received = read(accepted_socket, buffer, sizeof(buffer));
        if(bytes_received < 0) {
            break;
        }
        //printf("Received %d bytes\n", bytes_received);
        bytes_total += bytes_received;
        buffer[bytes_total] = '\0';

        char *header_end;
        while ((header_end = strstr(buffer, "\r\n\r\n")) != NULL) {
            int length_req = (header_end - buffer) + 4;
            char method[METHOD_SIZE], path[PATH_SIZE], version[VERSION_SIZE];
            int parsed = sscanf(buffer, "%4s %31s %15s", method , path, version);

            //Get method
            if (parsed == 3 && strcmp(method, "GET") == 0) {
                http_response(path, accepted_socket);
            }
            //Ignore other methods
            else {
                keep_alive = false;
                break;
            }

            if (strcasestr(buffer, "Connection: close")) {
                keep_alive = false;
            }

            int leftover_data = bytes_total - length_req;
            if (leftover_data > 0) {
                memmove(buffer, buffer + length_req, leftover_data);
            }
            bytes_total = leftover_data;
            buffer[bytes_total] = '\0';
        }
    }

    printf("Client disconnected.\n");
    close(accepted_socket);
    return NULL;
}
