#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_dummy(void);
void usage(int argc, char *argv[]);

bool test_dummy(void) {

    return true;
}

void usage(int argc, char *argv[]) {

	fprintf(stderr, "Usage: %s <test> \n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  if (argc == 1) {

      usage(argc, argv);

  }

  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;

  if (argv[1] == NULL) {
    return EXIT_FAILURE;
  }

  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // print test result
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
