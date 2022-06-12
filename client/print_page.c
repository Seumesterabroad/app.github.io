#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

const size_t BUFFER_SIZE = 32;

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

void print_page(char* path, char* name)
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

	// Send name Size
	printf("Sending Name Size\n");
	int n_size = strlen(name);
	write(sfd, &n_size, sizeof(n_size));
	printf("Name Size = %d\n", n_size);

	// Send Name
	printf("Sending Name\n");
	write(sfd, name, n_size);

	// Get Picture Size
	printf("Getting Picture Size\n");
	FILE *picture;
	picture = fopen(path, "r");
	int p_size;
	fseek(picture, 0, SEEK_END);
	p_size = ftell(picture);
	printf("Size = %d\n", p_size);
	fseek(picture, 0, SEEK_SET);

	// Send Picture Size
	printf("Sending Picture Size\n");
	write(sfd, &p_size, sizeof(p_size));

	// Send Picture as Byte Array
	printf("Sending Picture as Byte Array\n");
	char pic_buffer[p_size];
	while(!feof(picture))
	{
		fread(pic_buffer, 1, sizeof(pic_buffer), picture);
		write(sfd, pic_buffer, sizeof(pic_buffer));
		bzero(pic_buffer, sizeof(pic_buffer));
	}

	close(sfd);
}
