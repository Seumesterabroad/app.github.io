#ifndef GET_PAGE_H
#define GET_PAGE_H

int socket_connect();
void socket_close(const int fd);
int connexion(int *fd, char** username, char** password);
void send_image(int* fd, char** username, char** path);
void get_images(int* fd, char** username);
int get_image(int* fd, char** username, int* num);

#endif
