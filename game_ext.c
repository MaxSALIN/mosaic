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

    game g = safeMalloc();
    g->wrapping = wrapping;
    g->row = nb_rows;
    g->col = nb_cols;

    // creates a game where all squares are UNSATISFIED, EMPTY and UNCONSTRAINED
    for (int i = 0; i < (g->col*g->row); i++) {
        g->statut[i] = UNSATISFIED;
        g->couleur[i] = EMPTY;
        g->contrainte[i] = UNCONSTRAINED;
    }
    
  return g;
}

uint game_nb_rows(cgame g){
    return g->row;
}

uint game_nb_cols(cgame g){
    return g->col;
}

bool game_is_wrapping(cgame g){
    return g->wrapping;
}

neighbourhood game_get_neighourhood(cgame g){
    return g->neighbour;
}

void game_undo(game g){

}

void game_redo(game g){
    
}