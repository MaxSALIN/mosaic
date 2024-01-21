#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

//#################################   Aux_functions   #################################//

/**
 * @brief Allocate safely a game.
 * @details Allocate the game safely and checks if it's correctly allocated.
 * @param row the number of rows.
 * @param col the number of columns.
 * @pre @p g must be a valid pointer toward a game structure.
 * @return a pointer toward a new game structure.
 **/
game safeMalloc(uint row, uint col) {
  // allocation of the game g
  game g = malloc(sizeof(game_s));
  // checks if it's correctly allocated
  if (g == NULL) {
    fprintf(stderr, "Game NULL\n");
    exit(EXIT_FAILURE);
  }
  // allocation of the queues
  g->queue = queue_new();
  g->queue_saves = queue_new();
  // checks if they're correctly allocated
  if (g->queue == NULL || g->queue_saves == NULL) {
    fprintf(stderr, "queue NULL\n");
    exit(EXIT_FAILURE);
  }
  // creates the different arrays in the structure game
  g->couleur = malloc(sizeof(*(g->couleur)) * row * col);
  g->contrainte = malloc(sizeof(*(g->contrainte)) * row * col);
  g->statut = malloc(sizeof(*(g->statut)) * row * col);
  // checks if they're correctly allocated
  if (g->couleur == NULL || g->contrainte == NULL || g->statut == NULL) {
    fprintf(stderr, "Game parameters NULL\n");
    exit(EXIT_FAILURE);
  }
  return g;
}

/**
 * @brief Checks if the game is correctly allocated
 * @details Checks the different settings of the game to see if it's correctly allocated. Will print a message of the error and the name of the function.
 * @param g a pointer to a game.
 * @param i a coordinate to a designed row.
 * @param j a coordinate to a designed column.
 * @param text the name of the function.
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i and j must be correct coordinates in the game size.
 **/
void checkGame(game g, uint i, uint j, char *text) {
  // checks if i and j are between 0 and g->row-1 or g->col-1
  if (i >= g->row || j >= g->col) {
    fprintf(stderr,
            "In %s, the parameters for i and j are not between 0 and %d for i and 0 and %d for j :\ni=%d\nj=%d\n", text,
            g->row - 1, g->col - 1, i, j);
    exit(EXIT_FAILURE);
  }
  // checks if the game is not NULL
  if (g == NULL) {
    fprintf(stderr, "Game NULL in %s\n", text);
    exit(EXIT_FAILURE);
  }
  // checks if queues are not NULL
  if (g->queue == NULL || g->queue_saves == NULL) {
    fprintf(stderr, "queue NULL in %s\n", text);
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Change the status of a square and its neighbours, depending on their neighbours's colors
 * @details Checks the constraint of the current square, and colors of the nearby squares to change its status. Does the same for nearby squares defined by its neigh
 * @param g a pointer to a game.
 * @param i a coordinate to a designed row.
 * @param j a coordinate to a designed column.
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i and j must be correct coordinates in the game size.
 **/
void putStatus(game g, uint i, uint j) {
  checkGame((game)g, 0, 0, "putStatus");
  // changes the status for the squares with a constraint of -1 (UNCONSTRAINT)
  if (game_get_constraint(g, i, j) == -1) {
    if (game_nb_neighbors(g, i, j, EMPTY) == 0) {
      g->statut[j + g->col * i] = SATISFIED;
    } else {
      g->statut[j + g->col * i] = UNSATISFIED;
    }
  }

  // changes the status for the squares with a constraint between 0 and MAX_CONSTRAINT
  else if (game_get_constraint(g, i, j) != -1) {
    if ((game_nb_neighbors(g, i, j, BLACK) == game_get_constraint(g, i, j) &&
         game_nb_neighbors(g, i, j, WHITE) ==
             (game_nb_neighbors(g, i, j, BLACK) + game_nb_neighbors(g, i, j, WHITE)) - game_get_constraint(g, i, j)) &&
        game_nb_neighbors(g, i, j, EMPTY) == 0) {
      g->statut[j + g->col * i] = SATISFIED;
    } else if ((game_nb_neighbors(g, i, j, BLACK) > game_get_constraint(g, i, j) ||
                game_nb_neighbors(g, i, j, WHITE) >
                    (game_nb_neighbors(g, i, j, BLACK) + game_nb_neighbors(g, i, j, WHITE) +
                     game_nb_neighbors(g, i, j, EMPTY)) -
                        game_get_constraint(g, i, j))) {
      g->statut[j + g->col * i] = ERROR;
    } else if ((game_nb_neighbors(g, i, j, BLACK) < game_get_constraint(g, i, j) ||
                game_nb_neighbors(g, i, j, WHITE) <
                    (game_nb_neighbors(g, i, j, BLACK) + game_nb_neighbors(g, i, j, WHITE) +
                     game_nb_neighbors(g, i, j, EMPTY)) -
                        game_get_constraint(g, i, j))) {
      g->statut[j + g->col * i] = UNSATISFIED;
    }
  }
}

/**
 * @brief Changes the color and the status of the square and its neighbours
 * @details Checks the constraint of the current square, and colors of the nearby squares to change its status. Does the same for nearby squares defined by its neigh
 * @param g a pointer to a game.
 * @param i a coordinate to a designed row.
 * @param j a coordinate to a designed column.
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p i and j must be correct coordinates in the game size.
 **/

void putAllStatus(game g, uint i, uint j){
  checkGame((game)g, 0, 0, "putAllStatus");
  uint pi_next = 0;
  uint pj_next = 0;
  // changes the status for the neighbor squares
  for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
    if (game_get_next_square(g, i, j, dir, &pi_next, &pj_next) == true) {
      putStatus(g, pi_next, pj_next);
    }
  }
}

// check the game next square 
bool check_game_nb_neighbors_aux(cgame g, uint i, uint j, direction dir, uint pi_next, uint pj_next, color c) {
    checkGame((game)g, 0, 0, "check_game_bv_neighbors_aux");
    if (game_get_next_square((game)g, i, j, dir, &pi_next, &pj_next) == true) {
      if (game_get_color(g, pi_next, pj_next) == c) {
        return true;
      }
    }
    return false;
}

//#################################   End   #################################//

game game_new(constraint *constraints, color *colors) {
  game g = safeMalloc(5, 5);
  g->row = DEFAULT_SIZE;
  g->col = DEFAULT_SIZE;
  g->wrapping = false;
  g->neighbour = FULL;

  if (constraints == NULL) {
    exit(EXIT_FAILURE);
  }
  // puts the constraints and colors of the wanted game
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
      g->contrainte[j + g->col * i]= constraints[j + g->col * i];
      if (colors == NULL) {
        g->couleur[j + g->col * i] = EMPTY;
      } else {
        g->couleur[j + g->col * i] = colors[j + g->col * i];
      }
    }
  }
  // changes the status according to the colors and constraints previously chosen
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
      putStatus(g, i, j);
    }
  }
  return g;
}

game game_new_empty(void) {
  game g = safeMalloc(5, 5);
  g->row = DEFAULT_SIZE;
  g->col = DEFAULT_SIZE;
  g->wrapping = false;
  g->neighbour = FULL;
  // creates a game where all squares are UNSATISFIED, EMPTY and UNCONSTRAINED
  for (int i = 0; i < g->col * g->row; i++) {
    g->statut[i] = UNSATISFIED;
    g->couleur[i] = EMPTY;
    g->contrainte[i] = UNCONSTRAINED;
  }
  return g;
}

game game_copy(cgame g) {
  checkGame((game)g, 0, 0, "game_copy");
  game g2 = safeMalloc(g->row, g->col);
  g2->row = g->row;
  g2->col = g->col;
  g2->neighbour = g->neighbour;
  g2->wrapping = g->wrapping;
  // copies the constraint, color and status from g to g2
  for (int i = 0; i < g2->col * g2->row; i++) {
    g2->contrainte[i] = g->contrainte[i];
    g2->statut[i] = g->statut[i];
    g2->couleur[i] = g->couleur[i];
  }

  return g2;
}

bool game_equal(cgame g1, cgame g2) {
  checkGame((game)g1, 0, 0, "game_equal");
  checkGame((game)g2, 0, 0, "game_equal");
  // checks if the constraint, color and status are the same between g1 and g2 for all squares
  for (int i = 0; i < g1->col * g1->row; i++) {
    if (g2->contrainte[i] != g1->contrainte[i] || g2->statut[i] != g1->statut[i] || g2->couleur[i] != g1->couleur[i] ||
        g1->row != g2->row || g1->col != g2->col || g1->neighbour != g2->neighbour || g1->wrapping != g2->wrapping) {
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
  queue_free_full(g->queue, free);
  queue_free_full(g->queue_saves, free);
  free(g);
}

void game_set_constraint(game g, uint i, uint j, constraint n) {
  checkGame(g, i, j, "game_set_constraint");
  g->contrainte[j + g->col * i] = n;
  putAllStatus(g, i, j);
}

void game_set_color(game g, uint i, uint j, color c) {
  checkGame(g, i, j, "game_set_color");
  g->couleur[j + g->col * i] = c;
  putAllStatus(g, i, j);
}

constraint game_get_constraint(cgame g, uint i, uint j) {
  checkGame((game)g, i, j, "game_get_constraint");
  return g->contrainte[j + g->col * i];
}

color game_get_color(cgame g, uint i, uint j) {
  checkGame((game)g, i, j, "game_get_color");
  return g->couleur[j + g->col * i];
}

bool game_get_next_square(cgame g, uint i, uint j, direction dir, uint *pi_next, uint *pj_next) {
  checkGame((game)g, i, j, "game_get_next_square");

  // case of non-wrapping
  if (g->wrapping == false) {
    // cases of the sides
    if ((j == g->col - 1 && ((dir == RIGHT) || (dir == UP_RIGHT) || (dir == DOWN_RIGHT))) ||
        (i == g->row - 1 && ((dir == DOWN) || (dir == DOWN_RIGHT) || (dir == DOWN_LEFT))) ||
        (j == 0 && ((dir == LEFT) || (dir == DOWN_LEFT) || (dir == UP_LEFT))) ||
        (i == 0 && ((dir == UP) || (dir == UP_RIGHT) || (dir == UP_LEFT)))) {
      return false;
    }
  }

  // changes the output values of pi_next and pj_next to match the direction
    switch (dir) {
      case UP:
        *pi_next = (i+game_nb_rows(g) - 1)%game_nb_rows(g);
        *pj_next = j;
        break;
      case DOWN:
        *pi_next = (i+  game_nb_rows(g) + 1)%game_nb_rows(g);
        *pj_next = j;
        break;
      case LEFT:
        *pi_next = i;
        *pj_next = (j+game_nb_cols(g) - 1)%game_nb_cols(g);
        break;
      case RIGHT:
        *pi_next = i;
        *pj_next = (j +game_nb_cols(g)+ 1)%game_nb_cols(g);
        break;
      case UP_LEFT:
        *pi_next = (i +game_nb_rows(g) - 1)%game_nb_rows(g);
        *pj_next = (j +game_nb_cols(g) - 1)%game_nb_cols(g);
        break;
      case UP_RIGHT:
        *pi_next = (i+game_nb_rows(g) - 1)%game_nb_rows(g);
        *pj_next = (j +game_nb_cols(g)+ 1)%game_nb_cols(g);
        break;
      case DOWN_LEFT:
        *pi_next = (i+game_nb_rows(g) + 1)%game_nb_rows(g);
        *pj_next = (j +game_nb_cols(g)- 1)%game_nb_cols(g);
        break;
      case DOWN_RIGHT:
        *pi_next = (i+ game_nb_rows(g) + 1)%game_nb_rows(g);
        *pj_next = (j+game_nb_cols(g) + 1)%game_nb_cols(g);
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
  return g->statut[j + g->col * i];
}

int game_nb_neighbors(cgame g, uint i, uint j, color c) {
  checkGame((game)g, i, j, "game_nb_neighbors");
  uint count = 0;
  uint pi_next = 0;
  uint pj_next = 0;
  // checks all around the square chosen the number of neighbors with the same color as c

  switch (g->neighbour) {
    
    case FULL:
      for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
        if(check_game_nb_neighbors_aux((game)g, i, j, dir, pi_next, pj_next, c)){
          count++;
        }
      }
    break;

    case FULL_EXCLUDE:
      for (direction dir = UP; dir <= DOWN_RIGHT; dir++) {
        if(check_game_nb_neighbors_aux((game)g, i, j, dir, pi_next, pj_next, c)){
          count++;
        }
      }
    break;

    case ORTHO:
      for (direction dir = HERE; dir <= RIGHT; dir++) {
        if(check_game_nb_neighbors_aux((game)g, i, j, dir, pi_next, pj_next, c)){
          count++;
        }
      }
    break;

    case ORTHO_EXCLUDE:
      for (direction dir = UP; dir <= RIGHT; dir++) {
        if(check_game_nb_neighbors_aux((game)g, i, j, dir, pi_next, pj_next, c)){
          count++;
        }
      }
    break;

    default:
      break;

  }

  return count;
}

void game_play_move(game g, uint i, uint j, color c) {
  checkGame(g, i, j, "game_play_move");
  move *m = malloc(sizeof(move));  // penser à free move
  if (m == NULL) {
    exit(EXIT_FAILURE);
  }
  m->i = i;
  m->j = j;
  m->c = game_get_color(g, i, j);
  queue_push_head(g->queue, m);
  queue_clear_full(g->queue_saves, free);
  game_set_color(g, i, j, c);
}

bool game_won(cgame g) {
  checkGame((game)g, 0, 0, "game_won");
  // checks if all of the squares are colored at satisfied
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
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
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
      game_set_color(g, i, j, EMPTY);
    }
  }
  // changes all of the status of the squares
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
      putStatus(g, i, j);
    }
  }
  queue_clear_full(g->queue, free);
  queue_clear_full(g->queue_saves, free);
}
