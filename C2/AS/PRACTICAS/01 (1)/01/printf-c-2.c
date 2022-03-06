// printf-c-2.c
#include <stdio.h>

int i = 0x12345678;
char *formato = "i = %i = 0x%08x\n";

int main()
{
	printf(formato, i, i);
}
