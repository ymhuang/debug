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

void CharToStr(CHAR16 *s, char *c)
{
    int i = 0;
    while (c[i] != '\0')
    {
        *(s+i) = c[i];
        i++;
	*(s+i) = '\0';
    }
}

void WaitForEnter ( void ) 
{
    EFI_INPUT_KEY key;
    char * msg = "Press ENTER to continue";
    CHAR16 s[32];
    CHAR16 n[2] = {'\n', '\r'};

    CharToStr( s, msg );
    ST->ConOut->OutputString(ST->ConOut, s );
    ST->ConOut->OutputString(ST->ConOut, n );

    do
    {
        if ( ST->ConIn->ReadKeyStroke( ST->ConIn, &key ) == 0 )
        {
            if (key.UnicodeChar == 0x0D)
	    {
                break;
            }
        }
    } while (1);

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
