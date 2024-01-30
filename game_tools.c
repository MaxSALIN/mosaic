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
    return EXIT_FAILURE;
  }
  game g;
  int rows, cols = 0;

  // we haven't reach the end of the file yet.
  while (!feof(save)) {
    // first line of the file.

    g->row = fgets("<nb_rows>", 10, save);
    rows = fgets("<nb_rows>", 10, save);
    g->col = fgetst("<nb_cols>", 10, save);
    cols = fgetst("<nb_cols>", 10, save);

    g->wrapping = fgets("<wrapping>", 1, save);
    g->neighbour = fgets("<neigh>", 1, save);

    // second line of the file.

    for (int x = 0; x <= rows; x++) {
      for (int y = 0; y <= cols; y++) {
        // particular situation where the constraint is none.
        if (strstr(fgets("<squares[" + x + "][" + y + "]", 1, save), "-") != NULL) {
          g->contrainte[x][y] = UNCONSTRAINED;
        } else if (fgets("<squares[" + x + "][" + y + "]", 1, save) >= 0 &&
                   fgets("<squares[" + x + "][" + y + "]", 1, save) <= 9) {
          g->contrainte[x][y] = (int)fgets("<squares[" + x + "][" + y + "]", 1, save);
        }

        if (strstr(fgets("<squares[" + x + "][" + y + "]", 1, save), "e") != NULL) {
          g->couleur[x][y] = EMPTY;
        } else if (strstr(fgets("<squares[" + x + "][" + y + "]", 1, save), "w") != NULL) {
          g->couleur[x][y] = WHITE;
        } else if (strstr(fgets("<squares[" + x + "][" + y + "]", 1, save), "b") != NULL) {
          g->couleur[x][y] = BLACK;
        }
      }
    }
  }
  fclose(save);

  return g;
}

void game_save(cgame g, char *filename) {
  FILE *file = fopen(filename, "w");
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
