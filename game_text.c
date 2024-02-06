#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

int main(int argc, char** argv) {
  // Initialising game
  game g;
  bool stop = false;
  if (argc == 1) {
    g = game_default();
  } else {
    g = game_load(argv[1]);
  }
  while (!game_won(g)) {
    int i, j = 0;
    if(!stop){
      printf("\n");
      game_print(g);
    }
    

    // Verify if the game has any error
    for (i = 0; i < DEFAULT_SIZE; i++) {
      for (j = 0; j < DEFAULT_SIZE; j++) {
        if (game_get_status(g, i, j) == ERROR) {
          printf("\nSquare (%d,%d) : error\n", i, j);
        }
      }
    }
    // End of the verification

    // First scanf between help, restart, quit and the move
    char choice = 0;
    char* filename = NULL;
    
    printf("\nPress 'h' for help\n");
    int r1 = scanf(" %c", &choice);

      
    if (r1 == 1) {

      // When we want to get the whole text instead of one character.

      if (choice == 'h' && !stop)  // Help
      {
        printf(
            "\nPress 'w <i> <j>' to set the square (i,j) white\nPress 'b <i> <j>' to set the square (i,j) "
            "black\nPress "
            "'e <i> "
            "<j>' to set the square (i,j) empty\nPress 'z' to undo your move\nPress 'y' to redo your moven\nPress "
            "'r' "
            "to restart\nPress 'q' to quit\nPress 's filename.txt' to save your game in a filename.txt (filename can be "
            "any one word name)\n");
            stop = true;
      }
      else if (choice == 'r' && !stop)  // Restart
      {
        printf("\nGame restarted\n");
        game_restart(g);
        stop = true;
      }
      else if (choice == 'q'&& !stop)  // Quit
      {
        printf("\nToo bad, you were so close, what a shame..\n");
        game_delete(g);
        stop = true;
        return EXIT_SUCCESS;
      }
      else if (choice == 'z' && !stop)  // Undo
      {
        printf("\nMove Undo\n");
        game_undo(g);
        stop = true;
      }
      else if (choice == 'y' && !stop)  // Redo
      {
        printf("\nMove Redo\n");
        game_redo(g);
        stop = true;
      }
      else if (choice == 's' && !stop) {
        int r2 = scanf(" %s", filename);
        if (r2 == 1) {
          game_save(g, filename);
          stop = true;
        } else {
          printf("\nIncorrect file name arguments\n");
          stop = true;
        }
      }
      else if ((choice == 'w' || choice == 'b' || choice == 'e') && !stop)  // Move
      {
        int r3 = scanf(" %d %d", &i, &j);  // Scanf for the position of the move
        if (r3 == 2) {
          if (i < DEFAULT_SIZE && i >= 0 && j < DEFAULT_SIZE && j >= 0) {
            if (choice == 'w') {
              game_play_move(g, i, j, WHITE);  // Play white color at the chosen square
              stop = true;
            }
            if (choice == 'b') {
              game_play_move(g, i, j, BLACK);  // Play black color at the chosen square
              stop = true;
            }
            if (choice == 'e') {
              game_play_move(g, i, j, EMPTY);  // Empty the chosen square
              stop = true;
            }

          } else {
            printf("\nYour move is not legal, it has to be between 0 and %d\n",
                   DEFAULT_SIZE - 1);  // The move is illegal
                   stop = true;
          }
        }
      }
      stop=false;
    }
  }

  // Victory
  printf("\nCongratulation, you won! This is the final grid :\n \n");
  game_print(g);
  game_delete(g);
  return EXIT_SUCCESS;
}
