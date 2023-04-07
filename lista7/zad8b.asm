; calculating e^x using x87
; karol janic 2023


global _start

section .text

_start:
    finit

    fldl2e                  ; load log(e)
    fmul    DWORD [x]       ; mul st(0) * x
    fld1                    ; load 1
    fldl2e                  ; load log(e)
    fmul    DWORD [x]       ; mul st(0) * x
    fprem                   ; reminder of st(0) / st(1)
    f2xm1                   ; 2^(st(0)) - 1
    faddp                   ; add 1 to st(0)
    fscale                  ; st(0) * 2^int(st(1))
    fstp                    ; pop st(1)

    fst     DWORD [x]

    mov     eax, 1
    mov     ebx, 0
    int     0x80

section .data 
x:      dd 3.14


; e = 2^(lg(e))
; e^x = 2^(lg(e))^x = 2^(x*lg(e))
; ale mozemy podnosic do poteg z zakresu (0, 1) albo scalowac przez potegi 2, wiec:
; e^x = 2^frac(x*lg(e)) * 2^int(x*lg(e))
