global _start

section .text
_start:
    mov     edx, hellolen 
    mov     ecx, hello 
    mov     ebx, 1
    mov     eax, 4 
    int     0x80

    mov     ebx, 0
    mov     eax, 1
    int     0x80

section .data 
hello:      db 'Hello world!', 0Ah
hellolen:   equ $-hello