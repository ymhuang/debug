
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

void CharToStr(CHAR16 *s, char *c, unsigned char n)
{
    int i = 0;

    *(s+n-1) = '\0';

    while (c[i] != '\0')
    {
        *(s+i) = c[i];
        if ( ++i > n )
        {
            *(s+i) = '\0';
            break;
        }
        else
        {
            *(s+i) = '\0';
        }
    }
}

void UlToStrHEX(CHAR16 *s, unsigned long bin, unsigned char n)
{
    int tmp;

    s += n;
    *s = '\0';

    while (n--)
    {
        tmp = bin % 16;
        if ( (tmp >= 0) && (tmp <= 9) )
        {
            *--s = tmp + '0';
        }
        else if ( tmp == 10 )
        {
            *--s = 'A';
        }
        else if ( tmp == 11 )
        {
            *--s = 'B';
        }
        else if ( tmp == 12 )
        {
            *--s = 'C';
        }
        else if ( tmp == 13 )
        {
            *--s = 'D';
        }
        else if ( tmp == 14 )
        {
            *--s = 'E';
        }
        else if ( tmp == 15 )
        {
            *--s = 'F';
        }
        else
        {
            *--s = '?';
        }

        bin /= 16;
    }
    return;
}

void WaitForEnter ( void ) 
{
    EFI_INPUT_KEY key;

    ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L"Press ENTER to continue...\r\n" );

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

void DumpMem( char * p, unsigned long length )
{
    unsigned long i = 0;
    int j = 0;
    CHAR16 s[32];
    char t[17];

    ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L"Address: " );
    UlToStrHEX( s, (unsigned long)p, 16 );
    ST->ConOut->OutputString(ST->ConOut, s );
    ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L"\r\n" );

    do
    {
        UlToStrHEX( s, p[i], 2 );
        ST->ConOut->OutputString(ST->ConOut, s );
        if ( p[i] < 0x20 || p[i] > 0x7E )
        {
            t[j] = '.';
        }
        else
        {
            t[j] = p[i];
        }

        if ( ++j > 17 )
        {
            j = 0;
        }

        if ( ((i+1)%16) == 0 )
        {
            t[j] = '\0';
            CharToStr( s, t, j );
            ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L"  " );
            ST->ConOut->OutputString(ST->ConOut, s );
            ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L"\r\n" );
            j = 0;
        }
        else
        {
            if ( ((i+1)%8) == 0 )
            {
                ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L"-" );
            }
            else
            {
                ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L" " );
            }
        }

        i++;

    } while ( i <= length );
    ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)L"\r\n" );
    WaitForEnter();
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
