; printing prime numbers from range [2, x](x is defined in .data section) to standard output using Erastotenes Sieve
; karol janic 2023


%include        'print_eax.asm'

global _start

section .data
array_size  equ 100


section .bss
array:  RESD array_size


section .text
_start:
    mov     esi, array
    mov     ecx, 0
    
set_zeros:
    mov     DWORD[esi], 1
    add     esi, 4
    inc     ecx
    cmp     ecx, array_size
    jne     set_zeros

    mov     DWORD[array], 0

find_primes:
    mov     ecx, 1
    mov     esi, array
find_primes_loop:
    inc     ecx
    add     esi, 4
    cmp     ecx, array_size
    jg      print_result
    mov     eax, DWORD[esi]
    cmp     eax, 1
    je      mark_composite
    jmp     find_primes_loop

mark_composite:
    mov     edx, ecx
    mov     edi, esi
mark_composite_loop:
    add     edx, ecx
    mov     eax, ecx
    shl     eax, 2
    add     edi, eax
    cmp     edx, array_size
    jg      find_primes_loop
    mov     DWORD[edi], 0

    jmp     mark_composite_loop

print_result:
    mov     esi, array
    mov     eax, 1
print_result_loop:
    mov     ebx, [esi]
    cmp     ebx, 0
    je      continue
    call    print_eax
continue:
    inc     eax
    add     esi, 4
    cmp     eax, array_size
    jle     print_result_loop

exit:
    mov     ebx, 0
    mov     eax, 1
    int     0x80