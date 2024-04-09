#include "game_tools.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_aux.h"


void putStatus(game g, uint i, uint j);

bool checkStatus(game g, uint i, uint j) {
  uint pi_next = 0;
  uint pj_next = 0;
  if (game_get_neighbourhood(g) == FULL) {
    for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
      if (game_get_next_square(g, i, j, dir, &pi_next, &pj_next) == true) {
        if (game_get_status(g, pi_next, pj_next) == ERROR) {
          return true;
        }
      }
    }
  }
  if (game_get_neighbourhood(g) == FULL_EXCLUDE) {
    for (direction dir = UP; dir <= DOWN_RIGHT; dir++) {
      if (game_get_next_square(g, i, j, dir, &pi_next, &pj_next) == true) {
        if (game_get_status(g, pi_next, pj_next) == ERROR) {
          return true;
        }
      }
    }
  }
  if (game_get_neighbourhood(g) == ORTHO) {
    for (direction dir = HERE; dir <= RIGHT; dir++) {
      if (game_get_next_square(g, i, j, dir, &pi_next, &pj_next) == true) {
        if (game_get_status(g, pi_next, pj_next) == ERROR) {
          return true;
        }
      }
    }
  }
  if (game_get_neighbourhood(g) == ORTHO_EXCLUDE) {
    for (direction dir = UP; dir <= RIGHT; dir++) {
      if (game_get_next_square(g, i, j, dir, &pi_next, &pj_next) == true) {
        if (game_get_status(g, pi_next, pj_next) == ERROR) {
          return true;
        }
      }
    }
  }
  return false;
}

void startGameSolver(game g3, uint i, uint j, bool gridPlayed[game_nb_rows(g3)][game_nb_cols(g3)], color c) {
  uint pi_next = 0;
  uint pj_next = 0;
  if (game_get_neighbourhood(g3) == FULL) {
    for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
      if (game_get_next_square(g3, i, j, dir, &pi_next, &pj_next) == true) {
        game_set_color(g3, pi_next, pj_next, c);
        gridPlayed[pi_next][pj_next] = true;
      }
    }
  }
  if (game_get_neighbourhood(g3) == FULL_EXCLUDE) {
    for (direction dir = UP; dir <= DOWN_RIGHT; dir++) {
      if (game_get_next_square(g3, i, j, dir, &pi_next, &pj_next) == true) {
        game_set_color(g3, pi_next, pj_next, c);
        gridPlayed[pi_next][pj_next] = true;
      }
    }
  }
  if (game_get_neighbourhood(g3) == ORTHO_EXCLUDE) {
    for (direction dir = HERE; dir <= RIGHT; dir++) {
      if (game_get_next_square(g3, i, j, dir, &pi_next, &pj_next) == true) {
        game_set_color(g3, pi_next, pj_next, c);
        gridPlayed[pi_next][pj_next] = true;
      }
    }
  }
  if (game_get_neighbourhood(g3) == ORTHO_EXCLUDE) {
    for (direction dir = UP; dir <= RIGHT; dir++) {
      if (game_get_next_square(g3, i, j, dir, &pi_next, &pj_next) == true) {
        game_set_color(g3, pi_next, pj_next, c);
        gridPlayed[pi_next][pj_next] = true;
      }
    }
  }
}

game game_load(char *filename) {
  FILE *save = fopen(filename, "r");
  if (save == NULL) {
    exit(EXIT_FAILURE);
  }
  uint row = 0;
  uint col = 0;
  uint wrapping = 0;
  uint neighbour = 0;

  // we haven't reach the end of the file yet.

  // first line of the file.

  int scanrow = fscanf(save, " %d", &row);
  if (scanrow == EOF) {
    exit(EXIT_FAILURE);
  }
  int scancol = fscanf(save, " %d", &col);
  if (scancol == EOF) {
    exit(EXIT_FAILURE);
  }
  int scanwrapping = fscanf(save, " %d", &wrapping);
  if (scanwrapping == EOF) {
    exit(EXIT_FAILURE);
  }
  int scanneighbour = fscanf(save, " %d", &neighbour);
  if (scanneighbour == EOF) {
    exit(EXIT_FAILURE);
  }
  fgetc(save);
  if (row > 10 || col > 10) {
    fprintf(stderr, "rows or cols too big");
    exit(EXIT_FAILURE);
  }

  constraint constraints1[row * col];
  color colors1[row * col];
  // second line of the file.

  for (int x = 0; x < row; x++) {
    for (int y = 0; y < col; y++) {
      char test = fgetc(save);

      if (test == '-') {
        constraints1[y + col * x] = UNCONSTRAINED;
      } else {
        constraints1[y + col * x] = test - '0';
      }
      char test2 = fgetc(save);
      if (test2 == 'e') {
        colors1[y + col * x] = EMPTY;
      } else if (test2 == 'w') {
        colors1[y + col * x] = WHITE;
      } else if (test2 == 'b') {
        colors1[y + col * x] = BLACK;
      }
    }
    fgetc(save);
  }
  fclose(save);
  return game_new_ext(row, col, constraints1, colors1, wrapping, neighbour);
}

void game_save(cgame g, char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    exit(EXIT_FAILURE);
  }
  fprintf(file, "%d %d %d %d\n", g->row, g->col, g->wrapping, g->neighbour);
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
      if (game_get_constraint(g, i, j) == UNCONSTRAINED) {
        fprintf(file, "-");
      } else {
        fprintf(file, "%d", game_get_constraint(g, i, j));
      }
      if (game_get_color(g, i, j) == EMPTY) {
        fprintf(file, "e");
      } else if (game_get_color(g, i, j) == WHITE) {
        fprintf(file, "w");
      } else {
        fprintf(file, "b");
      }
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

void first_solver(game g, game g3, uint pos, uint len, uint *count, bool *solve, bool *found) {
  if (!*found) {
    if (pos == len) {
      if (game_won(g3)) {
        if ((*count) == 0) {
          for (int i = 0; i < game_nb_rows(g3); i++) {
            for (int j = 0; j < game_nb_cols(g3); j++) {
              uint colorGame = game_get_color(g3, i, j);
              game_play_move(g, i, j, colorGame);
              if (*solve) {
                *found = true;
              }
            }
          }
        }
        (*count)++;
      }
      return;
    }
    // extend current word recursively with value 0 at position pos

    game_play_move(g3, (pos / game_nb_cols(g)), (pos % game_nb_cols(g)), WHITE);
    first_solver(g, g3, pos + 1, len, count, solve, found);

    // extend current word recursively with value 1 at position pos

    game_play_move(g3, (pos / game_nb_cols(g)), (pos % game_nb_cols(g)), BLACK);
    first_solver(g, g3, pos + 1, len, count, solve, found);
  }
}

void first_solver2(game g, game g3, uint pos, uint len, uint *count, bool *solve, bool *found, bool *start,
                   bool gridPlayed[game_nb_rows(g3)][game_nb_cols(g3)]) {
  bool error = false;
  /*if (*start) {
    *start = false;
    for (int i = 0; i < game_nb_rows(g3); i++) {
      for (int j = 0; j < game_nb_cols(g3); j++) {
        if (game_get_constraint(g3, i, j) == 0 && (game_get_color(g3, i, j) ==
  EMPTY)) { startGameSolver(g3, i, j, gridPlayed, WHITE);
        }
        if (game_get_constraint(g3, i, j) == game_nb_neighbors(g3, i, j, EMPTY)
  + game_nb_neighbors(g3, i, j, BLACK) + game_nb_neighbors(g3, i, j, WHITE) &&
            (game_get_color(g3, i, j) == EMPTY)) {
          startGameSolver(g3, i, j, gridPlayed, BLACK);
        }
        if (gridPlayed[i][j] != true) {
          gridPlayed[i][j] = false;
        }
      }
    }
  }*/
  if (!*found) {
    if (pos == len) {
      if (game_won(g3)) {
        if ((*count) == 0) {
          for (int i = 0; i < game_nb_rows(g3); i++) {
            for (int j = 0; j < game_nb_cols(g3); j++) {
              uint colorGame = game_get_color(g3, i, j);
              game_set_color(g, i, j, colorGame);
              if (*solve) {
                *found = true;
              }
            }
          }
        }
        (*count)++;
      }
      return;
    }

    // extend current word recursively with value 0 at position pos
    uint i = pos / game_nb_cols(g);
    uint j = pos % game_nb_cols(g);
    // if (gridPlayed[i][j] == false) {
    game_set_color(g3, i, j, WHITE);
    error = checkStatus(g3, i, j);
    //}
    if (!error) {
      first_solver2(g, g3, pos + 1, len, count, solve, found, start, gridPlayed);
    }
    game_set_color(g3, i, j, EMPTY);
    error = false;
    // extend current word recursively with value 1 at position pos
    // if (gridPlayed[i][j] == false) {
    game_set_color(g3, i, j, BLACK);
    error = checkStatus(g, i, j);
    //}
    if (!error) {
      first_solver2(g, g3, pos + 1, len, count, solve, found, start, gridPlayed);
    }
    game_set_color(g3, i, j, EMPTY);
    error = false;
  }
}

bool game_solve(game g) {
  // Initialization
  bool solve = true;
  bool found = false;
  uint nbwords = 0;
  game g3 = game_copy(g);
  bool start = true;
  bool gridPlayed[game_nb_rows(g3)][game_nb_cols(g3)];
  first_solver2(g, g3, 0, game_nb_cols(g) * game_nb_rows(g), &nbwords, &solve, &found, &start, gridPlayed);
  game_delete(g3);
  game_print(g);
  return (nbwords == 1);
}

uint game_nb_solutions(cgame g) {
  bool solve = false;
  bool found = false;
  uint nbwords = 0;
  game gcopy = game_copy(g);
  bool start = true;
  bool gridPlayed[game_nb_rows(gcopy)][game_nb_cols(gcopy)];
  first_solver2(gcopy, gcopy, 0, game_nb_cols(g) * game_nb_rows(g), &nbwords, &solve, &found, &start, gridPlayed);

  game_delete(gcopy);
  return nbwords;
}

game game_random(uint nb_rows, uint nb_cols, bool wrapping, neighbourhood neigh,
                 bool with_solution, float black_rate, float constraint_rate)
{
  assert(black_rate >= 0.0f && black_rate <= 1.0f);
  assert(constraint_rate >= 0.0f && constraint_rate <= 1.0f);
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, neigh);
  assert(g);

  // fill the grid with random colors
  for (uint i = 0; i < nb_rows; i++) {
    for (uint j = 0; j < nb_cols; j++) {
      color c = (rand() < black_rate * (float)RAND_MAX) ? BLACK : WHITE;
      game_set_color(g, i, j, c);
    }
  }

  // fill the grid with actual constraint at random positions
  uint nb_squares = nb_rows * nb_cols;
  uint nb_constraints = constraint_rate * nb_squares;
  for (uint i = 0; i < nb_constraints; i++) {
    uint row = rand() % nb_rows;
    uint col = rand() % nb_cols;
    int nb_blacks = game_nb_neighbors(g, row, col, BLACK);
    game_set_constraint(g, row, col, nb_blacks);
  }

  // check solution
  if (!game_won(g)) {
    game_delete(g);
    return NULL;
  }

  if (!with_solution) game_restart(g);
  return g;
}