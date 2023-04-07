; printing prime numbers from range [2, eax] to standard output
; karol janic 2023

%include        'print_eax.asm'

global _start

section .text
_start:
    mov     eax, 100000

    sub     eax, 2          ; interval size
    shl     eax, 2          ; interval size in bytes ; eax = 4 * eax
    mov     edi, esp
    add     edi, 4
    sub     esp, eax        ; allocate space on stack for array with numbers
    mov     edx, esp        ; array is from edi to edx
    mov     esi, edi
    
mark_all_as_primes:
    cmp     esi, edx 
    je      find_primes
    mov     DWORD[esi], 1
    sub     esi, 4
    jmp     mark_all_as_primes

find_primes:
    mov     esi, edi
    mov     ecx, 2

find_primes_loop1: 
    cmp     esi, edx 
    je      print_result

    mov     eax, [esi]
    cmp     eax, 1
    je prime
    jmp not_prime

prime:
    mov     ebx, 1
    call    find_primes_loop2
    sub     esi, 4
    add     ecx, 1
    jmp     find_primes_loop1

not_prime:
    sub     esi, 4
    add     ecx, 1
    jmp     find_primes_loop1

find_primes_loop2:
    mov     eax, ebx
    shl     eax, 2          ; eax = eax * 4
    sub     eax, esi
    neg     eax             ; eax = esi - 4 * ebx
    cmp     eax, edx
    jle     loop2_end
    mov     eax, ecx 
    add     eax, ebx 
    push    edx             ; store edx
    xor     edx, edx
    idiv    ecx 
    cmp     edx, 0
    pop     edx             ; restore edx
    je      loop2_zero_reminder
    jmp     loop2_nonzero_reminder

loop2_zero_reminder:
    mov     eax, ebx

    shl     eax, 2          ; eax = eax * 4
    sub     eax, esi
    neg     eax             ; eax = esi - 4 * ebx

    mov     DWORD[eax], 0

    add     ebx, 1
    jmp     find_primes_loop2

loop2_nonzero_reminder:
    add     ebx, 1
    jmp     find_primes_loop2

loop2_end:
    ret

print_result:
    mov     esi, edi
    mov     ecx, 2

print_result_loop:
    cmp     esi, edx 
    je      exit
    mov     eax, [esi]
    cmp     eax, 1
    je      prime_number
    jmp     composite_number
    
prime_number:
    mov     eax, ecx
    call    print_eax
    sub     esi, 4
    add     ecx, 1
    jmp print_result_loop

composite_number:
    sub     esi, 4
    add     ecx, 1
    jmp print_result_loop

exit:    
    mov     ebx, 0
    mov     eax, 1
    int     0x80