; printing 16-bits BCD number passed in ax to standard output in decimal format
; karol janic 2023


global _start

section .text
_start:
    mov     ax, 10h
    call    print_ax

    mov     eax, 0Ah 
    push    eax
    mov     edx, 1      ; chars number
    mov     ecx, esp    ; number = line feed
    mov     ebx, 1      ; standard output
    mov     eax, 4      ; write
    int     80h

    mov     ebx, 0
    mov     eax, 1
    int     0x80

print_ax:
    mov     ecx, 1

    mov     bx, ax
    and     bx, 0x000F
    movzx   edx, bx
    add     edx, 48
    push    edx

extract_numbers:
    shr     ax, 4
    cmp     ax, 0
    je      print

    mov     bx, ax
    and     bx, 0x000F 
    movzx   edx, bx
    add     edx, 48
    push    edx

    add     ecx, 1
    jmp     extract_numbers

print:
    mov     esi, 0
    mov     edi, ecx

print_loop:
    cmp     esi, edi
    je      end_loop

    mov     edx, 1      ; numbers number
    mov     ecx, esp    ; number address
    mov     ebx, 1      ; standard output
    mov     eax, 4      ; write
    int     80h
    pop     eax
    
    add     esi, 1
    jmp     print_loop

end_loop:
    ret
