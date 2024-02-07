; definice pro nas typ procesoru
.include "m169def.inc"
; podprogramy pro praci s displejem
.org 0x1000
.include "print.inc"

; Zacatek programu - po resetu
.org 0
jmp start

.org 0x100
delka: .db 6 ; definice read-only konstanty v pameti programu (jeden bajt s hodnotou 6)
retez: .db "NEBUDE-LI PRSET NEZMOKNEM",0 ; retezec zakonceny nulou (nikoli znakem "0")

; Zacatek programu - hlavni program
.org 0x200
start:

    ; Inicializace zasobniku
	ldi r16, 0xFF
	out SPL, r16
	ldi r16, 0x04
	out SPH, r16
    ; Inicializace displeje
	call init_disp

	; *** ZDE muzeme psat nase instrukce
	ldi r30, low(2*retez) ; 2
    ldi r31, high(2*retez)

    lpm r16, Z+           ; 3
    ; ... nejaka uzitecna cinnost ...
    lpm r16, Z+           ; nahraj dalsi bajt (znak) z retezce do r16, posun pozici v retezci
    ; ...

end: jmp end        ; Zastavime program - nekonecna smycka
