#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"



bool test_dummy(){
    return true;
}

bool test_set_color(){
    game g = game_default();
    game_set_color(g,1,1,BLACK);
    game_set_color(g,2,2,WHITE);
    game_set_color(g,3,3,EMPTY);
    if(game_get_color(g,1,1) != BLACK || game_get_color(g,2,2) != WHITE || game_get_color(g,3,3) != EMPTY){
        return false;
    }

    return true;
}

bool test_play_move(){
    game g = game_default();
    game_play_move(g,1,1,BLACK);
    game_play_move(g,2,2,WHITE);
    game_play_move(g,3,3,EMPTY);
    if(game_get_color(g,1,1) != BLACK || game_get_color(g,2,2) != WHITE || game_get_color(g,3,3) != EMPTY){
        return false;
    }
    return true;
}

bool test_restart(){
    game g = game_default();
    game_play_move(g,1,1,BLACK);
    game_restart(g);
    if(game_get_color(g,1,1) == BLACK){
        return false;
    }
    game_play_move(g,2,2,WHITE);
    game_restart(g);
    if(game_get_color(g,2,2) == WHITE){
        return false;
    }
    return true;
}

int test_toccelli(char *argv[])
{
    bool ok = false;
    if(!(strcmp("dummy", argv[1]))){
        ok = test_dummy();
    }
    else if(!(strcmp("set_color", argv[1])))
    {
        ok = test_set_color();
    } 
    else if(!(strcmp("play_move", argv[1])))
    {
        ok = test_play_move();
    } 
    else if(!(strcmp("restart", argv[1])))
    {
        ok = test_restart();
    } 
    else
    {
        fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

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

/* ********** USAGE ********** */

void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[])
{
    if (argc == 1)
        usage(argc, argv);
    
    return test_toccelli(argv);   
}