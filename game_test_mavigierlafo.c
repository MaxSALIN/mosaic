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
// bool test_next_square();
bool test_default();
void usage(int argc, char *argv[]);

bool test_dummy(void) {

    return true;
}

bool test_won(void) {

    game g;
    g=game_default_solution();

    // on vérifie si toutes les cases sont noires ou blanches
    int count_success = 0;
    for (int x = 0; x < DEFAULT_SIZE; x++) {
      for (int y = 0; y < DEFAULT_SIZE; y++) {
        if (game_get_color(g, x, y) == WHITE || game_get_color(g, x, y) == BLACK) {
          if (game_get_status(g, x, y) == SATISFIED) {
              count_success++;
          }
        }
      }
    }

    // si c'est le cas ET que la fct renvoie truc -> won fonctionne

    if (count_success == (DEFAULT_SIZE * DEFAULT_SIZE) && game_won(g)) {
        return true;
    } else {
      return false;
    }
    return true;
}

bool test_nb_neighbors(void) {

  // on fait la mm chose que fait la fct et on compare le résultat
  game g;
  g=game_default();

  int i = 2;
  int j = 2;
  color c = WHITE;
  int nb_neighbors = 0;
  bool result = false;

  // plusieurs jeux

  for (int x = i-2; x <= i+2; x++) {
    if (game_get_color(g, x, j) == c) {
        nb_neighbors++;
    }
  }
  for (int y = j-2; y <= j+2; y++) {
    if (game_get_color(g, i, y) == c) {
        nb_neighbors++;
    }
  }

  if (nb_neighbors == game_nb_neighbors(g, i, j, c)) {
    result = true;
  } else {
    result = false;
  }

  c = BLACK;
  nb_neighbors = 0;
  for (int x = i-2; x <= i+2; x++) {
    if (game_get_color(g, x, j) == c) {
        nb_neighbors++;
    }
  }
  for (int y = j-2; y <= j+2; y++) {
    if (game_get_color(g, i, y) == c) {
        nb_neighbors++;
    }
  }

  if (nb_neighbors == game_nb_neighbors(g, i, j, c)) {
    result = true;
  } else {
    result = false;
  }

  return result;
}

bool test_get_status(void) {
    game g;
    g = game_default();

    bool test = false;

    int i = 3;
    int j = 3;

    // il ne faut pas qu'il y ait plus de 3 carré noir dans l'espace 3x3 d'un bloc
    if (game_nb_neighbors(g, i, j, BLACK) == 3 && game_get_status(g,i,j) == SATISFIED) {
        test = true;
    } else if (game_nb_neighbors(g, i, j, BLACK) > 3 && game_get_status(g,i,j) == ERROR) {
        test = true;
    } else if (game_nb_neighbors(g, i, j, BLACK) < 3 && game_get_status(g,i,j) == UNSATISFIED) {
        test = true;
    }

    return test;
}

/*bool test_next_square(void) {
    game g;
    g = game_default();

    bool result = false;

    int i = 2;
    int j = 3;

    pi_next = 2;
    pj_next = 3;

    if (i < DEFAULT_SIZE && j < DEFAULT_SIZE) {

        if ((i+pi_next) < DEFAULT_SIZE && (j+pj_next < DEFAULT_SIZE)) {

            result = true;

            Tuple coo = getCoo(d, i,j,pi_next, pj_next);

            int i_bis = coo.index(0);
            int j_bis = coo.index(1);

            result = (i_bis <= DEFAULT_SIZE && j_bis <= DEFAULT_SIZE);

            if (game_get_next_square(g, i, j, LEFT, pi_next, pj_next) == result) {
                result = true;  
            }

            return result;
        }
    }

    return result;
}

struct Point {
    int x;
    int y;
};

Tuple getCoo(direction d, int i, int j, int i2, int j2) {

  struct Point point;   

    if (d == LEFT) {
        point.x = i-i2;
        return point.x;
    } else if (d == RIGHT) {
        return i+i2;
    } else if (d == DOWN) {
        return j-j2;
    } else if (d == UP) {
        return j+j2;
    } else if (d == DOWN_RIGHT) {
        return (i+i2, j-j2);
    } else if (d == DOWN_LEFT) {
        return (i-i2, j-j2);
    } else if (d == UP_RIGHT) {
        return (i+i2, j+j2);
    } else if (d == UP_LEFT) {
        return (i-i2, j+j2);
    } else if (d == HERE) {
        return (i, j);
    }
    return (i, j);
}*/

bool test_default(void) {

    game g;
    g = game_default();

    int count_noempty = 0;

    for (int x = 0; x < DEFAULT_SIZE; x++) {
      for (int y = 0; y < DEFAULT_SIZE; y++) {
        if (game_get_color(g, x, y) != EMPTY) {
          if (game_get_constraint(g, x, y) > MIN_CONSTRAINT && game_get_constraint(g, x, y) <= MAX_CONSTRAINT) {
              count_noempty++;
          }
        }
      }
    }

    if (count_noempty == 8) {
      return true;
    } else {
      return false;
    }

}

void usage(int argc, char *argv[]) {

	fprintf(stderr, "Usage: %s <test> \n", argv[0]);
	exit(EXIT_FAILURE);
}



int main(int argc, char *argv[])
{
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
  } else if (strcmp("test_get_status", argv[1]) == 0) {
    ok = test_get_status();
  }else if (strcmp("test_default", argv[1]) == 0) {
    ok = test_default();
  } else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }
   

  // print test result
  if (ok)
  {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  }
  else
  {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
