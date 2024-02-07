; definice pro nas typ procesoru
.include "m169def.inc"
; podprogramy pro praci s displejem
.org 0x1000
.include "print.inc"

; Zacatek programu - po resetu
.org 0
jmp start

; Zacatek programu - hlavni program
.org 0x100
start:
    ; Inicializace zasobniku
	ldi r16, 0xFF
	out SPL, r16
	ldi r16, 0x04
	out SPH, r16
    ; Inicializace displeje
	call init_disp

	; *** ZDE muzeme psat nase instrukce
	ldi r19, 0x11	
	ldi r18, 0x75
	ldi r17, 0xF3
	ldi r16, 0x8C
	ldi r23, 0x2D
	ldi r22, 0xFE
	ldi r21, 0x2F
	ldi r20, 0xAE
;	---------------------------------------------
	add r16, r20
	adc r17, r21
	adc r18, r22
	adc r19, r23
	BRVS exception
	LDI r16, 'O'	; znak
	LDI r17, 2      ; pozice (zacinaji od 2)
	CALL show_char  ; zobraz znak

	LDI r16, 'K'	; znak
	LDI r17, 3      ; pozice (zacinaji od 2)
	CALL show_char  ; zobraz znak

end: JMP end        ; Zastavime program - nekonecna smycka

exception:
	LDI r16, 'V'	; znak
	LDI r17, 2      ; pozice (zacinaji od 2)
	CALL show_char  ; zobraz znak
	JMP exception
