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
    game_set_color(g,3,3,BLACK);
    game_set_color(g,3,3,EMPTY);
    if(game_get_color(g,1,1) != BLACK || game_get_color(g,2,2) != WHITE || game_get_color(g,3,3) != EMPTY){
        return false;
    }
    game_delete(g);
    return true;
}

bool test_play_move(){
    game g = game_new_empty();
    game_set_constraint(g,1,1,1);
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            if(i == 1 && j == 1){
                game_play_move(g,i,j,BLACK);
            }else{
                game_play_move(g,i,j,WHITE);
            }
        }
    }
    for(int i = 0;i<3;i++){
        for(int j = 0;j<3;j++){
            if(i == 1 && j == 1){
                if(game_get_color(g,i,j) != BLACK && game_get_status(g,i,j) != SATISFIED){
                    return false;
                }
            }else{
                if(game_get_color(g,i,j) != WHITE){
                    return false;
                }
            }
        }
    }
    game_delete(g);
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
    game_delete(g);
    return true;
}

bool test_equal(){
    game g1 = game_default();
    game g2 = game_default();
    game_play_move(g1,1,1,BLACK);
    game_play_move(g2,1,1,BLACK);
    if(!game_equal(g1,g2)){
        return false;
    }
    game_play_move(g2,1,1,WHITE);
    if(game_equal(g1,g2)){
        return false;
    }
    game_delete(g1);
    game_delete(g2);
    return true;
}

bool test_copy(){
    game g1 = game_default();
    game_play_move(g1,1,1,BLACK);
    game g2 = game_copy(g1);
    if(!game_equal(g1,g2)){
        return false;
    }
    game_delete(g1);
    game_delete(g2);
    return true;
}

bool test_set_constraint(){
    game g = game_new_empty();
    for (int x = 0; x < DEFAULT_SIZE; x++) {
        for (int y = 0; y < DEFAULT_SIZE; y++) {
            game_set_constraint(g,x,x,0);
            game_play_move(g, x, y, WHITE);
        }
    }
    for (int x = 0; x < DEFAULT_SIZE; x++) {
        for (int y = 0; y < DEFAULT_SIZE; y++) {
            if(game_nb_neighbors(g, x, y, BLACK)!=0 && game_get_status(g,x,y)==SATISFIED)
            {
                return false;
            }
        }
    }
    for (int x = 0; x < DEFAULT_SIZE; x++) {
        for (int y = 0; y < DEFAULT_SIZE; y++) {
            game_set_constraint(g,x,x,9);
            game_play_move(g, x, y, BLACK);
        }
    }
    
    for (int x = 1; x < DEFAULT_SIZE-1; x++) {
        for (int y = 1; y < DEFAULT_SIZE-1; y++) {
            if(game_nb_neighbors(g, x, y, BLACK)!=9 && game_get_status(g,x,y)==SATISFIED)
            {
                return false;
            }
        }
    }
    game g2=game_new_empty();
    game_set_constraint(g2,3,3, 9);
    for (int x = 1; x < DEFAULT_SIZE-1; x++) {
        for (int y = 1; y < DEFAULT_SIZE-1; y++) {
            game_set_color(g2, x, y, BLACK);
        }
    }
    game_set_constraint(g2,3,3, 0);
    if(game_get_status(g2,3,3)==SATISFIED){
        return false;
    }

    game_delete(g);
    game_delete(g2);
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
    else if(!(strcmp("equal", argv[1])))
    {
        ok = test_equal();
    }    
    else if(!(strcmp("copy", argv[1])))
    {
        ok = test_copy();
    }
    else if(!(strcmp("set_constraint", argv[1])))
    {
        ok = test_set_constraint();
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