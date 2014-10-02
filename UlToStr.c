void UlToStr(CHAR16 *s, unsigned long bin, unsigned char n)
{
    s += n;
    *s = '\0';

    while (n--)
    {
        *--s = (bin % 10) + '0';
        bin /= 10;
    }
    return;
}

void main()
{
	CHAR16 s[33];
	CHAR16 n[] = { '\n', '\r' };
	int a = 123456;

	UlToStr( s, a, 32 );
	ST->ConOut->OutputString(ST->ConOut, s );
	ST->ConOut->OutputString(ST->ConOut, n );
}
