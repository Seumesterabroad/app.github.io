#include "graph_processing.h"
#include "graph_operation.h"


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