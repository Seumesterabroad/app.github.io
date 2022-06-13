/*graph.h*/
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "string.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "SDL/SDL_ttf.h"


/* Adjacency list node*/
typedef struct adjlist_node
{
    int vertex;                /*Index to adjacency list array*/
    struct adjlist_node *next; /*Pointer to the next node*/
}adjlist_node_t, *adjlist_node_p;

/* Adjacency list */
typedef struct node
{
    int adj_number;           /*number of members in the list (for future use)*/
    adjlist_node_t *head;      /*head of the adjacency linked list*/
    unsigned long time;
    unsigned long soonest_time_done;
    unsigned long latest_time_done;
    unsigned long marge;
    char* name;
}node_t, *node_p;

/* Graph structure. A graph is an array of adjacency lists.
   Size of array will be number of vertices in graph*/
typedef struct graph
{
    int num_vertices;         /*Number of vertices*/
    node_t *nodelist;     /*node lists' array*/
}graph_t, *graph_p;

typedef struct data_pert
{
    unsigned char *anterio;
    unsigned long duree;
    char* name;
    unsigned long index;
}data_pert;

typedef struct list
{
    struct data_pert *data_pert;
    struct list *next;
}list;

#endif
