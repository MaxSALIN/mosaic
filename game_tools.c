#include "game_tools.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"
#include "game_struct.h"

game game_load(char *filename){
    return NULL;
}

void game_save(cgame g, char *filename){
    FILE* file = fopen(filename, "w");
    fprintf(file, "%d %d %d %d\n",g->row,g->col,g->wrapping,g->neighbour);
    for (int i = 0; i < g->row; i++)
    {
        for (int j = 0; j < g->col; j++)
        {
            if(game_get_constraint(g,i,j) == UNCONSTRAINED){
                fprintf(file, "-");
            }else{
                fprintf(file,"%d",game_get_constraint(g,i,j));
            }
            if(game_get_color(g,i,j) == EMPTY){
                fprintf(file,"e");
            }else if(game_get_color(g,i,j) == WHITE){
                fprintf(file,"w");
            }else{
                fprintf(file,"b");
            }
        }
        fprintf(file,"\n");
    }
    fclose(file);
}