#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

double reciprocal(int i) {
  assert (i != 0);
  return 1.0/i;
}

int main(int argc, char const *argv[])
{
  int i;
  i = atoi(argv[1]);
  printf("The reciprocal of %d is %g\n", i, reciprocal(i));
  return 0;
}
