#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_struct.h"
#include "game_ext.h"


game safeMalloc(void);
void checkGame(game g, uint i, uint j, char *text);
void putStatus(game g, uint i, uint j);

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints, color *colors, bool wrapping,
                  neighbourhood neigh){
  game g = safeMalloc();
  if (constraints == NULL) {
    exit(EXIT_FAILURE);
  }
  g->row=nb_rows;
  g->col=nb_cols;
  g->wrapping=wrapping;
  g->neighbour=neigh;
  // puts the constraints and colors of the wanted game
  for (int i = 0; i < nb_cols; i++) {
    for (int j = 0; j < nb_rows; j++) {
      g->contrainte[j + nb_rows * i] = constraints[j + nb_rows * i];
      if (colors == NULL) {
        g->couleur[j + nb_rows * i] = EMPTY;
      } else {
        g->couleur[j + nb_rows * i] = colors[j + nb_rows * i];
      }
    }
  }
  // changes the status according to the colors and constraints previously chosen
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->row; j++) {
      putStatus(g, i, j);
    }
  }
  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh){
    return NULL;
}

uint game_nb_rows(cgame g){
    return g->row;
}

uint game_nb_cols(cgame g){
    return g->col;
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