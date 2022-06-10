#include "graph_processing.h"

struct queue* queue_empty(void) 
{ 
    return NULL; 
}

int queue_is_empty(struct queue *q) 
{ 
    return q == NULL; 
}

struct queue* queue_push(struct queue *q, int x) 
{
    struct queue *tmp;
    tmp = malloc(sizeof (struct queue));
    tmp->value = x;
    if (q) 
    {
        tmp->next = q->next;
        q->next = tmp;
    } 
    else 
    {
        tmp->next = tmp;
    }
    return tmp;
}

int queue_pop(struct queue **q) 
{
    struct queue *tmp = (*q)->next;
    int x = tmp->value;
    if (tmp == tmp->next)
        *q = NULL;
    else
        (*q)->next = tmp->next;
    free(tmp);
    return x;
}

void init_start_end(graph_p graph)
{
    add_name(graph,0,"Start");
    graph->nodelist[0].time = 0;

    add_name(graph,graph->num_vertices - 1,"End");
    graph->nodelist[graph->num_vertices - 1].time = 0;
}

void build_from_list(list* L, graph_p G, graph_p Ginv, unsigned long *dout, unsigned long *din)
{
    init_start_end(G);
    init_start_end(Ginv);
    int n = G->num_vertices - 2;
    for (list* it = L->next; it != NULL; it = it->next)
    {
        data_pert *data = it->data_pert;
        int node = data->index;

        add_name(G, node, data->name);
        add_name(Ginv, node, data->name);
        
        add_time(G, node, data->duree);
        add_time(Ginv, node, data->duree);
        for(int i = 1; i < n+1; i++)
        {
            if(data->anterio[i])
            {
                add_edge(G,i,node);
                add_edge(Ginv, node, i);

                din[node] += 1;
                dout[i] += 1;
            }
        }
        
    }

    for(int i = 1; i < n + 1; i++)
    {
        if(din[i] == 0)
        {
            add_edge(G, 0, i);
            add_edge(Ginv, i, 0);
            din[i] += 1;
            dout[0] += 1;
        }
        if(dout[i] ==0)
        {
            add_edge(G, i, n+1);
            add_edge(Ginv, n+1, i);
            din[n+1] += 1;
            dout[i] += 1;
        }
    }
}

void Bellman(graph_p G, int src, unsigned long* din, unsigned long* dist)
{
    struct queue *q = queue_empty();
    q = queue_push(q,src);
    int x;
    int y;
    while(!queue_is_empty(q))
    {
        x = queue_pop(&q);
        for(adjlist_node_p adj = G->nodelist[x].head; adj != NULL; adj = adj->next)
        {
            y = adj->vertex;
            din[y] -= 1;
            if(din[y] == 0)
            {
                q = queue_push(q, y);
            }
            if(dist[x] + G->nodelist[x].time > dist[y])
            {
                dist[y] = dist[x] + G->nodelist[x].time;
            }
        }
    }
}

void update_val(graph_p G, unsigned long *dist1,unsigned long *dist2)
{
    unsigned long end = dist1[G->num_vertices-1];
    for (int i = 0; i < G->num_vertices; i++)
    {
        G->nodelist[i].soonest_time_done = dist1[i];
        G->nodelist[i].latest_time_done = end - dist2[i] - G->nodelist[i].time;
        G->nodelist[i].marge = G->nodelist[i].latest_time_done - G->nodelist[i].soonest_time_done;
    }
}

void traitement(list* sentinel, int nb_comp)
{
    unsigned long *din = calloc(nb_comp + 2, sizeof(unsigned long));
    unsigned long *dout = calloc(nb_comp + 2, sizeof(unsigned long));
    graph_p G = createGraph(nb_comp + 2);
    graph_p Ginv = createGraph(nb_comp + 2);
    build_from_list(sentinel, G, Ginv, dout, din);


    unsigned long *dist1 = calloc(G->num_vertices, sizeof(unsigned long));
    Bellman(G, 0, din, dist1);

    unsigned long *dist2 = calloc(G->num_vertices, sizeof(unsigned long));
    Bellman(Ginv, nb_comp+1, dout, dist2);

    update_val(G,dist1,dist2);

    print_graph(G);

    free(dist1);
    free(dist2);
    free(din);
    free(dout);
    destroyGraph(G);
    destroyGraph(Ginv);
}

void print_graph(graph_p G)
{
    for(int i = 0; i < G->num_vertices; i++)
    {
        printf("Task %s:\nSoonest time to start: %lu\nLatest time to start: %lu\nSlack: %lu\n",G->nodelist[i].name, G->nodelist[i].soonest_time_done,G->nodelist[i].latest_time_done,G->nodelist[i].marge);
        if(G->nodelist[i].marge == 0)
            printf("CRITICAL TASK\n");
        printf("\n");
    }
}