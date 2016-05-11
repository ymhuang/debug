#include <stdio.h>

char s[512] = "SMI:AOB29C01";

void main(void)
{
	int i;

        if ( i >= 0 && i <= 9 ) printf("aa\n");
        printf("%X\n", (char *)s[0]);

	return;
}
