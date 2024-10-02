#include <stdio.h>
#include <stdlib.h>

void foo()
{
	int *p = NULL;

	printf("*p = %d\n", *p);
	free(p);
}

void main()
{
	printf("Demo Core Dumped\n");
	foo();
}
