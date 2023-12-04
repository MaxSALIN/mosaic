#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GAME_SIZE (DEFAULT_SIZE * DEFAULT_SIZE)

struct game_s {
  color *couleur;
  constraint *contrainte;
  status *statut;
};

game safeMalloc() {
  // allocation of the game g
  game g = malloc(sizeof(game) * 3);
  // checks if it's correctly allocated
  if (g == NULL) {
    fprintf(stderr, "Game NULL\n");
    exit(EXIT_FAILURE);
  }

  // creates the different arrays in the structure game
  g->couleur = malloc(sizeof(*(g->couleur)) * GAME_SIZE);
  g->contrainte = malloc(sizeof(*(g->contrainte)) * GAME_SIZE);
  g->statut = malloc(sizeof(*(g->statut)) * GAME_SIZE);
  // checks if they're correctly allocated
  if (g->couleur == NULL || g->contrainte == NULL || g->statut == NULL) {
    fprintf(stderr, "Game parameters NULL\n");
    exit(EXIT_FAILURE);
  }
  return g;
}

void checkGame(game g, uint i, uint j, char *text) {
  // checks if i and j are between 0 and DEFAULT_SIZE-1
  if (i >= DEFAULT_SIZE || j >= DEFAULT_SIZE) {
    fprintf(stderr, "In %s, the parameters for i and j are not between 0 and %d", text, DEFAULT_SIZE - 1);
    exit(EXIT_FAILURE);
  }
  // checks if the game is not NULL
  if (g == NULL) {
    fprintf(stderr, "Game NULL in %s\n", text);
    exit(EXIT_FAILURE);
  }
}

void putStatus(game g, uint i, uint j) {
  // changes the status for the squares with a constraint of -1 (UNCONSTRAINT)
  if (game_get_constraint(g, i, j) == -1) {
    g->statut[j + DEFAULT_SIZE * i] = SATISFIED;
  }
  // changes the status for the squares with a constraint between 0 and MAX_CONSTRAINT
  else if (game_get_constraint(g, i, j) != -1) {
    if ((game_nb_neighbors(g, i, j, BLACK) == game_get_constraint(g, i, j) &&
         game_nb_neighbors(g, i, j, WHITE) ==
             (game_nb_neighbors(g, i, j, BLACK) + game_nb_neighbors(g, i, j, WHITE)) - game_get_constraint(g, i, j)) &&
        game_nb_neighbors(g, i, j, EMPTY) == 0) {
      g->statut[j + DEFAULT_SIZE * i] = SATISFIED;
    } else if ((game_nb_neighbors(g, i, j, BLACK) > game_get_constraint(g, i, j) ||
                game_nb_neighbors(g, i, j, WHITE) >
                    (game_nb_neighbors(g, i, j, BLACK) + game_nb_neighbors(g, i, j, WHITE) +
                     game_nb_neighbors(g, i, j, EMPTY)) -
                        game_get_constraint(g, i, j))) {  // bug à patch
      g->statut[j + DEFAULT_SIZE * i] = ERROR;
    } else if ((game_nb_neighbors(g, i, j, BLACK) < game_get_constraint(g, i, j) ||
                game_nb_neighbors(g, i, j, WHITE) <
                    (game_nb_neighbors(g, i, j, BLACK) + game_nb_neighbors(g, i, j, WHITE) +
                     game_nb_neighbors(g, i, j, EMPTY)) -
                        game_get_constraint(g, i, j))) {
      g->statut[j + DEFAULT_SIZE * i] = UNSATISFIED;
    }
  }
}

game game_new(constraint *constraints, color *colors) {
  game g = safeMalloc();
  if (constraints == NULL) {
    exit(EXIT_FAILURE);
  }
  // puts the constraints and colors of the wanted game
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      g->contrainte[j + DEFAULT_SIZE * i] = constraints[j + DEFAULT_SIZE * i];
      if (colors == NULL) {
        g->couleur[j + DEFAULT_SIZE * i] = EMPTY;
      } else {
        g->couleur[j + DEFAULT_SIZE * i] = colors[j + DEFAULT_SIZE * i];
      }
    }
  }
  // changes the status according to the colors and constraints previously chosen
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      putStatus(g, i, j);
    }
  }
  return g;
}

game game_new_empty(void) {
  game g = safeMalloc();
  // creates a game where all squares are UNSATISFIED, EMPTY and UNCONSTRAINED
  for (int i = 0; i < GAME_SIZE; i++) {
    g->statut[i] = UNSATISFIED;
    g->couleur[i] = EMPTY;
    g->contrainte[i] = UNCONSTRAINED;
  }
  return g;
}

game game_copy(cgame g) {
  checkGame((game)g, 0, 0, "game_copy");
  game g2 = safeMalloc();
  // copies the constraint, color and status from g to g2
  for (int i = 0; i < GAME_SIZE; i++) {
    g2->contrainte[i] = g->contrainte[i];
    g2->statut[i] = g->statut[i];
    g2->couleur[i] = g->couleur[i];
  }

  return g2;
}

bool game_equal(cgame g1, cgame g2) {
  checkGame((game)g2, 0, 0, "game_equal");
  checkGame((game)g1, 0, 0, "game_equal");
  // checks if the constraint, color and status are the same between g1 and g2 for all squares
  for (int i = 0; i < GAME_SIZE; i++) {
    if (g2->contrainte[i] != g1->contrainte[i] || g2->statut[i] != g1->statut[i] || g2->couleur[i] != g1->couleur[i]) {
      return false;
    }
  }

  return true;
}

void game_delete(game g) {
  checkGame(g, 0, 0, "game_delete");
  free(g->couleur);
  free(g->contrainte);
  free(g->statut);
  free(g);
}

void game_set_constraint(game g, uint i, uint j, constraint n) {
  checkGame(g, i, j, "game_get_constraint");
  g->contrainte[j + DEFAULT_SIZE * i] = n;
}

void game_set_color(game g, uint i, uint j, color c) {
  checkGame(g, i, j, "game_set_color");
  g->couleur[j + DEFAULT_SIZE * i] = c;
}

constraint game_get_constraint(cgame g, uint i, uint j) {
  checkGame((game)g, i, j, "game_get_constraint");
  return g->contrainte[j + DEFAULT_SIZE * i];
}

color game_get_color(cgame g, uint i, uint j) {
  checkGame((game)g, i, j, "game_get_color");
  return g->couleur[j + DEFAULT_SIZE * i];
}

bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next, uint *pj_next) {
  checkGame((game)g, i, j, "game_get_next_square");

  // cases of the sides
  if ((j == DEFAULT_SIZE - 1 && ((dir == RIGHT) || (dir == UP_RIGHT) || (dir == DOWN_RIGHT))) ||
      (i == DEFAULT_SIZE - 1 && ((dir == DOWN) || (dir == DOWN_RIGHT) || (dir == DOWN_LEFT))) ||
      (j == 0 && ((dir == LEFT) || (dir == DOWN_LEFT) || (dir == UP_LEFT))) ||
      (i == 0 && ((dir == UP) || (dir == UP_RIGHT) || (dir == UP_LEFT)))) {
    return false;
  }

  // changes the output values of pi_next and pj_next to match the direction
  switch (dir) {
    case UP:
      *pi_next = i - 1;
      *pj_next = j;
      break;
    case DOWN:
      *pi_next = i + 1;
      *pj_next = j;
      break;
    case LEFT:
      *pi_next = i;
      *pj_next = j - 1;
      break;
    case RIGHT:
      *pi_next = i;
      *pj_next = j + 1;
      break;
    case UP_LEFT:
      *pi_next = i - 1;
      *pj_next = j - 1;
      break;
    case UP_RIGHT:
      *pi_next = i - 1;
      *pj_next = j + 1;
      break;
    case DOWN_LEFT:
      *pi_next = i + 1;
      *pj_next = j - 1;
      break;
    case DOWN_RIGHT:
      *pi_next = i + 1;
      *pj_next = j + 1;
      break;
    case HERE:
      *pi_next = i;
      *pj_next = j;
      break;
    default:
      fprintf(stderr, "Error in game_get_next_square");
      exit(EXIT_FAILURE);
      break;
  }

  return true;
}

status game_get_status(cgame g, uint i, uint j) {
  checkGame((game)g, i, j, "game_get_status");
  return g->statut[j + DEFAULT_SIZE * i];
}

int game_nb_neighbors(cgame g, uint i, uint j, color c) {
  checkGame((game)g, i, j, "game_nb_neighbors");
  uint count = 0;
  uint pi_next = 0;
  uint pj_next = 0;
  // checks all around the square chosen the number of neighbors with the same color as c
  for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
    if (game_get_next_square((game)g, i, j, dir, &pi_next, &pj_next) == true) {
      if (game_get_color(g, pi_next, pj_next) == c) {
        count++;
      }
    }
  }
  return count;
}

void game_play_move(game g, uint i, uint j, color c) {
  checkGame(g, i, j, "game_play_move");
  // changes the color and status of the played square
  game_set_color(g, i, j, c);
  putStatus(g, i, j);
  uint pi_next = 0;
  uint pj_next = 0;
  // changes the status for the neighbor squares
  for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
    if (game_get_next_square(g, i, j, dir, &pi_next, &pj_next) == true) {
      putStatus(g, pi_next, pj_next);
    }
  }
}

bool game_won(cgame g) {
  checkGame((game)g, 0, 0, "game_won");
  // checks if all of the squares are colored at satisfied
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_color(g, i, j) == EMPTY || game_get_status(g, i, j) != SATISFIED) {
        return false;
      }
    }
  }
  return true;
}

void game_restart(game g) {
  checkGame(g, 0, 0, "game_restart");
  // puts all of the squares to EMPTY
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_color(g, i, j, EMPTY);
    }
  }
  // changes all of the status of the squares
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      putStatus(g, i, j);
    }
  }
}
