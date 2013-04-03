#include <stdio.h>

char s[512] = "SMI:AOB29C01";

void main(void)
{
	int i;

	i = s[0];
	printf("%c %d\n", i, strlen(&s[0]));
	i = s[4];
	printf("%c %d\n", i, strlen(&s[4]));

	return;
}
