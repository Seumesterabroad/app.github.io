#ifndef _GRAPH_OPERATION_H_
#define _GRAPH_OPERATION_H_

#include "graph.h"

adjlist_node_p createNode(int v);

graph_p createGraph(int n);

void destroyGraph(graph_p graph);

void add_edge(graph_t *graph, int src, int dest);

void display_graph(graph_p graph);

void add_time(graph_p graph, unsigned long node, unsigned long time);

void add_soonest_time(graph_p graph, unsigned long node, unsigned long soonest_time);

void add_latest_time(graph_p graph, unsigned long node, unsigned long latest_time);

void add_marge(graph_p graph, unsigned long node, unsigned long marge);

void add_name(graph_p graph, unsigned long node, char* name);

#endif