#define _GNU_SOURCE

#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include "shared_queue.h"

// Number of threads.
const size_t THREAD_COUNT = 4;

// Buffer size in bytes.
const size_t BUFFER_SIZE = 256;

// Constant strings
char path[] = "./graph_pic/graph_";
char extension_BMP[] = ".bmp";

void rewrite(int fd, const void *buf, size_t count)
{
	ssize_t written = 0;
	while (written < (ssize_t)count)
	{
		written = write(fd, buf, count);
		if (written == -1)
			err(1, "rewrite() : an error occurred");
	}
}

void echo(int fd_in, int fd_out)
{
	char buffer[BUFFER_SIZE];
	ssize_t r;
	while(1)
	{
		r = read(fd_in, buffer, BUFFER_SIZE);
		if (!r)
			break;
		if (r == -1)
			err(1, "echo() : an error occurred");
		rewrite(fd_out, buffer, r);
	}
}

void login(void* data, char** argv)
{
	*(data[0])++;
	if (!strcmp(data[1], argv[0]))
	{
		// Bon mot de passe
		write(*(data[2]), 0, 1);
	}

	else
	{
		// Mauvais mot de passe
		write(*(data[2]), 1, 1);
	}
}

// Function executed by the threads.
void* worker(void* arg)
{
	// Gets the shared queue.
	shared_queue* queue = arg;

	while (1)
	{
		int sock = shared_queue_pop(queue);

		int action;
		read(sock, &action, 1);

		// Connection
		if (action == 0)
		{
			int u_len, p_len;

			read(sock, &u_len, 16);
			char* username = calloc(u_len, 1);
			if (!username)
				errx(EXIT_FAILURE, "calloc()\n");
			read(sock, &username, u_len);

			read(sock, &p_len, 16);
			char* password = calloc(p_len, 1);
			if (!password)
				errx(EXIT_FAILURE, "calloc()\n");
			read(sock, &password, p_len);

			// DataBase
			sqlite3 *db;
			int rc;

			rc = sqlite3_open("../SeumesterAbroad.db", &db);
			if (rc)
				errx(EXIT_FAILURE, "Couldn't open database\n");

			// SQL Statement
			int* num_result = 0;

			int len = u_len + 44;
			char* query = calloc(len, 1);
			if (!query)
				errx(EXIT_FAILURE, "caaloc()\n");
			sprintf(query, "SELECT PASSWORD FROM users WHERE USERNAME = %s", username);

			void** data = [(void *)num_result, (void *)password, (void *)&sock];

			rc = sqlite3_exec(db, query, login, data, NULL);

			if (*num_result == 0) {
				int len_bis = 32 + u_len + p_len;
				char* add = calloc(len_bis, 1);
				sprintf(add, "INSERT INTO users VALUES (%s, %s, 0)", username, password);
				rc = sqlite3_exec(db, add, NULL, NULL, NULL);
			}

			sqlite3_close(db);
		}

		/*

		// Read Name Size
		printf("Reading Name Size\n");
		int n_size;
		read(sock, &n_size, sizeof(int));
		printf("Name size = %d\n", n_size);

		// Read Name
		printf("Reading Name\n");
		char n_array[n_size];
		printf("Array Created\n");
		read(sock, n_array, n_size);
		printf("Name = %s\n", n_array);
		
		// Read Picture Size
		printf("Reading Picture Size\n");
		int p_size;
		read(sock, &p_size, sizeof(int));
		printf("Picture size = %d\n", p_size); 

		// Read Picture Byte Array
		printf("Reading Picture Byte Array\n");
		char p_array[p_size];
		printf("Array created\n");
		read(sock, p_array, p_size);

		// Convert it back into Picture
		printf("Converting back into Picture\n");
		FILE *image;
		char *p_path = calloc(22 + n_size + 1, sizeof(char));
		if (!p_path)
			errx(EXIT_FAILURE, "calloc()");
		for (size_t i = 0; i < 18; i++) {
			p_path[i] = path[i];
			printf("%c", p_path[i]);
		}

		for (int i = 18; i < 18 + n_size; i++) {
			p_path[i] = n_array[i - 18];
			printf("%c", p_path[i]);
		}
		
		for (int i = 18 + n_size; i < 22 + n_size; i++) {
			p_path[i] = extension_BMP[i - 18 - n_size];
			printf("%c", p_path[i]);
		}

		image = fopen(p_path, "w");
		printf("\nConcat done\n");
		fwrite(p_array, 1, sizeof(p_array), image);
		
		free(p_path);
		fclose(image);
		close(sock);

		*/
	}
}

int main()
{
	// Creates the shared queue.
	shared_queue* queue = shared_queue_new();

	// Declare and initialize hints
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	// Get the linked list
	struct addrinfo* result;
	int err = getaddrinfo(NULL, "2048", &hints, &result);

	// Iterate over the linked list
	struct addrinfo* p;

	// File descriptor for the socket
	int sfd;
	for (p = result; p != NULL; p = p->ai_next)
	{
		// Try to create a socket
		sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sfd == -1)
			continue;
		int optval = 1;
		int err = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
		if (err == -1)
			errx(EXIT_FAILURE, "We left because of a mistake in setsockopt(), but idk if I was supposed to do that");

		// Try to bind the address
		err = bind(sfd, p->ai_addr, p->ai_addrlen);

		if (!err)
			break;
		else
			close(sfd);
	}

	// Free the linked list
	freeaddrinfo(result);

	// Handle errors
	if (!p)
		errx(EXIT_FAILURE, "No socket has been open");

	// Mark the socket as passive, it can now be used to accept incoming connection resquests
	err = listen(sfd, 5);

	// Exit if error
	if (err == -1)
	{
		close(sfd);
		errx(EXIT_FAILURE, "listen()");
	}

	pthread_t thr[THREAD_COUNT];
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		if (pthread_create(&thr[i], NULL, worker, queue))
			errx(EXIT_FAILURE, "pthread_create()");
	}

	while (1)
	{
		// Accept incoming connection
		// CA SERT POUR QU'EN FAIT LE PREMIER PORT IL SOIT LIBRE !!!!
		int cfd = accept(sfd, NULL, NULL);

		if (cfd == -1)
			errx(EXIT_FAILURE, "accept()");

		shared_queue_push(queue, cfd);
	}

	close(sfd);

	return 0;
}
