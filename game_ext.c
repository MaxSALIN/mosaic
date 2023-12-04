#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_struct.h"
#include "game_ext.h"

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints, color *colors, bool wrapping,
                  neighbourhood neigh){
                    return NULL;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh){
    return NULL;
}

uint game_nb_rows(cgame g){
    return 0;
}

uint game_nb_cols(cgame g){
    return 0;
}

bool game_is_wrapping(cgame g){
    return false;
}

neighbourhood game_get_neighourhood(cgame g){
    return 0;
}

void game_undo(game g){

}

void game_redo(game g){
    
}