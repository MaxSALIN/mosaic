#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
bool test_dummy(void);
void usage(int argc, char *argv[]);

void usage(int argc, char *argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

bool test_dummy(void)
{
    return true;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        usage(argc, argv);
    }
    bool ok =false;
    if(strcmp("dummy", argv[1]) == 0)
    {
        ok=test_dummy();
    }
    else
    {
        fprintf(stderr, "1\n");
        exit(EXIT_FAILURE);
    }
    if (ok)
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