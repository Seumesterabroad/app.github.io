#include "construction.h"

void plotSheet(SDL_Surface* res, size_t height, size_t width)
{
    //vertical lines
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            if(j == 0 || j == 200 || j == 300 || j == 400  || j == 500 || j == 600 || j== 699 || i % 25 == 0)
            {
                put_pixel(res,j,i,SDL_MapRGB(res->format, 0,0,0));
            }
        }
        
    }
    
}

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

int get_smallest(graph_p G, int* M)
{
    unsigned long smallest = 100000000;
    int res = -1;
    for (int i = 1; i < G->num_vertices-1; i++)
    {
        if(M[i] == 0 && G->nodelist[i].soonest_time_done < smallest)
        {
            res = i;
            smallest = G->nodelist[i].soonest_time_done;
        }
    }
    M[res] = 1;
    return res;
}

void plotGraph(graph_p G, SDL_Surface* res)
{
    fill(res,2,2,"Name");
    fill(res,202,2,"Time Needed");
    fill(res,302,2,"Soonest time");
    fill(res,402,2,"Latest time");
    fill(res,502,2,"Slack");
    fill(res,602,2,"Critical task");

    int* M = calloc(G->num_vertices, sizeof(int));
    for (int i = 1; i < G->num_vertices-1; i++)
    {
        size_t smallest = i;
        smallest = get_smallest(G,M);
        fill(res,2,(2 + i*25),G->nodelist[smallest].name);
        char str[4];
        sprintf(str, "%lu", G->nodelist[smallest].time);
        fill(res,202,(2 + i*25),str);
        sprintf(str, "%lu", G->nodelist[smallest].soonest_time_done);
        fill(res,302,(2 + i*25),str);
        sprintf(str, "%lu", G->nodelist[smallest].latest_time_done);
        fill(res,402,(2 + i*25),str);
        sprintf(str, "%lu", G->nodelist[smallest].marge);
        fill(res,502,(2 + i*25),str);
        if(!G->nodelist[smallest].marge)
            fill(res,602,(2 + i*25),"CRITICAL");
    }
    free(M);
    
}

SDL_Surface* create(size_t num_vertices,graph_p G)
{
    size_t height = 1 + 25 * (num_vertices+1);
    size_t width = 700;
    SDL_Surface* res = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            put_pixel(res,j,i,SDL_MapRGB(res->format, 255,255,255));
        }
        
    }
    plotSheet(res,height,width);
    

    plotGraph(G,res);

    return res;
}

