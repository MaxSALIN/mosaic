#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"

bool test_dummy();
bool test_won();
bool test_nb_neighbors();
bool test_get_status();
bool test_next_square();
bool test_default();
bool test_print();
void usage(int argc, char *argv[]);

bool test_dummy(void) { 
  game g= game_new_empty();
  game_set_color(g, 0 ,2, WHITE);
  game_print(g);
  return true; 
}

bool test_won(void) {
  game g;
  g = game_default_solution();
  game g_compare = game_default();
  bool result = true;
  // on vérifie si toutes les cases sont noires ou blanches
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      if (game_get_color(g, x, y) != WHITE && game_get_color(g, x, y) != BLACK) {
        result = false;
      }
      if ((game_get_status(g, x, y) != SATISFIED) ||
          (game_get_constraint(g, x, y) != game_get_constraint(g_compare, x, y))) {
        result = false;
      }
      if (game_get_constraint(g, x, y) != -1) {
        if (game_nb_neighbors(g, x, y, BLACK) != game_get_constraint(g, x, y)) {
          result = false;
        }
      }
    }
  }

  // si c'est le cas ET que la fct renvoie truc -> won fonctionne
  if (game_won(g) != result) {
    return false;
  }

  // test d'une game finit qui n'est pas won
  game g2 = game_new_empty();
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_set_constraint(g2, x, y, 1);
      game_play_move(g2, x, y, WHITE);
    }
  }
  if (game_won(g2) == true) {
    return false;
  }

  // test d'une game finit qui est won
  game g3 = game_new_empty();
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      game_set_constraint(g3, x, y, 0);
      game_play_move(g3, x, y, WHITE);
    }
  }
  if (game_won(g3) == false) {
    return false;
  }

  // test en cours d'une partie
  game g4 = game_new_empty();
  while (game_won(g4) != true) {
    for (int x = 0; x < DEFAULT_SIZE; x++) {
      for (int y = 0; y < DEFAULT_SIZE; y++) {
        game_play_move(g4, x, y, WHITE);
      }
    }
  }
  for (int x = 0; x < DEFAULT_SIZE; x++) {
    for (int y = 0; y < DEFAULT_SIZE; y++) {
      if (game_won(g4) != true || game_get_color(g4, x, y) == EMPTY || game_get_status(g4, x, y) != SATISFIED) {
        return false;
      }
    }
  }

  // cas d'une game qui a restart
  game_restart(g4);
  if (game_won(g4) == true) {
    return false;
  }

  // cas d'une game qui est une copie
  game g5 = game_copy(g3);
  if (game_won(g5) == false) {
    return false;
  }

  // cas d'une game juste créer
  game g6 = game_default();
  if (game_won(g6) == true) {
    return false;
  }

  // cas de 'on modifie une game gagnée'
  game g7 = game_copy(g);
  game_play_move(g7, 0, 0, BLACK);
  if (game_won(g7) == true) {
    return false;
  }
  game_delete(g);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  game_delete(g5);
  game_delete(g6);
  game_delete(g7);
  game_delete(g_compare);
  return true;
}

bool test_nb_neighbors(void) {
  // on fait la mm chose que fait la fct et on compare le résultat
  game g;
  g = game_new_empty();

  int i = 2;
  int j = 2;
  color c = WHITE;
  int nb_neighbors = 0;
  bool result = false;

  // plusieurs jeux

  for (int x = i - 1; x <= i + 1; x++) {
    for (int y = j - 1; y <= j + 1; y++) {
      if (game_get_color(g, x, j) == c) {
        nb_neighbors++;
      }
    }
  }
  if (nb_neighbors == game_nb_neighbors(g, i, j, c)) {
    result = true;
  } else {
    result = false;
  }

  c = BLACK;
  nb_neighbors = 0;
  for (int x = i - 1; x <= i + 1; x++) {
    for (int y = j - 1; y <= j + 1; y++) {
      game_set_color(g, x, y, c);
    }
  }
  for (int x = i - 1; x <= i + 1; x++) {
    for (int y = j - 1; y <= j + 1; y++) {
      if (game_get_color(g, x, j) == c) {
        nb_neighbors++;
      }
    }
  }
  if (nb_neighbors == game_nb_neighbors(g, i, j, c)) {
    result = true;
  } else {
    result = false;
  }
  game_delete(g);
  return result;
}

bool test_get_status(void) {
  game g;
  g = game_new_empty();
  int i = 3;
  int j = 3;
  game g_dummy = game_new_empty();
  for (int k = 0; k <= 9; k++) {
    
    game_set_constraint(g, i, j, k);
    // test pour satisfied et error
    for (int x = i - 1; x <= i + 1; x++) {
      for (int y = j - 1; y <= j + 1; y++) {
        game_play_move(g, x, y, WHITE);
      }
    }
    for (int y = i - 1; y <= i + 1; y++) {
      for (int x = j - 1; x <= j + 1; x++) {
        if ((game_nb_neighbors(g, i, j, BLACK) == k) && (game_nb_neighbors(g, i, j, WHITE)==k-game_nb_neighbors(g, i,j, BLACK)) && game_get_status(g, i, j) != SATISFIED && game_nb_neighbors(g, i, j, EMPTY)==0) {
          return false;
        } else if ((game_nb_neighbors(g, i, j, BLACK) > k && game_get_status(g, i, j) != ERROR)) {
          game_print(g);
          printf("%d, %d, %d, %d, %d ", x, y,k, game_nb_neighbors(g, i, j, BLACK), game_get_status(g, i, j));
          return false;
        }
        game_play_move(g, x, y, BLACK);
      }
    }

    game_restart(g);
    // test pour unsatisfied
    for (int x = i - 1; x <= i + 1; x++) {
      for (int y = j - 1; y <= j + 1; y++) {
        game_play_move(g, x, y, WHITE);
      }
    }
    for (int y = i - 1; y <= i + 1; y++) {
      for (int x = j - 1; x <= j + 1; x++) {
        if (game_nb_neighbors(g, i, j, BLACK) < k && game_get_status(g, i, j) != UNSATISFIED) {
          return false;
        }
        game_play_move(g, x, y, BLACK);
      }
    }
  }
  game_delete(g);
  game_delete(g_dummy);
  return true;
}

bool test_get_next_square(void) {
  game g;
  g = game_default();
  uint px_next[DEFAULT_SIZE + 2];
  uint py_next[DEFAULT_SIZE + 2];
  // centre 3x3
  for (uint x = 1; x < DEFAULT_SIZE - 1; x++) {
    for (uint y = 1; y < DEFAULT_SIZE - 1; y++) {
      if (game_get_next_square(g, x, y, HERE, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, UP, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, DOWN, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, LEFT, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, RIGHT, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, DOWN_LEFT, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, DOWN_RIGHT, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, UP_LEFT, px_next, py_next) == false) {
        return false;
      }
      if (game_get_next_square(g, x, y, UP_RIGHT, px_next, py_next) == false) {
        return false;
      }
    }
  }
  // ligne du haut
  for (uint x = 0; x < DEFAULT_SIZE; x++) {
    if (game_get_next_square(g, 0, x, HERE, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, 0, x, UP, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, 0, x, DOWN, px_next, py_next) == false) {
      return false;
    }
    if (x == 0) {
      if (game_get_next_square(g, 0, x, LEFT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != 0) {
      if (game_get_next_square(g, 0, x, LEFT, px_next, py_next) == false) {
        return false;
      }
    }
    if (x == DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 0, x, RIGHT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 0, x, RIGHT, px_next, py_next) == false) {
        return false;
      }
    }
    if (x == 0) {
      if (game_get_next_square(g, 0, x, DOWN_LEFT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != 0) {
      if (game_get_next_square(g, 0, x, DOWN_LEFT, px_next, py_next) == false) {
        return false;
      }
    }
    if (x == DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 0, x, DOWN_RIGHT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 0, x, DOWN_RIGHT, px_next, py_next) == false) {
        return false;
      }
    }
    if (game_get_next_square(g, 0, x, UP_LEFT, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, 0, x, UP_RIGHT, px_next, py_next) == true) {
      return false;
    }
  }
  // ligne du bas
  for (uint x = 0; x < DEFAULT_SIZE; x++) {
    if (game_get_next_square(g, 4, x, HERE, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, 4, x, UP, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, 4, x, DOWN, px_next, py_next) == true) {
      return false;
    }
    if (x == 0) {
      if (game_get_next_square(g, 4, x, LEFT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != 0) {
      if (game_get_next_square(g, 4, x, LEFT, px_next, py_next) == false) {
        return false;
      }
    }
    if (x == DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 4, x, RIGHT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 4, x, RIGHT, px_next, py_next) == false) {
        return false;
      }
    }
    if (x == 0) {
      if (game_get_next_square(g, 4, x, UP_LEFT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != 0) {
      if (game_get_next_square(g, 4, x, UP_LEFT, px_next, py_next) == false) {
        return false;
      }
    }
    if (x == DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 4, x, UP_RIGHT, px_next, py_next) == true) {
        return false;
      }
    }
    if (x != DEFAULT_SIZE - 1) {
      if (game_get_next_square(g, 4, x, UP_RIGHT, px_next, py_next) == false) {
        return false;
      }
    }
    if (game_get_next_square(g, 4, x, DOWN_LEFT, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, 4, x, DOWN_RIGHT, px_next, py_next) == true) {
      return false;
    }
  }
  // ligne gauche
  for (uint x = 1; x < DEFAULT_SIZE - 1; x++) {
    if (game_get_next_square(g, x, 0, HERE, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 0, UP, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 0, DOWN, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 0, LEFT, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, x, 0, RIGHT, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 0, DOWN_LEFT, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, x, 0, DOWN_RIGHT, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 0, UP_LEFT, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, x, 0, UP_RIGHT, px_next, py_next) == false) {
      return false;
    }
  }
  // ligne droite
  for (uint x = 1; x < DEFAULT_SIZE - 1; x++) {
    if (game_get_next_square(g, x, 4, HERE, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 4, UP, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 4, DOWN, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 4, LEFT, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 4, RIGHT, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, x, 4, DOWN_LEFT, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 4, DOWN_RIGHT, px_next, py_next) == true) {
      return false;
    }
    if (game_get_next_square(g, x, 4, UP_LEFT, px_next, py_next) == false) {
      return false;
    }
    if (game_get_next_square(g, x, 4, UP_RIGHT, px_next, py_next) == true) {
      return false;
    }
  }
  return true;
}

bool test_default(void) {
  game g;
  g = game_default();
  int count_noempty = 0;
  for (int y = 0; y < DEFAULT_SIZE; y++) {
    for (int x = 0; x < DEFAULT_SIZE; x++) {
      if (game_get_color(g, x, y) == EMPTY && game_get_status(g, x, y) == UNSATISFIED) {
        if (game_get_constraint(g, x, y) == 0 && x == 0 && y == 0) {
          count_noempty++;
        } else if (game_get_constraint(g, x, y) == 3 && x == 3 && y == 0) {
          count_noempty++;
        } else if (game_get_constraint(g, x, y) == 5 && x == 1 && y == 1) {
          count_noempty++;
        } else if (game_get_constraint(g, x, y) == 4 && x == 2 && y == 2) {
          count_noempty++;
        } else if (game_get_constraint(g, x, y) == 6 && x == 0 && y == 3) {
          count_noempty++;
        } else if (game_get_constraint(g, x, y) == 6 && x == 2 && y == 3) {
          count_noempty++;
        } else if (game_get_constraint(g, x, y) == 3 && x == 3 && y == 3) {
          count_noempty++;
        } else if (game_get_constraint(g, x, y) == 1 && x == 4 && y == 2) {
          count_noempty++;
        }
      }
    }
  }
  game_delete(g);
  if (count_noempty != 8) {
    return false;
  }
  return true;
}

bool test_print(void) {
  game g;
  g = game_default();

  game_print(g);
  game_delete(g);
  return true;
}

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <test> \n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    usage(argc, argv);
  }

  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;

  if (argv[1] == NULL) {
    return EXIT_FAILURE;
  }

  // dummy test
  if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy();
  } else if (strcmp("test_won", argv[1]) == 0) {
    ok = test_won();
  } else if (strcmp("test_nb_neighbors", argv[1]) == 0) {
    ok = test_nb_neighbors();
  } else if (strcmp("test_get_next_square", argv[1]) == 0) {
    ok = test_get_next_square();
  } else if (strcmp("test_get_status", argv[1]) == 0) {
    ok = test_get_status();
  } else if (strcmp("test_default", argv[1]) == 0) {
    ok = test_default();
  } else if (strcmp("test_print", argv[1]) == 0) {
    ok = test_print();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
