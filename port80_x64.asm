format MS64 COFF

section '.text' code readable executable

public ymhPort80ABFF

ymhPort80ABFF:

    mov eax, 0xABFF
    mov dx, 80h
    out dx, eax
    ret
