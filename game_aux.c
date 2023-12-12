#include "game_aux.h"
#include "game_struct.h"
#include "game_ext.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

game safeMalloc(uint row, uint col);
void checkGame(game g, uint i, uint j, char *text);
void putStatus(game g, uint i, uint j);

void game_print(cgame g) {
  checkGame((game)g, 0, 0, "game_print");
  // prints the numbers and the top outline
  printf("   ");
  for (int i = 0; i < g->col; i++) {
    printf("%d", i);
    printf(" ");
  }
  printf("\n");
  printf("   ");
  for (int i = 0; i < g->col; i++) {
    printf("--");
  }
  printf("\n");

  // prints the game numbers
  for (int i = 0; i < g->row; i++) {
    // prints the numbers on the left side
    printf("%d |", i);
    //
    for (int j = 0; j < g->col; j++) {
      switch (g->couleur[j + g->col* i]) {
        case EMPTY:  // for EMPTY
          if (g->contrainte[j + g->col* i] == UNCONSTRAINED) {
            printf(" ");
          } else
            for (int k = 0; k < 10; k++) {
              if (g->contrainte[j + g->col* i] == k) {
                printf("%d", k);
              }
            }
          printf(" ");
          break;

        case WHITE:  // for WHITE
          if (g->contrainte[j + g->col* i] == UNCONSTRAINED) {
            printf("□ ");
          }

          if (g->contrainte[j + g->col* i] == 0) {
            printf("🄋 ");
          }
          if (g->contrainte[j + g->col* i] == 1) {
            printf("➀ ");
          }
          if (g->contrainte[j + g->col* i] == 2) {
            printf("➁ ");
          }
          if (g->contrainte[j + g->col* i] == 3) {
            printf("➂ ");
          }
          if (g->contrainte[j + g->col* i] == 4) {
            printf("➃ ");
          }
          if (g->contrainte[j + g->col* i] == 5) {
            printf("➄ ");
          }
          if (g->contrainte[j + g->col* i] == 6) {
            printf("➅ ");
          }
          if (g->contrainte[j + g->col* i] == 7) {
            printf("➆ ");
          }
          if (g->contrainte[j + g->col* i] == 8) {
            printf("➇ ");
          }
          if (g->contrainte[j + g->col* i] == 9) {
            printf("➈ ");
          }

          break;
        case BLACK:  // for BLACK
          if (g->contrainte[j + g->col* i] == UNCONSTRAINED) {
            printf("■ ");
          }
          if (g->contrainte[j + g->col* i] == 0) {
            printf("🄌 ");
          }
          if (g->contrainte[j + g->col* i] == 1) {
            printf("➊ ");
          }
          if (g->contrainte[j + g->col* i] == 2) {
            printf("➋ ");
          }
          if (g->contrainte[j + g->col* i] == 3) {
            printf("➌ ");
          }
          if (g->contrainte[j + g->col* i] == 4) {
            printf("➍ ");
          }
          if (g->contrainte[j + g->col* i] == 5) {
            printf("➎ ");
          }
          if (g->contrainte[j + g->col* i] == 6) {
            printf("➏ ");
          }
          if (g->contrainte[j + g->col* i] == 7) {
            printf("➐ ");
          }
          if (g->contrainte[j + g->col* i] == 8) {
            printf("➑ ");
          }
          if (g->contrainte[j + g->col* i] == 9) {
            printf("➒ ");
          }

          break;
      }
    }
    // prints the left and right outline
    printf("|\n");
  }
  printf("   ");
  // prints the outline at the bottom
  for (int i = 0; i < g->col; i++) {
    printf("--");
  }
  printf("\n");
}

game game_default(void) {
  game g = safeMalloc(5, 5);
  g->row=DEFAULT_SIZE;
  g->col=DEFAULT_SIZE;
  g->wrapping=false;
  g->neighbour=FULL;
  // setting up the different precise constraints in game_default
  for (int i = 0; i < g->col*g->row; i++) {
    // puts the color EMPTY and the status UNSTATISFIED to all squares
    g->statut[i] = UNSATISFIED;
    g->couleur[i] = EMPTY;
    // puts the correct constraint to all squares
    if (i == 0) {
      g->contrainte[i] = 0;
    } else if (i == 3) {
      g->contrainte[i] = 3;
    } else if (i == 6) {
      g->contrainte[i] = 5;
    } else if (i == 12) {
      g->contrainte[i] = 4;
    } else if (i == 14) {
      g->contrainte[i] = 1;
    } else if (i == 15) {
      g->contrainte[i] = 6;
    } else if (i == 17) {
      g->contrainte[i] = 6;
    } else if (i == 18) {
      g->contrainte[i] = 3;
    } else {
      g->contrainte[i] = -1;
    }
  }
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
      putStatus(g, i, j);
    }
  }
  return g;
}

game game_default_solution(void) {
  game g = game_default();
  // puts all of the squares to SATISFIED and changes the colors of it according to the solution
  for (int i = 0; i < g->row; i++) {
    for (int j = 0; j < g->col; j++) {
      g->statut[j + g->col* i] = SATISFIED;
      if (i > 1 && j > -1 && j < 2) {
        g->couleur[j + g->col* i] = BLACK;
      } else if (i != 3 && j == 2) {
        g->couleur[j + g->col* i] = BLACK;
      } else if (i == 4 && j == 3) {
        g->couleur[j + g->col* i] = BLACK;
      } else if (i == 1 && j == 4) {
        g->couleur[j + g->col* i] = BLACK;
      } else {
        g->couleur[j + g->col* i] = WHITE;
      }
    }
  }
  return g;
}