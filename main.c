#include "graph.h"
#include "parser.h"
#include "graph_operation.h"
#include "graph_processing.h"

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
    parser(&data_1,"0",3);
    parser(&data_2,"0",3);
    parser(&data_3,"1",3);
        
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