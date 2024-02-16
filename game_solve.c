#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/*void first_solver(game g, uint pos, uint len, uint *word, unsigned long *count){
  if (pos == len)
  {
    if(game_won(g)){
      (*count)++;
      game_print(g);
    }
    return;
  }

  // extend current word recursively with value 0 at position pos
  word[pos] = 0;
  game_play_move(g,pos/game_nb_cols(g), pos%game_nb_cols(g), WHITE);
  first_solver(g, pos + 1, len, word, count);

  // extend current word recursively with value 1 at position pos
  word[pos] = 1;
  game_play_move(g,pos/game_nb_cols(g), pos%game_nb_cols(g), BLACK);
  first_solver(g, pos + 1, len, word, count);

}*/

int main(int argc, char** argv) {
  /*if (argc != 4) {
      fprintf(stderr, "No arguments input");
      exit(EXIT_FAILURE);
  } else {
      game g=game_load(argv[2]);
      FILE *file = fopen(argv[3], "r");
      if (file == NULL) {
          fprintf(stderr, "output not loaded");
          exit(EXIT_FAILURE);
      }
      if(!strcmp(argv[1], "-s")){
          if(game_solve(g)){
              game_save(g, argv[3]);
          }
          else{
              exit(EXIT_FAILURE);
          }
      }
      else if(!strcmp(argv[1], "-c")){

          fprintf(file, "%d",game_nb_solutions(g));
      }
  }*/
  constraint con[4];
  color col[4];
  for (int i = 0; i < 4; i++) {
    con[i] = UNCONSTRAINED;
    col[i] = EMPTY;
  }
  con[0] = 1;
  game g = game_new_ext(2, 2, con, col, false, FULL);
  game_solve(g);
  /*game g=game_default();
  uint word[game_nb_cols(g)*game_nb_rows(g)];
  unsigned long nbwords = 0;

  // reset word
  for (int k = 0; k < game_nb_cols(g)*game_nb_rows(g); k++)
      word[k] = 0;
  first_solver(g, 0, game_nb_cols(g)*game_nb_rows(g), word, &nbwords);
  printf("Nombre de solutions: %lu\n", nbwords);*/
}