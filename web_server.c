#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <pthread.h>
#include "response.h"

void sigint_handler(int sig) {
    printf("Sever shutting down...\n");
    exit(0);
}

int main (int argc, char *argv[]) {

    int port = DEFAULT_PORT;
    int operation;

    while((operation = getopt(argc, argv, ":p:")) != -1) {
        switch (operation){
            case 'p':
                if (optarg == NULL) {
                    break;
                }
                else {
                    sscanf(optarg, "%d", &port);
                }
                break;
            default:
                continue;
        }

    }
    if (port == DEFAULT_PORT) {}
    // Check if the port number is valid
    else if (port < 1024 || port > 65535) {
        fprintf(stderr, "Error: Invalid port number. Port must be between 1024 and 65535.\n");
        exit(EXIT_FAILURE);
    }

    printf("%d", port);

    //Create a socket with AF_INET: IPv4 and SOCK_STREAN: TCP. System choses the protocol 
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // define IPv4 family address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // initialize the socket structure to zero to clear any garbage values
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // bind to localhost
    server_addr.sin_port = htons(port); // convert port number to network byte order
    printf("Binding to port %d...\n", port); 

    printf("Press Ctrl+C to stop the server.\n");
    // Set up signal handler for SIGINT
    if (signal(SIGINT, sigint_handler)) {
        perror("Error setting up signal handler");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    int return_value = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (return_value < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    return_value = listen(socket_fd, LISTEN_BACKLOG); // listen for incoming connections

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Accept incoming connections
    while(true) {
        pthread_t thread;
        int* client_fd_buf = malloc(sizeof(int));

        *client_fd_buf = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);

        printf("Client connected.%d\n", *client_fd_buf);

        pthread_create(&thread, NULL, (void* (*) (void*))handle_connection, (void*)client_fd_buf);
    }
    return 0;
}