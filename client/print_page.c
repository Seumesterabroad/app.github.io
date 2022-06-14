#define _GNU_SOURCE

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
    printf("Entered connexion\n");

    int u_len = strlen(*username);
    int p_len = strlen(*password);

    int k = 0;

    write(*fd, &k, sizeof(int));
    write(*fd, &u_len, sizeof(int));
    printf("u_len sent OK\n");
    printf("username = %s\n", *username);
    write(*fd, *username, u_len);
    printf("username sent OK\n");
    write(*fd, &p_len, sizeof(int));
    write(*fd, *password, p_len);

    int ret = 0;

    read(*fd, &ret, 1);

    return ret;
}

void send_image(int *fd, char **username, char **path)
{
    // Send code 1
    int k = 1;
    write(*fd, &k, sizeof(int));

    int len = strlen(*username);

    // Send username and its length
    write(*fd, &len, sizeof(int));
    write(*fd, *username, strlen(*username));

    // Get Picture Size
    printf("Getting Picture Size\n");
    FILE *picture;
    picture = fopen(*path, "r");
    int p_size;
    fseek(picture, 0, SEEK_END);
    p_size = ftell(picture);
    printf("Size = %d\n", p_size);
    fseek(picture, 0, SEEK_SET);

    // Send Picture Size
    printf("Sending Picture Size\n");
    write(*fd, &p_size, sizeof(int));

    // Send Picture as Byte Array
    printf("Sending Picture as Byte Array\n");
    char pic_buffer[p_size];
    while (!feof(picture))
    {
        fread(pic_buffer, 1, sizeof(pic_buffer), picture);
        write(*fd, pic_buffer, sizeof(pic_buffer));
        bzero(pic_buffer, sizeof(pic_buffer));
    }
}

void get_images(int *fd, char **username)
{
    // setup ./images folder
    struct stat st;

    if (stat(path_images, &st) == -1)
    {
        mkdir(path_images, 0777);
    }

    // Send code 2
    int k = 2;
    write(*fd, &k, sizeof(int));

    int len = strlen(*username);

    // Send username and its length
    write(*fd, &len, sizeof(int));
    write(*fd, *username, strlen(*username));

    int images;

    read(*fd, &images, sizeof(int));
    for (int i = 1; i <= images; i++)
    {
        int i_len;
        read(*fd, &i_len, sizeof(int));
        printf("Picture size : %d\n", i_len);

        char *p_array = calloc(i_len, 1);
        read(*fd, p_array, i_len);

        FILE *image;

        char *path = calloc(10 + (i / 10) + 2 + 4, 1);
        if (!path)
            errx(EXIT_FAILURE, "calloc()");
        strcat(path, path_images);
        char *number = calloc((i / 10) + 2, 1);
        sprintf(number, "%d", i);
        strcat(path, number);
        strcat(path, extension_BMP);

        image = fopen(path, "wb+");
        printf("\nConcat done\n");
        printf("%d\n", i);
        printf("%s\n", path);
        fwrite(p_array, 1, i_len, image);

        free(path);
        free(number);
        free(p_array);
        fclose(image);
    }
}

void get_image(int *fd, char **username, int *num)
{
    // setup ./images folder
    struct stat st;

    if (stat(path_images, &st) == -1)
    {
        mkdir(path_images, 0777);
    }

    // Send code 3
    int k = 3;
    write(*fd, &k, sizeof(int));

    int len = strlen(*username);

    // Send username and its length
    write(*fd, &len, sizeof(int));
    write(*fd, *username, strlen(*username));

    write(*fd, num, sizeof(int));

    int result;

    read(*fd, &result, sizeof(int));

    if (result == 0)
    {
        int i_len;
        read(*fd, &i_len, sizeof(int));
        printf("Picture size : %d\n", i_len);

        char *p_array = calloc(i_len, 1);
        read(*fd, p_array, i_len);

        FILE *image;

        char *path = calloc(10 + (*num / 10) + 2 + 4, 1);
        if (!path)
            errx(EXIT_FAILURE, "calloc()");
        strcat(path, path_images);
        char *number = calloc((*num / 10) + 2, 1);
        sprintf(number, "%d", *num);
        strcat(path, number);
        strcat(path, extension_BMP);

        image = fopen(path, "wb+");
        printf("\nConcat done\n");
        printf("%d\n", *num);
        printf("%s\n", path);
        fwrite(p_array, 1, i_len, image);

        free(path);
        free(number);
        free(p_array);
        fclose(image);
    }
    else
    {
        printf("Image Existe po !\n");
    }
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