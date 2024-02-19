// include library and .h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"
#include "queue.h"

// functions
bool test_dummy(void);
bool test_new_empty(void);
bool test_get_color(void);
bool test_get_constraint(void);
bool test_delete(void);
bool test_new(void);
bool test_default_solution(void);
bool test_new_empty_ext(void);
bool test_new_ext(void);
bool test_undo_redo(void);
bool test_wrapping(void);
bool test_neigh(void);
bool test_row_col(void);
bool test_nb_neigh_new(void);
bool test_solve(void);
bool test_nb_solutions(void);
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
    game_delete(g);
    return false;
  }
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((game_get_color(g, i, j) != EMPTY) || (game_get_constraint(g, i, j) != -1)) {
        game_delete(g);
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
    game_delete(g);
    return false;
  }

  game_set_color(g, 1, 1, BLACK);
  if (game_get_color(g, 1, 1) != BLACK) {
    game_delete(g);
    return false;
  }
  game_set_color(g, 2, 2, WHITE);
  if (game_get_color(g, 2, 2) != WHITE) {
    game_delete(g);
    return false;
  }
  game_set_color(g, 3, 3, EMPTY);
  if (game_get_color(g, 3, 3) != EMPTY) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_get_constraint(void) {
  game g = game_default();
  if (game_get_constraint(g, 0, 0) > 9 || game_get_constraint(g, 0, 0) < -1) {
    game_delete(g);
    return false;
  }
  game_set_constraint(g, 1, 1, UNCONSTRAINED);
  if (game_get_constraint(g, 1, 1) != UNCONSTRAINED) {
    game_delete(g);
    return false;
  }
  for (int i = 0; i <= 9; i++) {
    game_set_constraint(g, 2, 2, i);
    if (game_get_constraint(g, 2, 2) != i) {
      game_delete(g);
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
    game_delete(g_Test);
    return false;
  }

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((game_get_color(g_Test, i, j) != EMPTY) || (game_get_constraint(g_Test, i, j) != -1)) {
        game_delete(g_Test);
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
    game_delete(g_Test);
    game_delete(g_Test2);
    game_delete(g_sol);
    return false;
  }

  for (int j = 0; j < DEFAULT_SIZE; j++) {
    for (int i = 0; i < DEFAULT_SIZE; i++) {
      if ((game_get_color(g_sol, i, j) != (game_get_color(g_Test2, i, j))) ||
          (game_get_constraint(g_sol, i, j) != (game_get_constraint(g_Test2, i, j)))) {
        game_delete(g_Test);
        game_delete(g_Test2);
        game_delete(g_sol);
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
    game_delete(g);
    return false;
  }

  if (!game_won(g)) {
    game_delete(g);
    return false;
  }
  game g_ref = game_default();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if ((game_get_constraint(g, i, j) != game_get_constraint(g_ref, i, j)) || game_get_status(g, i, j) != SATISFIED ||
          game_get_color(g, i, j) == EMPTY) {
        game_delete(g);
        game_delete(g_ref);
        return false;
      }
    }
  }
  game_delete(g);
  game_delete(g_ref);
  return true;
}

bool test_new_empty_ext(void) {
  for (int row = 1; row <= 10; row++) {
    for (int col = 1; col <= 10; col++) {
      game g = game_new_empty_ext(row, col, false, FULL);
      if (g == NULL) {
        game_delete(g);
        return false;
      }
      for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
          if ((game_get_color(g, i, j) != EMPTY) || (game_get_constraint(g, i, j) != -1)) {
            game_delete(g);
            return false;
          }
        }
      }
      game_delete(g);
    }
  }

  game g = game_new_empty_ext(8, 8, false, FULL);
  game_play_move(g, 4, 4, WHITE);
  game_play_move(g, 7, 4, BLACK);
  game g_copy = game_copy(g);
  if (game_get_color(g_copy, 4, 4) != WHITE) {
    game_delete(g_copy);
    game_delete(g);
    return false;
  }
  if (game_get_color(g_copy, 7, 4) != BLACK) {
    game_delete(g_copy);
    game_delete(g);
    return false;
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if ((i != 4 && j != 4) || (i != 7 && j != 4)) {
        if (game_get_color(g_copy, i, j) != EMPTY) {
          game_delete(g_copy);
          game_delete(g);
          return false;
        }
      }
    }
  }
  if (!game_equal(g, g_copy)) {
    game_delete(g_copy);
    game_delete(g);
    return false;
  }
  game_delete(g_copy);
  game_delete(g);
  return true;
}

bool test_new_ext(void) {
  for (int row = 1; row <= 10; row++) {
    for (int col = 1; col <= 10; col++) {
      constraint constraints1[row * col];
      for (int i = 0; i < row * col; i++) {
        constraints1[i] = -1;
      }
      for (int neigh = FULL; neigh <= ORTHO_EXCLUDE; neigh++) {
        game g_Test = game_new_ext(row, col, constraints1, NULL, true, neigh);
        if (g_Test == NULL) {
          game_delete(g_Test);
          return false;
        }
        for (int i = 0; i < row; i++) {
          for (int j = 0; j < col; j++) {
            if ((game_get_color(g_Test, i, j) != EMPTY) || (game_get_constraint(g_Test, i, j) != -1)) {
              game_delete(g_Test);
              return false;
            }
          }
        }
        game_delete(g_Test);
      }
    }
  }
  int row2 = 7;
  int col2 = 6;
  constraint constraints1[row2 * col2];
  color colors1[row2 * col2];
  for (int i = 0; i < row2 * col2; i++) {
    constraints1[i] = 0;
    colors1[i] = WHITE;
  }
  game g = game_new_ext(row2, col2, constraints1, colors1, true, FULL);

  for (int i = 0; i < row2; i++) {
    for (int j = 0; j < col2; j++) {
      if ((game_get_color(g, i, j) != WHITE) || (game_get_constraint(g, i, j) != 0) ||
          game_get_status(g, i, j) != SATISFIED) {
        game_print(g);
        game_delete(g);
        printf("test %d %d %d", i, j, game_get_status(g, 0, 0));
        return false;
      }
    }
  }
  game_play_move(g, row2 - 1, col2 - 1, BLACK);
  for (int i = 0; i < row2; i++) {
    for (int j = 0; j < col2; j++) {
      if ((i == 0 && j == 0) || (i == 0 && j > 3) || (i > 4 && j == 0)) {
        if (game_is_wrapping(g) == true) {
          if (game_get_status(g, i, j) != ERROR) {
            game_delete(g);
            return false;
          }
        }
      } else if (i > 4 && j > 3) {
        if (game_get_status(g, i, j) != ERROR) {
          game_delete(g);
          return false;
        }
      } else if (game_get_status(g, i, j) != SATISFIED) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_undo_redo(void) {
  game g = game_default();
  game g_copy1 = game_copy(g);
  game_play_move(g, 0, 0, BLACK);
  game g_copy2 = game_copy(g);
  game_play_move(g, 0, 0, WHITE);
  game g_copy3 = game_copy(g);
  game_undo(g);
  if (!game_equal(g, g_copy2) || game_get_color(g, 0, 0) != BLACK) {
    printf("Erreur premier Undo");
    game_delete(g);
    game_delete(g_copy1);
    game_delete(g_copy2);
    game_delete(g_copy3);
    return false;
  }
  game_undo(g);
  if (!game_equal(g, g_copy1)) {
    printf("Erreur deuxième Undo");
    game_delete(g);
    game_delete(g_copy1);
    game_delete(g_copy2);
    game_delete(g_copy3);
    return false;
  }
  game_redo(g);
  if (!game_equal(g, g_copy2)) {
    printf("Erreur premier Redo");
    game_delete(g);
    game_delete(g_copy1);
    game_delete(g_copy2);
    game_delete(g_copy3);
    return false;
  }
  game_redo(g);
  if (!game_equal(g, g_copy3)) {
    printf("Erreur deuxième Redo");
    game_delete(g);
    game_delete(g_copy1);
    game_delete(g_copy2);
    game_delete(g_copy3);
    return false;
  }
  game_undo(g);
  game_play_move(g, 4, 4, WHITE);
  game g_copyF = game_copy(g);
  game_redo(g);
  if (!game_equal(g, g_copyF)) {
    printf("Erreur dernier Undo/Redo");
    game_delete(g);
    game_delete(g_copy1);
    game_delete(g_copy2);
    game_delete(g_copy3);
    game_delete(g_copyF);
    return false;
  }
  game_restart(g);
  game g_copy4 = game_copy(g);
  game_play_move(g, 4, 4, WHITE);
  game_play_move(g, 4, 3, WHITE);
  game_restart(g);
  game_undo(g);
  if (!game_equal(g, g_copy4)) {
    printf("Erreur restart");
    game_delete(g);
    game_delete(g_copy1);
    game_delete(g_copy2);
    game_delete(g_copy3);
    game_delete(g_copy4);
    game_delete(g_copyF);
    return false;
  }
  game_delete(g);
  game_delete(g_copy1);
  game_delete(g_copy2);
  game_delete(g_copy3);
  game_delete(g_copy4);
  game_delete(g_copyF);
  return true;
}

bool test_wrapping(void) {
  game g = game_new_empty_ext(6, 6, true, FULL);
  game_set_constraint(g, 5, 5, 0);
  game_play_move(g, 0, 0, BLACK);
  if (game_get_status(g, 5, 5) != ERROR) {
    game_delete(g);
    printf("test");
    printf("tEST 2 %d", (-1) % 6);
    return false;
  }
  game_play_move(g, 0, 0, WHITE);
  game_play_move(g, 0, 4, BLACK);
  if (game_get_status(g, 5, 5) != ERROR) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_neigh(void) {
  uint pi_next = 0;
  uint pj_next = 0;
  // FULL
  game g = game_new_empty_ext(5, 5, false, FULL);
  game_set_constraint(g, 3, 3, 9);
  if (game_get_status(g, 0, 0) != UNSATISFIED) {
    game_delete(g);
    return false;
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      game_play_move(g, i, j, WHITE);
    }
  }
  if (game_get_status(g, 0, 0) != SATISFIED) {
    game_delete(g);
    return false;
  }
  for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
    if (game_get_next_square(g, 3, 3, dir, &pi_next, &pj_next) == true) {
      game_play_move(g, pi_next, pj_next, BLACK);
    }
  }
  if (game_get_status(g, 3, 3) != SATISFIED) {
    printf("Error in FULL");
    game_delete(g);
    return false;
  }
  if (!game_won(g)) {
    printf("Error in FULL (won)");
    game_delete(g);
    return false;
  }
  game_delete(g);
  // FULL_EXCLUDE
  game g2 = game_new_empty_ext(5, 5, false, FULL_EXCLUDE);
  game_set_constraint(g2, 3, 3, 8);
  if (game_get_status(g2, 0, 0) != UNSATISFIED) {
    game_delete(g2);
    return false;
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      game_play_move(g2, i, j, WHITE);
    }
  }
  if (game_get_status(g2, 0, 0) != SATISFIED) {
    game_delete(g2);
    return false;
  }
  for (direction dir = UP; dir <= DOWN_RIGHT; dir++) {
    if (game_get_next_square(g2, 3, 3, dir, &pi_next, &pj_next) == true) {
      game_play_move(g2, pi_next, pj_next, BLACK);
    }
  }
  if (game_get_status(g2, 3, 3) != SATISFIED) {
    printf("Error in FULL_EXCLUDE");
    game_delete(g2);
    return false;
  }
  if (!game_won(g2)) {
    printf("Error in FULL_EXCLUDE (won)");
    game_delete(g2);
    return false;
  }
  game_play_move(g2, 2, 2, WHITE);
  if (game_get_status(g2, 3, 3) != ERROR) {
    printf("Error in FULL_EXCLUDE");
    game_delete(g2);
    return false;
  }
  game_delete(g2);

  // ORTHO
  game g3 = game_new_empty_ext(5, 5, false, ORTHO);
  game_set_constraint(g3, 3, 3, 5);
  if (game_get_status(g3, 0, 0) != UNSATISFIED) {
    game_delete(g3);
    return false;
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      game_play_move(g3, i, j, WHITE);
    }
  }
  if (game_get_status(g3, 0, 0) != SATISFIED) {
    game_delete(g3);
    return false;
  }
  for (direction dir = HERE; dir < UP_LEFT; dir++) {
    if (game_get_next_square(g3, 3, 3, dir, &pi_next, &pj_next) == true) {
      game_play_move(g3, pi_next, pj_next, BLACK);
    }
  }
  if (game_get_status(g3, 3, 3) != SATISFIED) {
    printf("Error in ORTHO");
    game_delete(g3);
    return false;
  }
  if (!game_won(g3)) {
    printf("Error in ORTHO (won)");
    game_delete(g3);
    return false;
  }
  game_play_move(g3, 2, 3, WHITE);
  if (game_get_status(g3, 3, 3) != ERROR) {
    printf("Error in ORTHO");
    game_delete(g3);
    return false;
  }
  game_delete(g3);

  // ORTHO_EXCLUDE
  game g4 = game_new_empty_ext(5, 5, false, ORTHO_EXCLUDE);
  game_set_constraint(g4, 3, 3, 4);
  if (game_get_status(g4, 0, 0) != UNSATISFIED) {
    game_delete(g);
    return false;
  }
  // plays white in every square of the game
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      game_play_move(g4, i, j, WHITE);
    }
  }
  if (game_get_status(g4, 0, 0) != SATISFIED) {
    game_delete(g);
    return false;
  }
  for (direction dir = UP; dir < UP_LEFT; dir++) {
    if (game_get_next_square(g4, 3, 3, dir, &pi_next, &pj_next) == true) {
      game_play_move(g4, pi_next, pj_next, BLACK);
    }
  }
  if (game_get_status(g4, 3, 3) != SATISFIED) {
    printf("Error in ORTHO_EXCLUDE");
    game_delete(g4);
    return false;
  }
  if (!game_won(g4)) {
    printf("Error in ORTHO_EXCLUDE(won)");
    game_delete(g4);
    return false;
  }
  game_play_move(g4, 2, 3, WHITE);
  if (game_get_status(g4, 3, 3) != ERROR) {
    printf("Error in ORTHO_EXCLUDE");
    game_delete(g4);
    return false;
  }
  game_delete(g4);
  return true;
}

bool test_row_col(void) {
  game g = game_new_empty_ext(7, 7, false, FULL);
  if (game_nb_rows(g) != 7 || game_nb_cols(g) != 7) {
    return false;
  }
  game g2 = game_default();
  if (game_nb_rows(g2) != 5 || game_nb_cols(g2) != 5) {
    return false;
  }
  game_delete(g);
  game_delete(g2);
  return true;
}

bool test_nb_neigh_new(void) {
  // For FULL
  game g = game_new_empty_ext(8, 7, true, FULL);
  if (game_get_neighbourhood(g) != FULL) {
    game_delete(g);
    return false;
  }
  game_set_constraint(g, 2, 6, 4);
  game_play_move(g, 1, 6, BLACK);
  game_play_move(g, 3, 6, BLACK);
  game_play_move(g, 2, 5, BLACK);
  game_play_move(g, 2, 0, BLACK);
  game_play_move(g, 1, 5, WHITE);
  if (game_nb_neighbors(g, 2, 6, BLACK) != 4 || game_nb_neighbors(g, 2, 6, WHITE) != 1 ||
      game_nb_neighbors(g, 2, 6, EMPTY) != 4) {
    game_delete(g);
    return false;
  }
  if (game_get_status(g, 2, 6) != UNSATISFIED) {
    game_delete(g);
    return false;
  }
  game_delete(g);

  // For FULL_EXCLUDE
  g = game_new_empty_ext(8, 7, true, FULL_EXCLUDE);
  if (game_get_neighbourhood(g) != FULL_EXCLUDE) {
    game_delete(g);
    return false;
  }
  game_set_constraint(g, 2, 6, 4);
  game_play_move(g, 1, 6, BLACK);
  game_play_move(g, 3, 6, BLACK);
  game_play_move(g, 2, 5, BLACK);
  game_play_move(g, 2, 0, BLACK);
  game_play_move(g, 1, 5, WHITE);
  if (game_nb_neighbors(g, 2, 6, BLACK) != 4 || game_nb_neighbors(g, 2, 6, WHITE) != 1 ||
      game_nb_neighbors(g, 2, 6, EMPTY) != 3) {
    game_delete(g);
    return false;
  }
  if (game_get_status(g, 2, 6) != UNSATISFIED) {
    game_delete(g);
    return false;
  }
  game_delete(g);

  // For ORTHO
  g = game_new_empty_ext(8, 7, true, ORTHO);
  if (game_get_neighbourhood(g) != ORTHO) {
    game_delete(g);
    return false;
  }
  game_set_constraint(g, 2, 6, 4);
  game_play_move(g, 1, 6, BLACK);
  game_play_move(g, 3, 6, BLACK);
  game_play_move(g, 2, 5, BLACK);
  game_play_move(g, 2, 0, BLACK);
  game_play_move(g, 1, 5, WHITE);
  if (game_nb_neighbors(g, 2, 6, BLACK) != 4 || game_nb_neighbors(g, 2, 6, WHITE) != 0 ||
      game_nb_neighbors(g, 2, 6, EMPTY) != 1) {
    game_delete(g);
    return false;
  }
  if (game_get_status(g, 2, 6) != UNSATISFIED) {
    game_delete(g);
    return false;
  }
  game_delete(g);

  // For ORTHO_EXCLUDE
  g = game_new_empty_ext(8, 7, true, ORTHO_EXCLUDE);
  if (game_get_neighbourhood(g) != ORTHO_EXCLUDE) {
    game_delete(g);
    return false;
  }
  game_set_constraint(g, 2, 6, 4);
  game_play_move(g, 1, 6, BLACK);
  game_play_move(g, 3, 6, BLACK);
  game_play_move(g, 2, 5, BLACK);
  game_play_move(g, 2, 0, BLACK);
  game_play_move(g, 1, 5, WHITE);
  if (game_nb_neighbors(g, 2, 6, BLACK) != 4 || game_nb_neighbors(g, 2, 6, WHITE) != 0 ||
      game_nb_neighbors(g, 2, 6, EMPTY) != 0) {
    game_delete(g);
    return false;
  }
  if (game_get_status(g, 2, 6) != SATISFIED) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

bool test_solve(void) {
  /*game g=game_default();
  game gsol=game_default_solution();
  game_solve(g);
  if(!game_equal(g, gsol)){
    game_delete(g);
    game_delete(gsol);
    return false;
  }
  game_delete(g);
  game_delete(gsol);*/
  game g2 = game_load("defaultTest3.txt");
  game_solve(g2);
  if (!game_won(g2)) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);
  /*game g3=game_load("defaultTest2.txt");
  game_solve(g3);
  if(game_won(g3)){
    game_delete(g3);
    return false;
  }
  game_delete(g3);*/
  return true;
}

bool test_nb_solutions(void) {
  /*game g=game_default();
  if(game_nb_solutions(g)!=1){
    game_delete(g);
    return false;
  }
  game_delete(g);*/
  game g2 = game_load("defaultTest3.txt");
  game_solve(g2);
  if (game_nb_solutions(g2) != 16) {
    game_delete(g2);
    return false;
  }
  game_delete(g2);
  /* game g3=game_load("defaultTest2.txt");
   if(game_nb_solutions(g3)!=0){
     game_delete(g3);
     return false;
   }
   game_delete(g3);*/
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

  // test new_empty_ext
  else if (strcmp("new_empty_ext", argv[1]) == 0) {
    result_test = test_new_empty_ext();
  }

  // test new_ext
  else if (strcmp("new_ext", argv[1]) == 0) {
    result_test = test_new_ext();
  }

  // test undo_redo
  else if (strcmp("undo_redo", argv[1]) == 0) {
    result_test = test_undo_redo();
  }

  // test wrapping
  else if (strcmp("wrapping", argv[1]) == 0) {
    result_test = test_wrapping();
  }

  // test neigh
  else if (strcmp("neigh", argv[1]) == 0) {
    result_test = test_neigh();
  }

  // test row_col
  else if (strcmp("row_col", argv[1]) == 0) {
    result_test = test_row_col();
  }
  // test nb_neigh_new
  else if (strcmp("nb_neigh_new", argv[1]) == 0) {
    result_test = test_nb_neigh_new();
  }
  // test solve
  else if (strcmp("solve", argv[1]) == 0) {
    result_test = test_solve();
  }
  // test nb_solutions
  else if (strcmp("nb_solutions", argv[1]) == 0) {
    result_test = test_nb_solutions();
  }

  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // all tests works
  if (result_test) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}