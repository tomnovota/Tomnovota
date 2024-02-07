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
	ldi r16, 0xD4
	;ldi r16, 0xF3
	;ldi r16, 0xBD
;	duplicita
	mov r18, r16
;	maska
	ldi r20, 0x0F
;	ziskani dolnich ctyrech bitu
	and r18, r20
;	invertovani masky
	com r20
;	ziskani hornich ctyrech bitu
	and r16, r20
	lsr r16
	lsr r16
	lsr r16
	lsr r16
;	cislo k porovnani
	ldi r21, 0x0A
;	zjistit jestli je cislo vyjadrene pismenem nebo cislici (1)
	ldi r22, 7
	cp r16, r21
	brge increase1
;	vypise na displej cislici (1)
foo1:
;	prevod do ASCII
	ldi r23, 48
	add r16, r23
;	cislo na displej
	ldi r17, 2      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak
;	presun druhe cislice do r16 (kvuli podprogramu increase)
	mov r16, r18
;	zjistit jestli je cislo vyjadrene pismenem nebo cislici (2)
	cp r16, r21
	brge increase2
;	vypise na displej cislici (2)		
foo2:
;	prevod do ASCII
	add r16, r23
;	cislo na displej
	ldi r17, 3      ; pozice (zacinaji od 2)
	call show_char  ; zobraz znak
end:
	jmp end        ; Zastavime program - nekonecna smycka

; pricte k cislu +7 aby se dostalo do hodnoty, kterou má v ASCII
increase1:
	add r16, r22
	jmp foo1
increase2:
	add r16, r22
	jmp foo2
