; calculating ln(x) using x87
; karol janic 2023


global _start

section .text

_start:
    finit

    fld1                ; load 1                        
    fldl2e              ; load lg(e)                    
    fdiv                ; div st(1) / st(0)          
    fld     DWORD [x]   ; load x              
    fyl2x               ; calculate st(1) * lg(st(0))

    fst     DWORD [x]

    mov     eax, 1
    mov     ebx, 0
    int     0x80

section .data 
x:      dd 0.21


; ln(x) = lg(x) / lg(e)
