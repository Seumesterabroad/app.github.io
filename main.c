#include "graph.h"
#include "parser.h"
#include "graph_operation.h"
#include "graph_processing.h"

int main()
{
    data_pert data_1 = 
    {
      .anterio = NULL,
      .duree = 20,
      .name = "Commande",
      .index = 1
    };
    
    data_pert data_2 = 
    {
      .anterio = NULL,
      .duree = 30,
      .name = "Recrutement",
      .index = 2
    };
    
    data_pert data_3 = 
    {
      .anterio = NULL,
      .duree = 10,
      .name = "Plan de commition",
      .index = 3
    };

    data_pert data_4 = 
    {
      .anterio = NULL,
      .duree = 3,
      .name = "Installation",
      .index = 4
    };

    data_pert data_5 = 
    {
      .anterio = NULL,
      .duree = 3,
      .name = "Entretiens",
      .index = 5
    };

    data_pert data_6 = 
    {
      .anterio = NULL,
      .duree = 3,
      .name = "Formation",
      .index = 6
    };

    data_pert data_7 = 
    {
      .anterio = NULL,
      .duree = 30,
      .name = "Réservations",
      .index = 7
    };

    data_pert data_8 = 
    {
      .anterio = NULL,
      .duree = 15,
      .name = "Commandes des affiches",
      .index = 8
    };

    data_pert data_9 = 
    {
      .anterio = NULL,
      .duree = 4,
      .name = "Animations",
      .index = 9
    };

    data_pert data_10 = 
    {
      .anterio = NULL,
      .duree = 10,
      .name = "Commande de fleurs",
      .index = 10
    };

    data_pert data_11 = 
    {
      .anterio = NULL,
      .duree = 7,
      .name = "Décoration",
      .index = 11
    };

    data_pert data_12 = 
    {
      .anterio = NULL,
      .duree = 1,
      .name = "Cocktail",
      .index = 12
    };
    
    list sentinelle = 
    {
        .data_pert = NULL,
        .next = NULL,
    };
    
    list* sentinel = &sentinelle;
    parser(&data_1,"0",13);
    parser(&data_2,"0",13);
    parser(&data_3,"0",13);
    parser(&data_4,"1",13);
    parser(&data_5,"2",13);
    parser(&data_6,"4/5",13);
    parser(&data_7,"3",13);
    parser(&data_8,"7",13);
    parser(&data_9,"6/8",13);
    parser(&data_10,"4",13);
    parser(&data_11,"6/10",13);
    parser(&data_12,"9/10",13);
        
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

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_4;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_5;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_6;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_7;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_8;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_9;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_10;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_11;
    new_c -> next = NULL; 
    temp->next = new_c;

    temp = sentinel;
    while(temp -> next != NULL)
        temp = temp->next;
    new_c = malloc(sizeof(list));
    new_c -> data_pert = &data_12;
    new_c -> next = NULL; 
    temp->next = new_c;

    int nb_comp = 12;
    traitement(sentinel,nb_comp);

    new_c = sentinel->next;
    for (list* it = sentinel->next; it != NULL; )
    {
        new_c = it;
        temp = it->next;
        free(new_c);
        it = temp; 
    }

    free(data_1.anterio);
    free(data_2.anterio);
    free(data_3.anterio);
    free(data_4.anterio);
    free(data_5.anterio);
    free(data_6.anterio);
    free(data_7.anterio);
    free(data_8.anterio);
    free(data_9.anterio);
    free(data_10.anterio);
    free(data_11.anterio);
    free(data_12.anterio);
    
    return 0;
}