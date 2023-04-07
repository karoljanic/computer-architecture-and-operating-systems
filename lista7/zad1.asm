; printing 32-bits number passed in eax to standard output in hexadecimal format
; karol janic 2023


global _start

section .text
_start:
    mov     eax, 3E5FD9h
    call    print_eax

    mov     ebx, 0
    mov     eax, 1
    int     0x80

print_eax:
    push    eax
    push    ebx
    push    ecx
    push    edx
    push    esi

    mov     ecx, 0

.divide_loop:
    inc     ecx
    mov     edx, 0
    mov     esi, 16
    idiv    esi
    
    cmp     edx, 9
    jg      .is_letter
    add     edx, 48     ; is digit
    jmp     .end_if
.is_letter:
    add     edx, 55     ; is letter
.end_if:

    push    edx
    cmp     eax, 0
    jnz     .divide_loop

.print_loop:
    dec     ecx
    mov     eax, esp
    push    ecx

    mov     edx, 1      ; chars number
    mov     ecx, eax    ; number
    mov     ebx, 1      ; standard output
    mov     eax, 4      ; write
    int     80h

    pop     ecx
    pop     eax
    cmp     ecx, 0
    jnz     .print_loop

.exit:
    mov     eax, 0Ah 
    push    eax
    mov     edx, 1      ; chars number
    mov     ecx, esp    ; number = line feed
    mov     ebx, 1      ; standard output
    mov     eax, 4      ; write
    int     80h

    pop     eax
    pop     esi
    pop     edx
    pop     ecx
    pop     ebx
    pop     eax

    ret
