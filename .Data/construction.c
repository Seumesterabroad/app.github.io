#include "construction.h"

void fill(SDL_Surface* res, size_t x, size_t y, char* string)
{
    if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
    }
    TTF_Font *font = TTF_OpenFont("arial.ttf",15); // The linux path
    if(!font){printf("TTF_OpenFont: %s\n", TTF_GetError());;exit(1);}
    
    SDL_Color foregroundColor = { 0, 0, 0, 0};
    SDL_Color backgroundColor = { 255, 255, 255,0};

    SDL_Surface* textSurface = TTF_RenderText_Shaded(font, string, foregroundColor, backgroundColor);

    SDL_Rect textLocation = { x, y, 0, 0 };

    SDL_BlitSurface(textSurface, NULL, res, &textLocation);
    
    SDL_FreeSurface(textSurface);

    TTF_CloseFont(font);
}

void tasksBeforeAfter(graph_p G, SDL_Surface* res)
{
    int* before = calloc(G->num_vertices,sizeof(int));
    int after = 0;
    for (int i = 1; i < G->num_vertices-1 ; i++)
    {
        for(adjlist_node_p adj = G->nodelist[i].head; adj != NULL; adj = adj->next)
        {
            if(adj->vertex != G->num_vertices-1)
            {
                char str[128];
                //Tasks After
                sprintf(str,"%d/",adj->vertex);
                fill(res,(702 + after*25) ,(2 + i*25),str);
                after += 1;

                //Tasks Before
                sprintf(str,"%d/",i);
                fill(res,(602 + before[adj->vertex]*25) ,(2 + adj->vertex*25),str);
                before[adj->vertex] += 1;
            }
            
        }
        if (after == 0)
            fill(res,702  ,(2 + i*25),"None");
        after = 0;
    }
    for (int i = 1; i < G->num_vertices-1 ; i++)
    {
        if(before[i] == 0)
        {
            fill(res,602  ,(2 + i*25),"None");
        }
    }
    free(before);
}

void plotGraph(graph_p G, SDL_Surface* res)
{
    fill(res,2,2,"Nom");
    fill(res,202,2,"Temps");
    fill(res,302,2,"Le plus tot");
    fill(res,402,2,"Le plus tard");
    fill(res,502,2,"Marge");
    fill(res,602,2,"Antecedants");
    fill(res,702,2,"Ulterieurs");
    fill(res,802,2,"Tache Critique");
    for (int i = 1; i < G->num_vertices-1; i++)
    {
        char str[128];
        sprintf(str, "%d : %s", i, G->nodelist[i].name);
        fill(res,2,(2 + i*25),str);
        sprintf(str, "%lu", G->nodelist[i].time);
        fill(res,202,(2 + i*25),str);
        sprintf(str, "%lu", G->nodelist[i].soonest_time_done);
        fill(res,302,(2 + i*25),str);
        sprintf(str, "%lu", G->nodelist[i].latest_time_done);
        fill(res,402,(2 + i*25),str);
        sprintf(str, "%lu", G->nodelist[i].marge);
        fill(res,502,(2 + i*25),str); 
        if(!G->nodelist[i].marge)
            fill(res,802,(2 + i*25),"CRITICAL");
    }
    tasksBeforeAfter(G,res);
    
}

SDL_Surface* create(size_t num_vertices,graph_p G)
{
    size_t height = 1 + 25 * (num_vertices+1);
    size_t width = 900;
    SDL_Surface* res = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            if(j == 0 || j == 200 || j == 300 || j == 400  || j == 500 || j == 600 || j==700 || j==800|| j== 899 || i % 25 == 0)
            {
                put_pixel(res,j,i,SDL_MapRGB(res->format, 0,0,0));
            }
            else
            {
                put_pixel(res,j,i,SDL_MapRGB(res->format, 255,255,255));
            }
            
        }
        
    }
    

    plotGraph(G,res);

    return res;
}
