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
	LDI R16, 100
	LDI R17, 10
	LDI R18, 58
; 4 * R16
	LSL R16
	BRVS exception
	LSL R16
	BRVS exception
; 3 * R17
	MOV R19, R17
	LSL R19
	BRVS exception
	ADD R17, R19
	BRVS exception
; R16 + R17 - R18
	ADD R16, R17
	BRVS exception
	SUB R16, R18
	BRVS exception
; () / 8
	ASR R16
	BRCS exception
	ASR R16
	BRCS exception
	ASR R16
	BRCS exception
; () -> R20
	MOV R20, R16

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
