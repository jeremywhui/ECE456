#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

double reciprocal (int i){
  assert(i !=0); //verify whether i is zero
  return 1.0/i;
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: ./main n");
    exit(EXIT_FAILURE);
  }
  int i;
  i = atoi(argv[1]); //convert string to integer
  printf("The reciprocal of %d is %g.\n",i,reciprocal(i));
  return 0;
}

