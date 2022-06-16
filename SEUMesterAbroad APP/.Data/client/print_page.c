//#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <sys/stat.h>

const size_t BUFFER_SIZE = 32;
char path_images[] = "./images/";
char extension_BMP[] = ".bmp";

void rewrite(int fd, const void *buf, size_t count)
{
    ssize_t written = 0;
    while (written < (ssize_t)count)
    {
        written = write(fd, buf, count);
        if (written == -1)
            err(1, "An error occurred");
    }
}

char *build_query(const char *host, size_t *len)
{
    char *req;
    int r = asprintf(&req, "GET http://www.%s/ HTTP/1.0\r\n\r\n", host);

    if (r == -1)
        err(1, "Request could not be generated");

    *len = r;
    return req;
}

int connexion(int *fd, char **username, char **password)
{
    //printf("Entered connexion\n");

    int u_len = strlen(*username);
    int p_len = strlen(*password);

    int k = 0;

    if (write(*fd, &k, sizeof(int)) == -1)
            err(1, "An error occurred");
    if (write(*fd, &u_len, sizeof(int)) == -1)
            err(1, "An error occurred");
    //printf("u_len sent OK\n");
    //printf("username = %s\n", *username);
    if (write(*fd, *username, u_len) == -1)
            err(1, "An error occurred");
    //printf("username sent OK\n");
    if (write(*fd, &p_len, sizeof(int)) == -1)
            err(1, "An error occurred");
    if (write(*fd, *password, p_len) == -1)
            err(1, "An error occurred");

    int ret = 0;

    if (read(*fd, &ret, 1) == -1)
            err(1, "An error occurred");

    return ret;
}

void send_image(int *fd, char **username, char **path, char **name)
{
    FILE *picture;
    picture = fopen(*path, "r");

    if (picture != NULL)
    {
        // Send code 1
        int k = 1;
        if (write(*fd, &k, sizeof(int)) == -1)
            err(1, "An error occurred");

        int len = strlen(*username);

        // Send username and its length
        if (write(*fd, &len, sizeof(int)) == -1)
            err(1, "An error occurred");
        if (write(*fd, *username, len) == -1)
            err(1, "An error occurred");

        int name_len = strlen(*name);

        if (write(*fd, &name_len, sizeof(int)) == -1)
            err(1, "An error occurred");
        if (write(*fd, *name, name_len) == -1)
            err(1, "An error occurred");
        // Get Picture Size
        printf("Getting Picture Size\n");
        int p_size;
        if (fseek(picture, 0, SEEK_END) != 0)
            err(1, "An error occurred");
        p_size = ftell(picture);
        printf("Size = %d\n", p_size);
        if (fseek(picture, 0, SEEK_SET) != 0)
            err(1, "An error occurred");

        // Send Picture Size
        printf("Sending Picture Size\n");
        if (write(*fd, &p_size, sizeof(int)) == -1)
            err(1, "An error occurred");

        // Send Picture as Byte Array
        printf("Sending Picture as Byte Array\n");

        char *pic_buffer = calloc(p_size, 1);
        int h = 0;
        while (h < p_size)
        {
            if (fread(pic_buffer, 1, p_size, picture) <= 0) // JE SUIS PAS SURE DE CETTE CONDITION
                err(1, "An error occurred");
            int g = write(*fd, pic_buffer, p_size);
            printf("bytes sent : %d\n", g);
            bzero(pic_buffer, p_size);
            h += g;
        }
    }

    int k = 5;
    if (write(*fd, &k, sizeof(int)) == -1)
        err(1, "An error occurred");
}

char* parseur_str(char* str)
{
    size_t slash = 0;
    size_t i = 0;
    while (*(str + slash + i) != '\0')
    {
        i++;
        if (*(str+slash+i) == '/')
        {
            slash += i;
            i=1;
        }
    }
    char* res = malloc(sizeof(char));
    for (size_t temp = 0; temp < i; temp ++)
    {
        *(res + temp) = *(str + slash + temp + 1);
    }
    return res;
}

int get_image(int *fd, char **username, char **name_im)
{
    // Send code 3
    int k = 3;
    if (write(*fd, &k, sizeof(int)) == -1)
        err(1, "An error occurred");

    int len = strlen(*username);

    // Send username and its length
    if (write(*fd, &len, sizeof(int)) == -1)
        err(1, "An error occurred");
    if (write(*fd, *username, strlen(*username)) == -1)
        err(1, "An error occurred");
    int len2 = strlen(*name_im);
    if (write(*fd, &len2, sizeof(int)) == -1)
        err(1, "An error occurred");
    if (write(*fd, *name_im, len2) == -1)
        err(1, "An error occurred");

    int result;

    if (read(*fd, &result, sizeof(int)) == -1)
        err(1, "An error occurred");

    if (result == 0)
    {
        return 1;
    }
    else
    {
        printf("Image Existe po !\n");
        return 0;
    }
}

void select_file (int *fd,char **path_im)
{
    int i_len;
    if (read(*fd, &i_len, sizeof(int)) == -1)
        err(1, "An error occurred");
    printf("Picture size : %d\n", i_len);

    FILE *image;

    image = fopen(*path_im, "wb");
    printf("\nConcat done\n");

    char *p_array = calloc(i_len, 1);
    int h = 0;
    while (h < i_len)
    {
        int g = read(*fd, p_array, i_len);
        if (g == -1)
            err(1, "An error occurred");
        printf("bytes read : %d\n", g);
        if (fwrite(p_array, 1, g, image) != (size_t) g)
            err(1, "An error occurred");
        h += g;
    }

    free(p_array);
    fclose(image);
}

int socket_connect()
{

    // Declare and initialize hints
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Get the linked list
    struct addrinfo *result;
    int err = getaddrinfo("seumesterabroad.cl0pinette.fr", "2048", &hints, &result);

    // File descriptor for the socket askip aussi
    int sfd;

    // Pointer used to iterate over the linked list askip
    struct addrinfo *p;
    for (p = result; p != NULL; p = p->ai_next)
    {
        // Try to create a socket
        sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sfd == -1)
            continue;
        // Try to connect socket
        err = connect(sfd, p->ai_addr, p->ai_addrlen);

        // If success, break loop, else close socket
        if (err != -1)
            break;
        else
            close(sfd);
    }

    // Free result (linked list)
    freeaddrinfo(result);

    // Handle errors
    if (!p)
        errx(EXIT_FAILURE, "Couldn't connect");

    return sfd;
}

void socket_close(int fd)
{
    close(fd);
}