#include <err.h>
#include <stdlib.h>
#include "shared_queue.h"

shared_queue* shared_queue_new()
{
	sem_t lock;
	sem_t size;
	shared_queue* sq = malloc(sizeof(shared_queue));

	if (sem_init(&lock, 0, 1) == -1)
		errx(EXIT_FAILURE, "sem_init()");
	sq->lock = lock;

	if (sem_init(&size, 0, 0) == -1)
		errx(EXIT_FAILURE, "sem_init()");
	sq->size = size;
	sq->queue = NULL;

	return sq;
}

void shared_queue_push(shared_queue* sq, int val)
{
	if (sem_wait(&(sq->lock)) == -1)
		errx(EXIT_FAILURE, "sem_wait()");

	queue* new = queue_push(sq->queue, val);
	sq->queue = new;

	if (sem_post(&(sq->lock)) == -1)
		errx(EXIT_FAILURE, "sem_post()");
	if (sem_post(&(sq->size)) == -1)
		errx(EXIT_FAILURE, "sem_post()");
}

int shared_queue_pop(shared_queue* sq)
{
	if (sem_wait(&(sq->size)) == -1)
		errx(EXIT_FAILURE, "sem_wait()");
	if (sem_wait(&(sq->lock)) == -1)
		errx(EXIT_FAILURE, "sem_wait()");

	int pop;
	queue* popped = queue_pop(sq->queue, &pop);
	sq->queue = popped;

	if (sem_post(&(sq->lock)) == -1)
		errx(EXIT_FAILURE, "sem_post()");

	return pop;
}

void shared_queue_destroy(shared_queue* sq)
{
	queue* q = sq->queue;
	queue_empty(&q);

	if (sem_destroy(&(sq->lock)))
		errx(EXIT_FAILURE, "sem_destroy()");
	if (sem_destroy(&(sq->size)) == -1)
		errx(EXIT_FAILURE, "sem_destroy()");

	free(sq);
}
