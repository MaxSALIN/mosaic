#include "game_tools.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"
#include "game_struct.h"

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

  fscanf(save, " %d", &row);
  fscanf(save, " %d", &col);
  fscanf(save, " %d", &wrapping);
  fscanf(save, " %d", &neighbour);
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

void first_solver(game g, game g3, uint pos, uint len, uint *word, uint *count) {
  if (pos == len) {
    if (game_won(g3)) {
      if ((*count) == 0) {
        for (int i = 0; i < game_nb_rows(g3); i++) {
          for (int j = 0; j < game_nb_cols(g3); j++) {
            uint colorGame = game_get_color(g3, i, j);
            game_play_move(g, i, j, colorGame);
          }
        }
      }
      (*count)++;
    }
    return;
  }
  // extend current word recursively with value 0 at position pos
  word[pos] = 0;
  game_play_move(g3, pos / game_nb_cols(g), pos % game_nb_cols(g), WHITE);
  first_solver(g, g3, pos + 1, len, word, count);

  // extend current word recursively with value 1 at position pos
  word[pos] = 1;
  game_play_move(g3, pos / game_nb_cols(g), pos % game_nb_cols(g), BLACK);
  first_solver(g, g3, pos + 1, len, word, count);
}

bool game_solve(game g) {
  // Initialization
  uint word[game_nb_cols(g) * game_nb_rows(g)];
  uint nbwords = 0;
  for (int k = 0; k < game_nb_cols(g) * game_nb_rows(g); k++) {
    word[k] = 0;
  }
  game g2 = game_copy(g);
  game g3 = game_copy(g);
  first_solver(g, g3, 0, game_nb_cols(g) * game_nb_rows(g), word, &nbwords);
  if (nbwords == 0) {
    g = g2;
    game_delete(g2);
    game_delete(g3);
    game_print(g);
    return false;
  } else {
    game_delete(g2);
    game_delete(g3);
    game_print(g);
    return true;
  }
}

uint game_nb_solutions(cgame g) {
  uint word[game_nb_cols(g) * game_nb_rows(g)];
  uint nbwords = 0;
  for (int k = 0; k < game_nb_cols(g) * game_nb_rows(g); k++) {
    word[k] = 0;
  }
  game gcopy = game_copy(g);
  first_solver(gcopy, gcopy, 0, game_nb_cols(g) * game_nb_rows(g), word, &nbwords);
  game_delete(gcopy);
  return nbwords;
}