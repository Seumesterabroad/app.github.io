#include <err.h>
#include <stdlib.h>
#include "queue.h"

queue* queue_push(queue* start, int val)
{
	// Dinasty a validé
	
	queue* q = malloc(sizeof(queue));
	if (!q)
		errx(EXIT_FAILURE, "malloc()");

	q->val = val;

	if (!start)
		q->next = q;
	else
	{
		q->next = start->next;
		start->next = q;
	}

	return q;
}

queue* queue_pop(queue* start, int *pval)
{
	if (start)
	{
		queue* old = start->next;
		int val = old->val;
		*pval = val;

		if (start != start->next)
		{
			start->next = old->next;
			free(old);
			return start;
		}
		
		free(start);
	}

	return NULL;
}

void queue_empty(queue** pstart)
{
	int pval;
	while (*pstart != NULL)
		*pstart = queue_pop(*pstart, &pval);
}
