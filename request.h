#ifndef REQUEST_H
#define REQUEST_H

void cal_path(const int accepted_socket, const char *path_file);
void static_path(const int accepted_socket, const char *path_file, const char *real_path);
void sec_path (const int accepted_socket, const char *path_file);

#endif