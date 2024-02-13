#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

bool test_dummy() { return true; }

bool test_set_color() {
  game g = game_default();
  game_set_color(g, 1, 1, BLACK);
  game_set_color(g, 2, 2, WHITE);
  game_set_color(g, 0, 3, BLACK);
  if (game_get_color(g, 0, 3) != BLACK || game_get_constraint(g, 0, 3) != 3) {
    game_delete(g);
    return false;
  }
  game_set_color(g, 0, 3, EMPTY);
  if (game_get_color(g, 1, 1) != BLACK || game_get_color(g, 2, 2) != WHITE || game_get_color(g, 0, 3) != EMPTY) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_play_move() {
  game g = game_new_empty();
  game_set_constraint(g, 1, 1, 1);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == 1 && j == 1) {
        game_play_move(g, i, j, BLACK);
      } else {
        game_play_move(g, i, j, WHITE);
      }
    }
  }
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (i == 1 && j == 1) {
        if (game_get_color(g, i, j) != BLACK || game_get_status(g, i, j) != SATISFIED) {
          game_delete(g);
          return false;
        }
      } else {
        if (game_get_color(g, i, j) != WHITE) {
          game_delete(g);
          return false;
        }
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_restart() {
  game g = game_default();
  game_play_move(g, 1, 1, BLACK);
  game_restart(g);
  if (game_get_color(g, 1, 1) == BLACK) {
    game_delete(g);
    return false;
  }
  game_play_move(g, 2, 2, WHITE);
  game_restart(g);
  if (game_get_color(g, 2, 2) == WHITE) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_equal() {
  game g1 = game_default();
  game g2 = game_default();
  game_play_move(g1, 1, 1, BLACK);
  game_play_move(g2, 1, 1, BLACK);
  if (!game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_play_move(g2, 1, 1, WHITE);
  if (game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_copy() {
  game g1 = game_new_empty_ext(7, 6, true, ORTHO_EXCLUDE);
  game_play_move(g1, 1, 1, BLACK);
  game g2 = game_copy(g1);
  for (int i = 0; i < game_nb_rows(g1); i++) {
    for (int j = 0; j < game_nb_cols(g2); j++) {
      if (game_get_constraint(g1, i, j) != game_get_constraint(g2, i, j) ||
          game_get_status(g1, i, j) != game_get_status(g2, i, j) ||
          game_get_color(g1, i, j) != game_get_color(g2, i, j) || game_nb_cols(g1) != game_nb_cols(g2) ||
          game_nb_rows(g1) != game_nb_rows(g2) || game_is_wrapping(g1) != game_is_wrapping(g2) ||
          game_get_neighbourhood(g1) != game_get_neighbourhood(g2)) {
        game_delete(g1);
        game_delete(g2);
        return false;
      }
    }
  }

  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_set_constraint() {
  game g_default = game_default();
  game_set_color(g_default, 0, 3, BLACK);
  if (game_get_color(g_default, 0, 3) != BLACK || game_get_constraint(g_default, 0, 3) != 3) {
    game_delete(g_default);
    return false;
  }
  game_set_constraint(g_default, 0, 3, 8);
  if (game_get_color(g_default, 0, 3) != BLACK || game_get_constraint(g_default, 0, 3) != 8) {
    game_delete(g_default);
    return false;
  }
  game_delete(g_default);
  game g = game_new_empty();
  for (int y = 0; y < DEFAULT_SIZE; y++) {
    for (int x = 0; x < DEFAULT_SIZE; x++) {
      game_set_constraint(g, x, y, 0);
      game_play_move(g, x, y, WHITE);
    }
  }
  for (int y = 0; y < DEFAULT_SIZE; y++) {
    for (int x = 0; x < DEFAULT_SIZE; x++) {
      if (game_get_status(g, x, y) != SATISFIED) {
        game_delete(g);
        return false;
      }
    }
  }
  game g2 = game_new_empty();
  game_set_constraint(g2, 3, 3, 9);
  for (int x = 1; x < DEFAULT_SIZE - 1; x++) {
    for (int y = 1; y < DEFAULT_SIZE - 1; y++) {
      game_set_color(g2, x, y, BLACK);
    }
  }
  game_set_constraint(g2, 3, 3, 0);
  if (game_get_status(g2, 3, 3) == SATISFIED) {
    game_delete(g);
    game_delete(g2);
    return false;
  }

  game g3 = game_new_empty();
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_set_constraint(g3, x, y, -1);
    }
  }
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_play_move(g3, x, y, BLACK);
    }
  }
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      if (game_get_constraint(g3, x, y) != -1 || game_get_status(g3, x, y) != SATISFIED) {
        game_delete(g);
        game_delete(g2);
        game_delete(g3);
        return false;
      }
    }
  }
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_set_color(g3, x, y, WHITE);
    }
  }
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      if (game_get_constraint(g3, x, y) != -1 || game_get_status(g3, x, y) != SATISFIED) {
        game_delete(g);
        game_delete(g2);
        game_delete(g3);
        return false;
      }
    }
  }
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_set_color(g3, x, y, WHITE);
    }
  }
  for (int x = 0; x < DEFAULT_SIZE; x = x + 2) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      if (game_get_constraint(g3, x, y) != -1 || game_get_status(g3, x, y) != SATISFIED) {
        game_delete(g);
        game_delete(g2);
        game_delete(g3);
        return false;
      }
    }
  }
  game_delete(g);
  game_delete(g2);
  game_delete(g3);
  return true;
}

bool test_game_load_save() {
  game g_default = game_default();
  game_play_move(g_default, 2, 2, BLACK);
  game_play_move(g_default, 1, 1, WHITE);
  game_save(g_default, "defaultTest.txt");
  game g_load = game_load("defaultTest.txt");
  bool equal = game_equal(g_default, g_load);
  game_delete(g_default);
  game_delete(g_load);

  constraint constrainte[28];
  color colore[28];
  for(int i=0; i<28; i++){
    constrainte[i]=UNCONSTRAINED;
    colore[i]=EMPTY;
  }
  constrainte[4]=5;
  constrainte[26]=3;
  colore[21]=BLACK;
  game g2=game_new_ext(4, 7, constrainte, colore, true, ORTHO_EXCLUDE);
  game_save(g2, "defaultTest2.txt");
  game g_load2 = game_load("defaultTest2.txt");
  bool equal2 = game_equal(g2, g_load2);
  game_delete(g2);
  game_delete(g_load2);

  return (equal && equal2);
}

int test_toccelli(char *argv[]) {
  bool ok = false;
  if (!(strcmp("dummy", argv[1]))) {
    ok = test_dummy();
  } else if (!(strcmp("set_color", argv[1]))) {
    ok = test_set_color();
  } else if (!(strcmp("play_move", argv[1]))) {
    ok = test_play_move();
  } else if (!(strcmp("restart", argv[1]))) {
    ok = test_restart();
  } else if (!(strcmp("equal", argv[1]))) {
    ok = test_equal();
  } else if (!(strcmp("copy", argv[1]))) {
    ok = test_copy();
  } else if (!(strcmp("set_constraint", argv[1]))) {
    ok = test_set_constraint();
  } else if (!(strcmp("game_load_save", argv[1]))) {
    ok = test_game_load_save();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}

/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  return test_toccelli(argv);
}