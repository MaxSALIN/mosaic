#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

bool test_dummy();
bool test_won();
bool test_nb_neighbors();
void usage(int argc, char *argv[]);

bool test_dummy() {

    return true;
}

bool test_won() {

    game g;
    g=game_default_solution();

    // on vérifie si toutes les cases sont noires ou blanches
    int count_success = 0;
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) {
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

bool test_nb_neighbors() {

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
