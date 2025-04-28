#ifndef HELPER_H
#define HELPER_H

const char *get_mime_type(const char *extension);
int cal(const char *operand, const int first, const int second);
void not_found (const int accepted_socket);
void cal_path(const int accepted_socket, const char *path_file);
void static_path(const int accepted_socket, const char *path_file, const char *real_path);
void sec_path (const int accepted_socket, const char *path_file);

#endif