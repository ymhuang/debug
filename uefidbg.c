#include <Protocol/SimpleFileSystem.h>

#define HEX (16)
#define DEC (10)

EFI_SYSTEM_TABLE * ymhST = 0;

EFI_GUID ymhSimpFSProtGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

CHAR16 * ymhLogFile = (CHAR16 *)L"ymhdbg.log";
EFI_FILE_PROTOCOL * ymhFileHandle = NULL;
UINT16 ymhFileHandleOpenFlag = 0;

UINT16 ymhSimpFSProtRegFlag = 0;
EFI_EVENT ymhSimpFSProtEvent  = NULL;
void * ymhSimpFSProtReg = NULL;

typedef struct _ymhMsgType {
    CHAR16 * s;
    struct _ymhMsgType * next;
} ymhMsgType;
ymhMsgType * ymhMsgHead = NULL;
ymhMsgType * ymhMsgCurrent = NULL;

UINTN ymhCharLen( char * s )
{
    UINTN i = 0;
    char * p = NULL;

    p = s;
    while ( p[i] != '\0' )
    {
        ++i;
    }

    i += 1;

    return i;
}

UINTN ymhChar16Len( CHAR16 * s )
{
    UINTN i = 0;
    CHAR16 * p = NULL;

    p = s;
    while ( p[i] != '\0' )
    {
        ++i;
    }

    i = i * 2;

    return i;
}

void ymhIntToChar16( CHAR16 *s, UINTN i, UINTN size, UINTN base )
{
    CHAR16 tmp;

    s += size;
    *s = '\0';

    while ( size-- )
    {
        switch ( base )
        {
            case DEC:
                *--s = (i % 10) + '0';
                i /= 10;
                break;
            case HEX:
                tmp = i % 16;
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

                i /= 16;
                break;
        }
    }
    return;
}

void ymhCharToChar16( CHAR16 *s, char *c, UINTN size )
{
    UINTN i = 0;

    *(s+size-1) = '\0';

    while ( c[i] != '\0' )
    {
        *(s+i) = c[i];
        if ( ++i > size )
        {
            *(s+i) = '\0';
            break;
        }
        else
        {
            *(s+i) = '\0';
        }
    }

    return;
}

EFI_STATUS ymhWriteLogMem( CHAR16 * s )
{
    ymhMsgType * p = NULL;
    UINTN i = 0;
    UINTN size = 0;

    ymhST->BootServices->AllocatePool( EfiRuntimeServicesData, 
            sizeof(ymhMsgType), 
            &p );
    if ( NULL == p )
    {
        return EFI_OUT_OF_RESOURCES;
    }

    p->next = NULL;
    size = ymhChar16Len( s );
    ymhST->BootServices->AllocatePool( EfiRuntimeServicesData, 
            sizeof(CHAR16) * size, 
            &(p->s) );
    for ( i = 0; i < size; i++ )
    {
        (p->s)[i] = s[i];
    }

    if ( NULL == ymhMsgHead )
    {
        ymhMsgHead = ymhMsgCurrent = p;
    }
    else
    {
        ymhMsgCurrent->next = p;
        ymhMsgCurrent = p;
    }

    return EFI_SUCCESS;
}

EFI_STATUS ymhInitLogFile( CHAR16 * s );
EFI_STATUS ymhWriteLogFile( CHAR16 * s );
EFI_STATUS ymhDumpLogMem()
{
    ymhMsgType * p = ymhMsgHead;

    while ( p != NULL )
    {
        ymhWriteLogFile( (p->s) );
        ymhMsgCurrent = p;
        p = p->next;
        ymhST->BootServices->FreePool( (ymhMsgCurrent->s) );
        ymhST->BootServices->FreePool( ymhMsgCurrent );
    }

    return EFI_SUCCESS;
}

void ymhSimpFSProtInstalled( EFI_EVENT Event, VOID *Context )
{
    EFI_TPL oldTpl;

    ymhST->BootServices->CloseEvent( Event );
    ymhSimpFSProtEvent = NULL;

    oldTpl = ymhST->BootServices->RaiseTPL( TPL_HIGH_LEVEL );
    ymhST->BootServices->RestoreTPL( TPL_APPLICATION );

    ymhInitLogFile( (CHAR16 *)L"== BEGIN (ymhSimpFSProtInstalled)\r\n" );
    ymhDumpLogMem();

    ymhST->BootServices->RaiseTPL( oldTpl );

    return;
}

EFI_STATUS ymhRegSimpFSProt( void )
{
    EFI_STATUS status;

    if ( ymhSimpFSProtRegFlag == 1 )
    {
        // already register.
        return EFI_SUCCESS;
    }

    status = ymhST->BootServices->CreateEvent( EVT_NOTIFY_SIGNAL, TPL_NOTIFY, ymhSimpFSProtInstalled, NULL, &ymhSimpFSProtEvent );
    if ( EFI_ERROR(status) )
    {
        ymhSimpFSProtRegFlag = 0;
        return status;
    }

    status = ymhST->BootServices->RegisterProtocolNotify( &ymhSimpFSProtGuid, ymhSimpFSProtEvent, &ymhSimpFSProtReg );
    if ( EFI_ERROR(status) )
    {
        ymhST->BootServices->CloseEvent( ymhSimpFSProtEvent );
        ymhSimpFSProtEvent = NULL;
        ymhSimpFSProtRegFlag = 0;
        return status;
    }

    ymhSimpFSProtRegFlag = 1;

    return EFI_SUCCESS;
}

EFI_STATUS ymhInitLogFile( CHAR16 * s )
{
    EFI_STATUS status = EFI_ABORTED;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL * simpFS = 0;
    EFI_FILE_PROTOCOL * root = 0;
    UINTN size = 0;
    UINT16 textFileHeader = 0xFEFF;

    status = ymhST->BootServices->LocateProtocol( &ymhSimpFSProtGuid, NULL, (VOID **)&simpFS);
    if( EFI_ERROR(status) )
    {
        status = ymhRegSimpFSProt();
        if( EFI_ERROR(status) )
        {
            return status;
        }
        return status;
    }

    status = simpFS->OpenVolume( simpFS, &root );
    if ( EFI_ERROR(status) )
    {
        return status;
    }

    status = root->Open( root, &ymhFileHandle, ymhLogFile, (EFI_FILE_MODE_CREATE|EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE), 0 );
    if ( ymhFileHandle == 0 )
    {
        return EFI_ABORTED;
    }

    ymhFileHandleOpenFlag = 1;

    size = 2;
    status = ymhFileHandle->Write( ymhFileHandle, &size, &textFileHeader );
    if ( EFI_ERROR(status) )
    {
        status  = ymhFileHandle->Close( ymhFileHandle );
        if ( EFI_ERROR(status) )
        {
            return status;
        }
        ymhFileHandleOpenFlag = 0;
        return status;
    }

    size = ymhChar16Len( s );
    ymhFileHandle->Write( ymhFileHandle, &size, s );

    return EFI_SUCCESS;
}

EFI_STATUS ymhTerminateLogFile( CHAR16 * s )
{
    EFI_STATUS status;
    UINTN size = 0;

    if ( ymhFileHandleOpenFlag == 0)
    {
        return EFI_ABORTED;
    }

    size = ymhChar16Len( s );
    ymhFileHandle->Write( ymhFileHandle, &size, s );
    status  = ymhFileHandle->Close( ymhFileHandle );
    if ( EFI_ERROR(status) )
    {
        return status;
    }

    ymhFileHandleOpenFlag = 0;

    return EFI_SUCCESS;
}

EFI_STATUS ymhWriteLogFile( CHAR16 * s )
{
    EFI_STATUS status;
    UINTN size = 0;

    if ( ymhFileHandleOpenFlag == 0)
    {
        ymhWriteLogMem( s );
        return EFI_NOT_READY;
    }
    else
    {
        size = ymhChar16Len( s );
        status = ymhFileHandle->Write( ymhFileHandle, &size, s );
        if ( EFI_ERROR(status) )
        {
            return status;
        }
    }

    return EFI_SUCCESS;
}

void ymhDumpMem( char * p, unsigned long length )
{
    unsigned long i = 0;
    int j = 0;
    CHAR16 s[32];
    char t[17];

    ymhWriteLogFile( (CHAR16 *)L"Address: " );
    ymhIntToChar16( s, (UINTN)p, 32, HEX );
    ymhWriteLogFile( s );
    ymhWriteLogFile( (CHAR16 *)L"\r\n" );

    do
    {
        ymhIntToChar16( s, p[i], 2, HEX );
        ymhWriteLogFile( s );
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
            ymhCharToChar16( s, t, j );
            ymhWriteLogFile( (CHAR16 *)L"  " );
            ymhWriteLogFile( s );
            ymhWriteLogFile( (CHAR16 *)L"\r\n" );
            j = 0;
        }
        else
        {
            if ( ((i+1)%8) == 0 )
            {
                ymhWriteLogFile( (CHAR16 *)L"-" );
            }
            else
            {
                ymhWriteLogFile( (CHAR16 *)L" " );
            }
        }

        i++;

    } while ( i <= length );

    ymhWriteLogFile( (CHAR16 *)L" \r\n" );

    return;
}

void ymhWaitForEnter( void )
{
    EFI_INPUT_KEY key;

    ymhST->ConOut->OutputString( ymhST->ConOut, (CHAR16 *)L"==> Press ENTER to continue..." );

    do
    {
        /* Read key from real EFI console */
        if ( ymhST->ConIn->ReadKeyStroke( ymhST->ConIn, &key ) == 0 )
        {
            if (key.UnicodeChar == 0x0D)
            {
                break;
            }
        }
    } while (1);

    ymhST->ConOut->OutputString( ymhST->ConOut, (CHAR16 *)L"ENTER is pressed\r\n" );

    return;
}

//EFI_STATUS efi_main(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) ;
EFI_STATUS DxeEntry(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable )
{
    ymhST = SystemTable;
    ymhInitLogFile((CHAR16 *)L"== BEGIN (DxeEntry)\r\n");
    ymhWriteLogFile( (CHAR16 *)L"Test\r\n");
    ymhTerminateLogFile((CHAR16 *)L"== END (DxeEntry)\r\n");
}
