#include "parser.h"

int parser(struct data_pert* data, char* str, unsigned long nb_compo)
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
