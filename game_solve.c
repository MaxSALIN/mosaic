#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"


void first_solver(game g, uint pos, uint len, uint *word, unsigned long *count){
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
  
}



int main(int argc, char** argv){
    game g=game_default();
    uint word[game_nb_cols(g)*game_nb_rows(g)];
    unsigned long nbwords = 0;

    // reset word
    for (int k = 0; k < game_nb_cols(g)*game_nb_rows(g); k++)
        word[k] = 0;
    first_solver(g, 0, game_nb_cols(g)*game_nb_rows(g), word, &nbwords);
    printf("Nombre de solutions: %lu\n", nbwords);
}