/* fseek example */ 
#include <stdio.h> 
// This program modifies a specific part of a file using fseek()

int main () { 
  FILE * pFile; 
  pFile = fopen ( "example.txt" , "w" ); 
  fputs ( "This is an apple." , pFile ); 
  fseek ( pFile , 9 , SEEK_SET ); 
  fputs ( " sam" , pFile ); 
  fclose ( pFile ); 
  return 0;
 }
