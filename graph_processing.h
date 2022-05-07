#ifndef _GRAPH_PROCESSING_H_
#define _GRAPH_PROCESSING_H_

#include "graph.h"
#include "graph_operation.h"

struct queue 
{
    struct queue *next;
    int value;
};

void init_start_end(graph_p graph);

void build_from_list(list* L, graph_p G, graph_p Ginv, unsigned long *dout, unsigned long *din);

void Bellman(graph_p G, int src, unsigned long* din, unsigned long* dist);

#endif