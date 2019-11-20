//DEBUG.ymhuang.begin
#define YMH_UEFI_DBG 1

#if (YMH_UEFI_DBG == 1)

#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>
#include <Library/DebugLib.h>

#define HEX (16)
#define DEC (10)

extern EFI_SYSTEM_TABLE *ymhST;
extern EFI_STATUS ymhInitLogFile(CHAR16 *s);
extern EFI_STATUS ymhWriteLogFile(CHAR16 *s);
extern EFI_STATUS ymhTerminateLogFile(CHAR16 *s);
extern void ymhIntToChar16(CHAR16 *s, UINTN i, UINTN size, UINTN base);
extern void ymhDumpMem(char *p, unsigned long length);

#endif /* (YMH_UEFI_DBG == 1) */
//DEBUG.ymhuang.end
