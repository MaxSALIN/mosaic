// include library and .h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"

// functions
bool test_dummy(void);
bool test_new_empty(void);
bool test_get_color(void);
bool test_get_constraint(void);
bool test_delete(void);
bool test_new(void);
bool test_default_solution(void);
void usage(int argc, char *argv[]);

// coding of functions
void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

bool test_dummy(void) { return true; }

bool test_new_empty(void) {
  game g = game_new_empty();
  if (g == NULL) {
    return false;
  }
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((game_get_color(g, i, j) != EMPTY) || (game_get_constraint(g, i, j) != -1)) {
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_get_color(void) {
  game g = game_default();
  if (game_get_color(g, 0, 0) != EMPTY && game_get_color(g, 0, 0) != BLACK && game_get_color(g, 0, 0) != WHITE) {
    return false;
  }

  game_set_color(g, 1, 1, BLACK);
  if (game_get_color(g, 1, 1) != BLACK) {
    return false;
  }
  game_set_color(g, 2, 2, WHITE);
  if (game_get_color(g, 2, 2) != WHITE) {
    return false;
  }
  game_set_color(g, 3, 3, EMPTY);
  if (game_get_color(g, 3, 3) != EMPTY) {
    return false;
  }
  game_delete(g);
  return true;
}

bool test_get_constraint(void) {
  game g = game_default();
  if (game_get_constraint(g, 0, 0) > 9 || game_get_constraint(g, 0, 0) < -1) {
    return false;
  }
  game_set_constraint(g, 1, 1, UNCONSTRAINED);
  if (game_get_constraint(g, 1, 1) != UNCONSTRAINED) {
    return false;
  }
  for (int i = 0; i <= 9; i++) {
    game_set_constraint(g, 2, 2, i);
    if (game_get_constraint(g, 2, 2) != i) {
      return false;
    }
  }
  game_delete(g);
  return true;
}

bool test_delete(void) {
  game g = game_default();
  game_delete(g);
  return true;
}

bool test_new(void) {
  int k = 0;
  constraint constraints1[DEFAULT_SIZE * DEFAULT_SIZE];
  for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    constraints1[i] = -1;
  }
  game g_Test = game_new(constraints1, NULL);
  if (g_Test == NULL) {
    return false;
  }

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((game_get_color(g_Test, i, j) != EMPTY) || (game_get_constraint(g_Test, i, j) != -1)) {
        return false;
      }
    }
  }

  game g_sol = game_default_solution();
  constraint constraints2[25];

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      constraints2[k] = game_get_constraint(g_sol, i, j);
      k++;
    }
  }

  k = 0;

  color colors[25];

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      colors[k] = game_get_color(g_sol, i, j);
      k++;
    }
  }

  game g_Test2 = game_new(constraints2, colors);
  if (g_Test2 == NULL) {
    return false;
  }

  for (int j = 0; j < DEFAULT_SIZE; j++) {
    for (int i = 0; i < DEFAULT_SIZE; i++) {
      if ((game_get_color(g_sol, i, j) != (game_get_color(g_Test2, i, j))) ||
          (game_get_constraint(g_sol, i, j) != (game_get_constraint(g_Test2, i, j)))) {
        printf("x=%d, y=%d, %d %d %d %d", i, j, game_get_color(g_Test2, i, j), game_get_constraint(g_Test2, i, j),
               game_get_color(g_sol, i, j), game_get_constraint(g_sol, i, j));
        return false;
      }
    }
  }

  game_delete(g_Test);
  game_delete(g_Test2);
  game_delete(g_sol);
  return true;
}

bool test_default_solution(void) {
  game g = game_default_solution();
  if (g == NULL) {
    return false;
  }

  if (!game_won(g)) {
    return false;
  }
  game g_ref = game_default();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((game_get_constraint(g, i, j) != game_get_constraint(g_ref, i, j)) || game_get_status(g, i, j) != SATISFIED ||
          game_get_color(g, i, j) == EMPTY) {
        return false;
      }
    }
  }
  game_delete(g);
  game_delete(g_ref);
  return true;
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }
  bool result_test = false;

  // test dummy
  if (strcmp("dummy", argv[1]) == 0) {
    result_test = test_dummy();
  }

  // test new_empty
  else if (strcmp("new_empty", argv[1]) == 0) {
    result_test = test_new_empty();
  }

  // test get_color
  else if (strcmp("get_color", argv[1]) == 0) {
    result_test = test_get_color();
  }

  // test get_constraint
  else if (strcmp("get_constraint", argv[1]) == 0) {
    result_test = test_get_constraint();
  }

  // test delete
  else if (strcmp("delete", argv[1]) == 0) {
    result_test = test_delete();
  }

  // test new
  else if (strcmp("new", argv[1]) == 0) {
    result_test = test_new();
  }

  // test default_solution
  else if (strcmp("default_solution", argv[1]) == 0) {
    result_test = test_default_solution();
  }

  else {
    exit(EXIT_FAILURE);
  }

  // all tests works
  if (result_test) {
    fprintf(stderr, "0\n");
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "1\n");
    return EXIT_FAILURE;
  }
}