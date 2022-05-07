#include "graph_operation.h"

/* Function to create an adjacency list node*/
adjlist_node_p createNode(int v)
{
    adjlist_node_p newNode = (adjlist_node_p)malloc(sizeof(adjlist_node_t));
    if(!newNode)
        errx(EXIT_FAILURE,"Unable to allocate memory for new node");

    newNode->vertex = v;
    newNode->next = NULL;

    return newNode;
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
graph_p createGraph(int n)
{
    int i;
    graph_p graph = (graph_p)malloc(sizeof(graph_t));
    if(!graph)
        errx(EXIT_FAILURE,"Unable to allocate memory for graph");
    graph->num_vertices = n;

    /* Create an array of adjacency lists*/
    graph->nodelist = (node_p)malloc(n * sizeof(node_t));
    if(!graph->nodelist)
        errx(EXIT_FAILURE,"Unable to allocate memory for adjacency list array");

    for(i = 0; i < n; i++)
    {
        graph->nodelist[i].head = NULL;
        graph->nodelist[i].adj_number = 0;
    }

    return graph;
}

/*Destroys the graph*/
void destroyGraph(graph_p graph)
{
    if(graph)
    {
        if(graph->nodelist)
        {
            int v;
            /*Free up the nodes*/
            for (v = 0; v < graph->num_vertices; v++)
            {
                adjlist_node_p adjListPtr = graph->nodelist[v].head;
                while (adjListPtr)
                {
                    adjlist_node_p tmp = adjListPtr;
                    adjListPtr = adjListPtr->next;
                    free(tmp);
                }
            }
            /*Free the adjacency list array*/
            free(graph->nodelist);
        }
        /*Free the graph*/
        free(graph);
    }
}

/* Adds an edge to a graph*/
void add_edge(graph_t *graph, int src, int dest)
{
    /* Add an edge from src to dst in the adjacency list*/
    adjlist_node_p newNode = createNode(dest);
    newNode->next = graph->nodelist[src].head;
    graph->nodelist[src].head = newNode;
    graph->nodelist[src].adj_number++;
}

/* Function to print the adjacency list of graph*/
void display_graph(graph_p graph)
{
    int i;
    for (i = 0; i < graph->num_vertices; i++)
    {
        adjlist_node_p adjListPtr = graph->nodelist[i].head;
        printf("Node %d: time = %ld, name = %s\n", i, graph->nodelist[i].time,graph->nodelist[i].name);
        while (adjListPtr)
        {
            printf("%d->", adjListPtr->vertex);
            adjListPtr = adjListPtr->next;
        }
        printf("NULL\n");
    }
}

void add_time(graph_p graph, unsigned long node, unsigned long time)
{
    graph->nodelist[node].time = time;
}

void add_soonest_time(graph_p graph, unsigned long node, unsigned long soonest_time)
{
    graph->nodelist[node].time = soonest_time;
}

void add_latest_time(graph_p graph, unsigned long node, unsigned long latest_time)
{
    graph->nodelist[node].time = latest_time;
}

void add_marge(graph_p graph, unsigned long node, unsigned long marge)
{
    graph->nodelist[node].time = marge;
}

void add_name(graph_p graph, unsigned long node, char* name)
{
    graph->nodelist[node].name = malloc(strlen(name)+1);
    strcpy(graph->nodelist[node].name, name);
}
