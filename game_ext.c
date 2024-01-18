#include "game_ext.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_struct.h"
#include "queue.h"

game safeMalloc(uint row, uint col);
void checkGame(game g, uint i, uint j, char *text);
void putStatus(game g, uint i, uint j);
void make_move(game g, uint i, uint j, color c);

game game_new_ext(uint nb_rows, uint nb_cols, constraint *constraints, color *colors, bool wrapping,
                  neighbourhood neigh) {
  game g = safeMalloc(nb_rows, nb_cols);
  if (constraints == NULL || nb_cols > 10 || nb_rows > 10) {
    fprintf(stderr, "Error in the parameters");
    exit(EXIT_FAILURE);
  }
  g->row = nb_rows;
  g->col = nb_cols;
  g->wrapping = wrapping;
  g->neighbour = neigh;
  //  puts the constraints and colors of the wanted game
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      g->contrainte[j+  nb_cols*i]= constraints[j + nb_cols * i];
      if (colors == NULL) {
        g->couleur[j+  nb_cols*i]= EMPTY;
      } else {
        g->couleur[j+  nb_cols*i]= colors[j + nb_cols * i];
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

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh) {
  if (nb_cols > 10 || nb_rows > 10) {
    fprintf(stderr, "Error in the parameters");
    exit(EXIT_FAILURE);
  }
  game g = safeMalloc(nb_rows, nb_cols);
  g->wrapping = wrapping;
  g->row = nb_rows;
  g->col = nb_cols;
  g->neighbour = neigh;
  //  creates a game where all squares are UNSATISFIED, EMPTY and UNCONSTRAINED
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      g->statut[j+  nb_cols*i] = UNSATISFIED;
      g->couleur[j+  nb_cols*i]= EMPTY;
      g->contrainte[j+  nb_cols*i]=UNCONSTRAINED;
    }
  }

  return g;
}

uint game_nb_rows(cgame g) { return g->row; }

uint game_nb_cols(cgame g) { return g->col; }

bool game_is_wrapping(cgame g) { return g->wrapping; }

neighbourhood game_get_neighbourhood(cgame g) { return g->neighbour; }

void game_undo(game g) {
  // checks if the queue is empty
  if (!queue_is_empty(g->queue)) {
    // if it's not, get the move stored inside and plays it
    move *m1 = queue_pop_head(g->queue);
    color color_game = m1->c;
    m1->c = game_get_color(g, m1->i, m1->j);
    make_move(g, m1->i, m1->j, color_game);
    // put back the old move in the queue_saves
    queue_push_head(g->queue_saves, m1);
  } else {
    printf("Error: no more undo");
  }
}

void game_redo(game g) {
  // checks if the queue_saves is empty
  if (!queue_is_empty(g->queue_saves)) {
    // if it's not, get the move stored inside and plays it
    move *m1 = queue_pop_head(g->queue_saves);
    color color_game = m1->c;
    m1->c = game_get_color(g, m1->i, m1->j);
    make_move(g, m1->i, m1->j, color_game);
    // put back the old move in the queue
    queue_push_head(g->queue, m1);
  } else {
    printf("Error: no more redo");
  }
}
