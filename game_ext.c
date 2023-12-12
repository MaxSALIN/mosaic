#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_struct.h"
#include "game_ext.h"
#include "queue.h"

game safeMalloc(uint row, uint col);
void checkGame(game g, uint i, uint j, char *text);
void putStatus(game g, uint i, uint j);

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints, color *colors, bool wrapping,
                  neighbourhood neigh){
  game g = safeMalloc(nb_rows, nb_cols);
  if (constraints == NULL) {
    exit(EXIT_FAILURE);
  }
  g->row=nb_rows;
  g->col=nb_cols;
  g->wrapping=wrapping;
  g->neighbour=neigh;
  //g->queue=queue_new();
  //g->queue_saves=queue_new();
  // puts the constraints and colors of the wanted game
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      g->contrainte[j + nb_cols * i] = constraints[j + nb_cols * i];
      if (colors == NULL) {
        g->couleur[j + nb_cols * i] = EMPTY;
      } else {
        g->couleur[j + nb_cols * i] = colors[j + nb_cols * i];
      }
    }
  }
  // changes the status according to the colors and constraints previously chosen
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      putStatus(g, i, j);
    }
  }
  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh){

    game g = safeMalloc(nb_rows, nb_cols);
    g->wrapping = wrapping;
    g->row = nb_rows;
    g->col = nb_cols;
    g->neighbour=neigh;
    //g->queue=queue_new();
    //g->queue_saves=queue_new();
    // creates a game where all squares are UNSATISFIED, EMPTY and UNCONSTRAINED
    for (int i = 0; i < (nb_cols*nb_rows); i++) {
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

neighbourhood game_get_neighbourhood(cgame g){
    return g->neighbour;
}

void game_undo(game g){

  if (!queue_is_empty(g->queue)) {
    game val=queue_peek_head(g->queue);
    checkGame(val, 0, 0, "game_undo");
    queue_push_head(g->queue_saves, game_copy(g));
    queue_pop_head(g->queue);
    for (int i = 0; i < g->row; i++) {
      for (int j = 0; j < g->col; j++) {
          g->statut[j + g->col* i]=val->statut[j + g->col* i];
          g->contrainte[j + g->col* i]=val->contrainte[j + g->col* i];
          g->couleur[j + g->col* i]=val->couleur[j + g->col* i];
      }
    }
    
  } else {
    printf("Error: no more undo");
  }
}

void game_redo(game g){
  if (!queue_is_empty(g->queue_saves)) {
    game val=queue_peek_head(g->queue_saves);
    checkGame(val, 0, 0, "game_redo");
    queue_push_head(g->queue, val);
    queue_pop_head(g->queue_saves);
    for (int i = 0; i < g->row; i++) {
      for (int j = 0; j < g->col; j++) {
          g->statut[j + g->col* i]=val->statut[j + g->col* i];
          g->contrainte[j + g->col* i]=val->contrainte[j + g->col* i];
          g->couleur[j + g->col* i]=val->couleur[j + g->col* i];
      }
    }
    } else {
       printf("Error: no more redo");
    }
}
