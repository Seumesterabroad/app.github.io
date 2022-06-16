#ifndef GET_PAGE_H
#define GET_PAGE_H

int socket_connect();
void socket_close(const int fd);
int connexion(int *fd, char** username, char** password);
void send_image(int *fd, char **username, char **path, char **name);
char* parseur_str(char* str);
int get_image(int *fd, char **username, char **name_im);
void select_file (int *fd,char **path_im);


#endif
