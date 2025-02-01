/* setbuf example */
#include <stdio.h>

int main ()
{
  char buffer[100];
  FILE *pFile1, *pFile2;

  pFile1=fopen ("myfile1.txt","w");
  pFile2=fopen ("myfile2.txt","w");

  setvbuf (pFile1, buffer, _IOFBF, sizeof(buffer)); //set fully buffered
  fputs ("This is sent to a buffered stream",pFile1);
  fflush (pFile1);

  setvbuf (pFile2, NULL, _IONBF, 0);
  fputs ("This is sent to an unbuffered stream",pFile2);

  fclose (pFile1);
  fclose (pFile2);

  return 0;
}
