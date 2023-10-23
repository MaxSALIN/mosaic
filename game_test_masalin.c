//include library and .h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

//functions
bool test_dummy(void);
bool test_new_empty(void);
bool test_get_color(void);
void usage(int argc, char *argv[]);


//coding of functions
void usage(int argc, char *argv[]) 
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

bool test_dummy(void)
{
    return true;
}

bool test_new_empty(void)
{
    game g = game_new_empty();
    for(int i =0; i<DEFAULT_SIZE, i++)
    {
        for(int j =0; j<DEFAULT_SIZE, j++)
        {
            if (game_get_color(g, i, j)!=EMPTY)
            {
                return false;
            }
        }
    }
    game_delete(g);
    return true;
}

bool test_get_color(void)
{
    game g = game_default();
    if (game_get_color(g, 0, 0)!=EMPTY && game_get_color(g, 0, 0)!=BLACK && game_get_color(g, 0, 0)!=WHITE)
        {
                return false;
        }
    game_delete(g);
    return true;
}

bool test_get_constraint(void)
{
    game g = game_default();
    if(game_get_constraint(g, 0,0)==-1 || game_get_constraint(g, 0,0)<=9 ||game_get_constraint(g, 0,0)>0)
    {
        return true;
    }
    game_delete(g);
    return false;
}

bool test_delete(void)
    game g = game_default();
    game_delete(g);
    if (g ==NULL)
    {
        return true;
    }
    return false;

/*bool test_new(void)
{

}

bool test_default_solution(void)
{

}*/

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        usage(argc, argv);
    }
    bool result_test =false;

    //test dummy
    if(strcmp("dummy", argv[1]) == 0)
    {
        result_test=test_dummy();
    }

    //test new_empty
    if(strcmp("new_empty", argv[1]) == 0)
    {
        result_test=test_new_empty();
    }

    //test get_color
    if(strcmp("get_color", argv[1]) == 0)
    {
        result_test=test_get_color();
    }

    //test get_constraint
    if(strcmp("get_constraint", argv[1]) == 0)
    {
        result_test=test_get_constraint();
    }

    //test delete
    if(strcmp("delete", argv[1]) == 0)
    {
        result_test=test_delete();
    }
    
    else
    {
        exit(EXIT_FAILURE);
    }

    //all tests works
    if (result_test)
    {
        fprintf(stderr, "0\n");
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "1\n");
        return EXIT_FAILURE;
    }
}