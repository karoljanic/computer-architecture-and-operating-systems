; fast division: eax = int(ebx / edx); ebx = rem(ebx / edx)
; karol janic 2023


%include        'print_eax.asm'

global _start

section .text
_start:
    mov     ebx, 48
    mov     edx, 13

division:
    mov     eax, 0
    mov     ecx, 0
    mov     esi, ebx

bits_number_counter:
    cmp     esi, 0
    je      div_loop
    inc     ecx
    shr     esi, 1
    jmp     bits_number_counter

div_loop:
    cmp     ecx, 0
    jl      end_div
    mov     esi, edx
    shl     esi, cl
    cmp     esi, ebx
    jg      to_big
    sub     ebx, esi
    mov     esi, 1
    shl     esi, cl
    add     eax, esi
to_big:
    dec     ecx
    jmp     div_loop

end_div:
    call    print_eax

    mov     eax, ebx
    call    print_eax

    mov     ebx, 0
    mov     eax, 1
    int     0x80
