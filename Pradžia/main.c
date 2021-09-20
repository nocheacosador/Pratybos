#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		printf("Arguments detected: %i", argc - 1);
	}

	for (int i = 0; i < argc; i++)
		printf("Arg[%i]: %s\n", i, argv[i]);
	
	printf("Hello, pasauli!\n");
}