#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

//ymh.20130403 Implement GetTickCount(), which is a MFC API, in Linux.
long GetTickCount()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

#define YMHDBG_LOG	"c:\\ymhdbg.log"

#define YMHDBG_FUNC(F,M)	static const char __FUNC__[] = F;\
				static unsigned long __MASK__ = M;

#define YMHDBG_ENTRY()	{\
                        	FILE *fp;\
                        	fp = fopen(YMHDBG_LOG, "a");\
                        	if (fp != 0 ) {\
                        	        fprintf(fp, "%d: --> %s\n", GetTickCount(), __FUNC__);\
                        	        fclose(fp);\
                        	} else {\
                        	        printf("open fail\n");\
                        	}\
                        }

#define YMHDBG_EXIT()	{\
				FILE *fp;\
				fp = fopen(YMHDBG_LOG, "a");\
				if (fp != 0 ) {\
					fprintf(fp, "%d: <-- %s\n", GetTickCount(), __FUNC__);\
					fclose(fp);\
				} else {\
					printf("open fail\n");\
				}\
			}

#define YMHDBG_CODE_BASE (0x80000000)
#define YMHDBG_CODE_TEST (YMHDBG_CODE_BASE | 0x00000001)

#define YMHDBG_MASK_BASE (0x40000000)
#define YMHDBG_MASK_ALL  (YMHDBG_MASK_BASE | 0x0FFFFFFF)
#define YMHDBG_MASK_FUNC (YMHDBG_MASK_BASE | 0x00000002)
#define YMHDBG_MASK_TIMER (YMHDBG_MASK_BASE | 0x00000004)

static int ymhDbgCounter = 0;
static void *ymhDbgStoragePtr = NULL;
static int ymhDbgflag = 0;

void YMHDBG_MSG(const char *fmt, ...)
{
	FILE *fp;
	va_list ap;

	fp = fopen(YMHDBG_LOG, "a");
	
	va_start(ap, fmt);
	if (fp != 0 ) {
		vfprintf( fp, fmt, ap);
	} else {
		printf("open fail\n");
	}
	va_end(ap);

	if (fp != 0 ) {		
		fclose(fp);
	}
}


void main( void )
{
	YMHDBG_FUNC( "main()", YMHDBG_MASK_FUNC );
	YMHDBG_ENTRY();
	YMHDBG_MSG("test %s\n", "output");
	printf("%x %x\n", YMHDBG_MASK_ALL, YMHDBG_MASK_ALL & YMHDBG_MASK_FUNC );
	printf("%x %x\n", YMHDBG_MASK_ALL, YMHDBG_MASK_ALL & YMHDBG_MASK_FUNC & YMHDBG_MASK_TIMER);
	YMHDBG_EXIT();
}
