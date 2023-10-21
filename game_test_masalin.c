//include library and .h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

//functions
bool test_dummy(void);
bool test_new_empty(void);
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
    game g = game_new_empty()
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
    game_delete(g)
    return true
}

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
    else
    {
        fprintf(stderr, "1\n");
        exit(EXIT_FAILURE);
    }

    //test new_empty
    if(strcmp("new_empty", argv[1]) == 0)
    {
        result_test=test_new_empty();
    }
    else
    {
        fprintf(stderr, "Error test_new_empty\n");
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