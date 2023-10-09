#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "game_aux.h"

int main(void)
{
    //Initialising game
    game g;
    g=game_default();
    while (game_won(g) == false)
    {
        int i, j=0;
        printf("\n");
        game_print(g);

        //Verify if the game has any error
        for (i = 0; i < DEFAULT_SIZE; i++)  
        {
            for (j = 0; j < DEFAULT_SIZE; j++)
            {
                if(game_get_status(g,i,j)==ERROR)
                {
                    printf("\nSquare (%d,%d) : error\n", i, j);
                }
            }
        } 
        //End of the verification
        
        //First scanf between help, restart, quit and the move
        char choice =0;
        printf("\nPress 'h' for help\n");
        int r = scanf(" %c", &choice);
        if(r==1)
        {
            if (choice=='h') //Help
            {
                printf("\nPress 'w' <i> <j> to set square (i,j) white\nPress 'b' <i> <j> to set square (i,j) black\nPress 'e' <i> <j> to set square (i,j) empty\nPress 'r' to restart\nPress 'q' to quit\n");
            }
            if (choice=='r') //Restart
            {
                printf("\nGame restarted\n");
                game_restart(g);
            }
            if (choice=='q') //Quit
            {
                printf("\nToo bad, you were so close, what a shame..\n");
                game_delete(g);
                return EXIT_SUCCESS;
            }
            if (choice=='w' || choice=='b' || choice=='e') //Move
            {
                int r = scanf(" %d %d", &i, &j); //Scanf for the position of the move
                if (r==2)
                {
                    if(i < DEFAULT_SIZE && i>=0 && j<DEFAULT_SIZE && j>=0)
                    {
                        if (choice=='w')
                        {
                            game_play_move(g, i, j, WHITE); //Play white color at the chosen square
                        }
                        if (choice=='b')
                        {
                            game_play_move(g, i, j, BLACK); //Play black color at the chosen square
                        }
                        if (choice=='e')
                        {
                            game_play_move(g, i, j, EMPTY); //Empty the chosen square
                        }
                        
                    }
                    else
                    {
                        printf("\nYour move is not legal, it has to be between 0 and %d\n", DEFAULT_SIZE-1); //The move is illegal
                    }
                }
            }
        } 
    }

    //Victory
    printf("\nCongratulation, you won! This is the final grid :\n \n");
    game_print(g);
    return EXIT_SUCCESS;
}
