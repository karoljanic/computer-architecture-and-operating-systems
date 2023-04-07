; fast multiplication: eax = ebx * edx
; karol janic 2023


%include        'print_eax.asm'

global _start

section .text
_start:
    mov     ebx, 11
    mov     edx, 13

multiplication:
    mov     eax, 0
    mov     ecx, 0
mul_loop:
    push    ebx
    and     ebx, 1
    cmp     ebx, 1
    jne     is_not_power
    push    edx
    shl     edx, cl
    add     eax, edx
    pop     edx

is_not_power:
    pop     ebx
    inc     ecx
    shr     ebx, 1
    cmp     ebx, 0
    je      end_mul
    jmp     mul_loop

end_mul:
    call    print_eax

    mov     ebx, 0
    mov     eax, 1
    int     0x80

