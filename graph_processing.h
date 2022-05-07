#ifndef _GRAPH_PROCESSING_H_
#define _GRAPH_PROCESSING_H_

#include "graph.h"

void init_start_end(graph_p graph);

void update_end(graph_p graph);

void add_data(graph_p graph, data_pert data);

graph_p data_graph(struct list* head, int nb_comp);

#endif