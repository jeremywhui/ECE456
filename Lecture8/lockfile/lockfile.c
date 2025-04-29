#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
/*The program in Listing opens a file for writing, and then places a write lock on it. The program waits for the user to hit Enter and then unlocks and closes the file.*/
int main (int argc, char* argv[])
{
 char* file = "testfile.txt";
 char* buffer = "The additional text we want to write to the file. ";
 int fd;
 struct flock lock;

 printf ("opening %s\n", file);
 /* Open a file descriptor to the file. */
 fd = open (file, O_WRONLY);

 /* Initialize the flock structure. */
 memset (&lock, 0, sizeof(lock));
 lock.l_type = F_WRLCK;
 /* Place a write lock on the whole file. */
 printf ("Place a blocking lock on the file\n");
 fcntl (fd, F_SETLKW, &lock);
 printf ("Write to the file\n");
 lseek(fd, 0, SEEK_END); /* seek to the end of the file */
 write(fd, buffer, strlen(buffer)); /* write the buffer to the end */
 printf ("Finish writing; Hit Enter to unlock the file\n");
 /* Wait for the user to hit Enter. */
 getchar ();
 printf ("Enter received, unlock the file now\n");

 /* Release the lock. */
 lock.l_type = F_UNLCK;
 fcntl (fd, F_SETLKW, &lock);
 printf ("file is unlocked\n");
 close (fd);
 return 0;
}