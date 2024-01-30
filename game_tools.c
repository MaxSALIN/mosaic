#include "game_tools.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"
#include "game_struct.h"

game game_load(char *filename) {

    FILE* save = fopen(filename, "r");
    if (save == NULL) {
        exit(EXIT_FAILURE);
    }
    uint row = 0;
    uint col = 0;
    uint wrapping = 0;
    uint neighbour = 0;
     

    // we haven't reach the end of the file yet.
    while (!feof(save)) {

        // first line of the file.
        
        row = fgetc(save);
        col = fgetc(save);

        wrapping = fgetc(save);
        neighbour = fgetc(save);
        
    }
    constraint constraints1[row * col];
    color colors1[row * col];
    while (!feof(save)) {

        

        // second line of the file.

        for (int x = 0; x < row; x++) {
            for (int y = 0; y < col; y++) {

                if (fgetc(save) == '-') {
                    constraints1[y+col*x]=UNCONSTRAINED;
                } else {
                    constraints1[y+col*x]=fgetc(save);
                }

                if (fgetc(save) == 'e') {
                    colors1[y+col*x]=EMPTY;
                } else if (fgetc(save) == 'w') {
                    colors1[y+col*x]=WHITE;
                } else if (fgetc(save) == 'b') {
                    colors1[y+col*x]=BLACK;
                }
            }
        }     
    }
    fclose(save);
    
    return game_new_ext(row, col, constraints1, colors1, wrapping, neighbour);
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
