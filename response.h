#ifndef RESPONSE_H
#define RESPONSE_H

#define DEFAULT_PORT 80
#define LISTEN_BACKLOG 5
extern int print_val;

void* handle_connection(int* client_socket);
void http_response(const char *path_file, int accepted_socket);

#endif // ECHO_H