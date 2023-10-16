#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool test_toccelli(){
    return true;
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

    bool ok = false;
    if(!(strcmp("dummy", argv[1]))){
        ok = test_toccelli();
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