#include <stdio.h>

void maliciousCode()
{
	printf("Congratulations!\n");
	printf("You have entered malicious code!\n");
	printf("The malicious code is completed!\n");
}

void echo()
{
	char buffer[20];

	printf("Enter text:\n");
	scanf("%s", buffer);
	printf("You entered: \n%s\n", buffer);	
}

int main()
{
	printf("Call the legitimate function\n");
	echo();
	printf("Complete the legitimate function\n");
	return 0;
}
