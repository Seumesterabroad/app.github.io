/*graph.c*/
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <err.h>
#include "string.h"

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

void add_data(graph_p graph, data_pert data)
{
    unsigned long nb_comp = graph->num_vertices;
    unsigned long node = data.index;
    add_time(graph,node,data.duree);
    add_name(graph,node,data.name);
    for(size_t i = 0; i < nb_comp; i++)
    {
        if(data.anterio[i])
        {
            add_edge(graph,i,node);
        }
    }
}

void init_start_end(graph_p graph)
{
    add_name(graph,0,"Start");
    graph->nodelist[0].time = 0;

    add_name(graph,graph->num_vertices - 1,"End");
    graph->nodelist[graph->num_vertices - 1].time = 0;
}

void update_end(graph_p graph)
{
    int i;
    for (i = 0; i < graph->num_vertices; i++)
    {
        adjlist_node_p adjListPtr = graph->nodelist[i].head;
        if (adjListPtr == NULL && i != graph->num_vertices-1)
        {
            add_edge(graph,i,graph->num_vertices-1);
        }
    }
}

graph_p data_graph(struct list* head, int nb_comp) 
{
    //We create the graph
    graph_p graph = createGraph(nb_comp + 2);

    init_start_end(graph);

    //we add all the nodes and edges from the data pert list
    struct data_pert *data_pert;
    for(struct list* current = head->next; current != NULL; current = current->next)
    {
        data_pert = current->data_pert;
        add_data(graph,*data_pert);
    }

    update_end(graph);

    return graph;
}

int parseur(struct data_pert* data, char* str, unsigned long nb_compo)
{
    data->anterio = (unsigned char*)calloc(nb_compo, sizeof(unsigned char));
    char current = str[0];
    int index = 0;
    unsigned long anter = 0;
    int passed = 0;
    int in_number = 0;
    while (current != '\0')
    {
        //We read the number
        if(current >= 48 && current <= 57 && !passed)
        {
            anter *= 10;
            anter += current - 48;
            in_number = 1;
        }
        //There is a space between numbers
        else if(current >= 48 && current <= 57 && passed)
        {
            return 0;
        }
        //end of anterior
        else if(current == '/')
        {
            if(anter >= data->index)
                return 0;
            data->anterio[anter] = 1;
            anter = 0;
            passed = 0;
            in_number = 0; 
        }
        //spaces
        else if(current == ' ')
        {
            if(in_number)
            {
                passed = 1;
            } 
        }
        else
        {
            return 0;
        }
        index += 1;
        current = str[index]; 
    }
    if(in_number)
    {
        if(anter >= data->index)
            return 0;
        data->anterio[anter] = 1; 
    }   
    return 1;
}

int main()
{
    data_pert data_1 = 
    {
      .anterio = NULL,
      .duree = 4,
      .name = "Task 1",
      .index = 1
    };
    
    data_pert data_2 = 
    {
      .anterio = NULL,
      .duree = 1,
      .name = "Task 2",
      .index = 2
    };
    
    data_pert data_3 = 
    {
      .anterio = NULL,
      .duree = 6,
      .name = "Task 3",
      .index = 3
    };
    
    list sentinelle = 
    {
        .data_pert = NULL,
        .next = NULL,
    };
    
    list* sentinel = &sentinelle;
    parseur(&data_1,"0",3);
    parseur(&data_2,"0",3);
    parseur(&data_3,"1/2",3);
        
    struct list *temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
        
    
    struct list *new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_1;
    new_c -> next = NULL;
    
    temp->next = new_c;
    
    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
        
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_2;
    new_c -> next = NULL; 
    temp->next = new_c;
    
    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_3;
    new_c -> next = NULL; 
    temp->next = new_c;

    graph_p graph = data_graph(sentinel,3);

    display_graph(graph);
}
