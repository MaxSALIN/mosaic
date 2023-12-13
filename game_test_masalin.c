// include library and .h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"


// functions
bool test_dummy(void);
bool test_new_empty(void);
bool test_get_color(void);
bool test_get_constraint(void);
bool test_delete(void);
bool test_new(void);
bool test_default_solution(void);
bool new_empty_ext(void);
bool new_ext(void);
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

bool test_new_empty_ext(void){
  for(int row=1; row<=10; row++){
    for(int col=1; col<=10; col++){
      game g = game_new_empty_ext(row, col, false, FULL_EXCLUDE);
      if (g == NULL) {
        return false;
      }
      for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
          if ((game_get_color(g, i, j) != EMPTY) || (game_get_constraint(g, i, j) != -1)) {
            return false;
          }
        }
      }
      game_print(g);
      game_delete(g);
    }
  }
  return true;
}

bool test_new_ext(void){
  for(int row=1; row<=10; row++){ //problème de wrapping pour 1
    for(int col=1; col<=10; col++){
      constraint constraints1[row * col];
      for (int i = 0; i < row * col; i++) {
        constraints1[i] = -1;
      }
      for(int neigh = FULL; neigh<=ORTHO_EXCLUDE; neigh++){
        game g_Test = game_new_ext(row, col, constraints1, NULL, true, neigh);
        if (g_Test == NULL) {
          return false;
        }
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
              if ((game_get_color(g_Test, i, j) != EMPTY) || (game_get_constraint(g_Test, i, j) != -1)) {
                return false;
              }
            }
          }
        game_delete(g_Test);
      }
    }
  }
  int row2=7;
  int col2=6;
  constraint constraints1[row2 * col2];
  color colors1[row2 * col2];
  for (int i = 0; i < row2 * col2; i++) {
    constraints1[i] = 0;
    colors1[i]=WHITE;
  }
  game g = game_new_ext(row2, col2, constraints1, colors1, true, FULL);
  
  for (int i = 0; i < row2; i++) {
    for (int j = 0; j < col2; j++) {
      if ((game_get_color(g, i, j) != WHITE) || (game_get_constraint(g, i, j) != 0) || game_get_status(g, i, j)!=SATISFIED) {
        return false;
      }
    }
  }
  game_play_move(g, row2-1, col2-1, BLACK);
  for (int i = 0; i < row2; i++) {
    for (int j = 0; j < col2; j++) {
      if((i==0 && j==0) || (i==0 && j>3) || (i>4 && j==0)){
        if(game_is_wrapping(g)==true){   
          if(game_get_status(g, i, j)!=ERROR){
            return false;
          }
        }
      }
      else if(i>4 && j>3){
        if(game_get_status(g, i, j)!=ERROR){
          return false;
        }
      }
      else if (game_get_status(g, i, j)!=SATISFIED){
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_undo_redo(void){
  game g=game_default();
  game g_copy1=game_copy(g);
  game_play_move(g, 0, 0, BLACK);
  game g_copy2=game_copy(g);
  game_play_move(g, 0, 1, WHITE);
  game g_copy3=game_copy(g);
  game_undo(g);
  if(!game_equal(g, g_copy2)){
    printf("Erreur premier Undo");
    return false;
  }
  game_undo(g);
  if(!game_equal(g, g_copy1)){
    printf("Erreur deuxième Undo");
    return false;
  }
  game_redo(g);
  if(!game_equal(g, g_copy2)){
    printf("Erreur premier Redo");
    return false;
  }
  game_redo(g);
  if(!game_equal(g, g_copy3)){
    printf("Erreur deuxième Redo");
    return false;
  }
  game_undo(g);
  game_play_move(g, 4, 4, WHITE);
  game g_copyF=game_copy(g);
  game_redo(g);
  if(!game_equal(g, g_copyF)){
    printf("Erreur dernier Undo/Redo");
    return false;
  }
  game_delete(g);
  game_delete(g_copy1);
  game_delete(g_copy2);
  game_delete(g_copy3);
  game_delete(g_copyF);
  return true;
}

bool test_wrapping(void){
  /*game g= game_new_empty_ext(6,6,true, FULL);
  game_set_constraint(g, 5, 5, 0);
  game_play_move(g, 0, 0, BLACK);
  if(game_get_status(g, 5, 5)!=ERROR){
    return false;
  }
  game_play_move(g, 0, 0, WHITE);
  game_play_move(g, 0, 4, BLACK);
  if(game_get_status(g, 5, 5)!=ERROR){
    return false;
  }*/
  return true;
}

bool test_neigh(void){
  uint pi_next=0;
  uint pj_next=0;
  //FULL
  game g=game_new_empty_ext(5, 5, false, FULL);
  game_set_constraint(g, 3, 3, 9);
   for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
    printf("%d", dir);
    if (game_get_next_square(g, 3, 3, dir, &pi_next, &pj_next) == true) {
      game_play_move(g, pi_next, pj_next, BLACK);
    }
  }
  if(game_get_status(g, 3, 3)!=SATISFIED){
    printf("Error in FULL");
    return false;
  }
  game_delete(g);
  /*pi_next=0;
  pj_next=0;
  //FULL_EXCLUDE
  game g2=game_new_empty_ext(5, 5, false, FULL_EXCLUDE);
  game_set_constraint(g2, 3, 3, 8);
   for (direction dir = HERE; dir <= DOWN_RIGHT; dir++) {
    if (game_get_next_square(g2, 3, 3, dir, &pi_next, &pj_next) == true) {
      game_play_move(g2, pi_next, pj_next, BLACK);
    }
  }
  game_print(g);
  if(game_get_status(g2, 3, 3)!=SATISFIED){
    printf("Error in FULL_EXCLUDE");
    return false;
  }*/
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