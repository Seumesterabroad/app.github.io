#ifndef MAIN_H
#define MAIN_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct data_pert
{

    unsigned char *anterio;
    unsigned long duree;
    char* name;
    unsigned long index;

} data_pert;

typedef struct list
{
    struct data_pert *data_pert;
    struct list *next;
    
} list;


#endif
