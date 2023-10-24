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
bool test_get_constraint(void);
bool test_delete(void);
bool test_new(void);
bool test_default_solution(void);
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
    if(g==NULL)
    {
        return false;
    }
    for (int i = 0; i<DEFAULT_SIZE; i++)
    {
        for (int j = 0; j<DEFAULT_SIZE; j++)
        {
            if((game_get_color(g, i, j)!=EMPTY) || (game_get_constraint(g, i, j)!=-1))
            {
                return false;
            }
        }
    }
    return true;
}

bool test_get_color(void)
{
    //general test
    game g = game_default();
    if (game_get_color(g, 0, 0)!=EMPTY && game_get_color(g, 0, 0)!=BLACK && game_get_color(g, 0, 0)!=WHITE)
        {
                return false;
        }

    //case by case
    game_set_color(g, 1,1, BLACK);
    if(game_get_color(g,1,1)!=BLACK)
    {
        return false;
    }
    game_set_color(g, 2,2, WHITE);
    if(game_get_color(g,2,2)!=WHITE)
    {
        return false;
    }
    game_set_color(g, 3,3, EMPTY);
    if(game_get_color(g,3,3)!=EMPTY)
    {
        return false;
    }
    game_delete(g);
    return true;
}

bool test_get_constraint(void)
{
    game g = game_default();
    if(game_get_constraint(g, 0,0)==-1 || game_get_constraint(g, 0,0)<=9 ||game_get_constraint(g, 0,0)>=0)
    {
        return true;
    }
    game_set_constraint(g, 1,1, UNCONSTRAINED);
    if(game_get_constraint(g,1,1)!=UNCONSTRAINED)
    {
        return false;
    }
    for(int i=0; i<10;i++)
    {
        
        game_set_constraint(g, 2,2, i);
        if(game_get_constraint(g,2,2)!=i)
        {
            return false;
        }
        
    }
    game_delete(g);
    return false;
}


bool test_delete(void)
{
    game g = game_default();
    game g_compare = game_default();
    game_delete(g);
    game g2=game_new_empty();
    game g2_compare = game_new_empty();
    game_delete(g2);
    if(g==g_compare || g2==g2_compare)
    {
        return false;
    }
    return true;

}

bool test_new(void)
{
    int constraints1[25];
    for(int i =0; i<DEFAULT_SIZE*5; i++)
    {
        constraints1[i] = -1;
    }
    constraint * constraints2 = constraints1;
    game g= game_new(constraints2, NULL);
    for (int i = 0; i<DEFAULT_SIZE; i++)
    {
        for (int j = 0; j<DEFAULT_SIZE; j++)
        {
            if((game_get_color(g, i, j)!=EMPTY) || (game_get_constraint(g, i, j)!=-1))
            {
                return false;
            }
        }
    }
    return true; //Ajoutez teste avec d'autres couleurs et un autre array de constraints

}

bool test_default_solution(void)
{
    game g = game_default_solution();
    if(!game_won(g))
    {
        return false;
    }
    return true;
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

    //test new_empty
    else if(strcmp("new_empty", argv[1]) == 0)
    {
        result_test=test_new_empty();
    }

   //test get_color
    else if(strcmp("get_color", argv[1]) == 0)
    {
        result_test=test_get_color();
    }

    //test get_constraint
    else if(strcmp("get_constraint", argv[1]) == 0)
    {
        result_test=test_get_constraint();
    }

    //test delete
    else if(strcmp("delete", argv[1]) == 0)
    {
        result_test=test_delete();
    }

    //test new
    else if(strcmp("new", argv[1]) == 0)
    {
        result_test=test_new();
    }
    
    //test default_solution
    else if(strcmp("default_solution", argv[1]) == 0)
    {
        result_test=test_default_solution();
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