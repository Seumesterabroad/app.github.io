#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 32;
int action = 0;

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

void connexion(int* fd, char** username, char** password)
{
	printf("Entered connexion\n");

	int u_len = strlen(*username);
	int p_len = strlen(*password);
	
	int k = 0;

	write(*fd, &k, 1);
	write(*fd, &u_len, 16);
	printf("u_len sent OK\n");
	printf("username = %s\n", *username);
	write(*fd, *username, u_len);
	printf("username sent OK\n");
	write(*fd, &p_len, 16);
	write(*fd, *password, p_len);

	int ret = 0;

	read(*fd, &ret, 1);

	if (ret == 0) {
		printf("Connected - Username : %s\n", *username);
	}

	else {
		printf("Wrong password !");
	}
}

void send_image(int* fd, char** username, char** path)
{
	// Send code 1
	int k = 1;
	write(*fd, &k, 1);

	//Send username and its length
	write(*fd, strlen(*username), 16);
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
	write(*fd, &p_size, sizeof(p_size));

	// Send Picture as Byte Array
	printf("Sending Picture as Byte Array\n");
	char pic_buffer[p_size];
	while(!feof(picture))
	{
		fread(pic_buffer, 1, sizeof(pic_buffer), picture);
		write(*fd, pic_buffer, sizeof(pic_buffer));
		bzero(pic_buffer, sizeof(pic_buffer));
	}
}

void print_page(char* username, char* password)
{

	// Declare and initialize hints
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	// Get the linked list
	struct addrinfo* result;
	int err = getaddrinfo("localhost", "2048", &hints, &result);

	// File descriptor for the socket askip aussi
	int sfd;

	// Pointer used to iterate over the linked list askip
	struct addrinfo* p;
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

	if (action == 0) {
		connexion(&sfd, &username, &password);
	}

	else if (action == 1) {
		send_image(&sfd, &username, "example.bmp")
	}

	else if (action == 2) {
		//get_images();
	}

	else if (action == 3) {
		//get_image(number);
	}

	close(sfd);
}
