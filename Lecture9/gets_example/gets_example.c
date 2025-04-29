#include <stdio.h>



/* Sample implementation of library function gets() */
char *gets_sample(char *s)
{
	int c;
	char *dest = s;
	int gotchar = 0; /* Has at least one character been read? */
	while ((c = getchar()) != '\n' && c != EOF) {
		*dest++ = c; /* No bounds checking! */
		gotchar = 1;

	}
	*dest++ = '\0'; /* Terminate string */
	printf("String copied to location starting at %p successfully\n",s);
	if (c == EOF && !gotchar)
		return NULL; /* End of file or error */
	return s;
	}
/* Read input line and write it back */
void echo()
{
	char buf[8]; /* Way too small! */
	//printf("Input the string:\n");
	gets_sample(buf);
	//printf("Echo back the string:\n");
	puts(buf);
}

int main()
{
	echo();
	return 0;
}