#if defined(_WIN64)
# if defined(__cplusplus)
extern "C" {
# endif
void  __cdecl ymhPort80ABFF();
# if defined(__cplusplus)
} ;
# endif
#else
void ymhPort80ABFF() { return; }
#endif

void main()
{
	ymhPort80ABFF();
}
