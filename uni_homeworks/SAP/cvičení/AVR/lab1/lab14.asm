.cseg
; definice pro nas typ procesoru
.include "m169def.inc"
; podprogramy pro praci s displejem
.org 0x1000
.include "print.inc"
; Zacatek programu - po resetu
.org 0
jmp start
.org 0x100
delka: .db 6 ; definice read-only konstanty v pameti programu (jeden bajt s hodnotou 6) 1
mezera1: .db "          "
retez: .db "ZDRAVIM PANA DOKTORA KUBALIKA",0 ; retezec zakonceny nulou (nikoli znakem "0") 1
mezera2: .db "         ",0

.DEF znak = r16
.DEF pozice = r17
.DEF delka_displej = r18
.DEF zaloha1 = r19
.DEF zaloha2 = r20
.DEF i = r21
.DEF i1 = r22
.DEF i2 = r23
.DEF i3 = r24
.DEF zakonceni = r25
.DEF Z1 = r30
.DEF Z2 = r31

start:
    ; Inicializace zasobniku
    ldi znak, 0xFF
    out SPL, znak
    ldi znak, 0x04
    out SPH, znak

    ; Inicializace displeje
    call init_disp

	; Ulozeni hodnoty 'delka' do promenne 'delka_displej'
	ldi Z1, low (2*delka)
	ldi Z2, high (2*delka)
	lpm delka_displej, Z

	; Naplneni registru 'Z' adresou zadaneho retezce
    ldi Z1, low(2*retez)
    ldi Z2, high(2*retez)

	; Posune ukazatel na retezec o 'delku_displeje' zpet a tim doda mezery pred retezec aby na displej "prijel"
	mov i, delka_displej
foo1:
	ld znak, -Z
	dec i
	cpi i, 0
	brne foo1

	; Kopie registru Z do registru Y
	movw Y, Z
	; Kopie registru Z pro obnoveni v podprogramu reset
	mov zaloha1, Z1
	mov zaloha2, Z2
	
	; Inicializace promenne zakonceni
	ldi zakonceni, 0
	; Volani hlavniho podprogramu
	call jezdici_text

end: 
	jmp end

	; Po skonceni slova se registr vrati do puvodniho stavu
reset:
	cpi zakonceni, 0
	brne znovu
	inc zakonceni
	ld znak, Z+
	jmp jezdici_text
znovu:
	dec zakonceni
	mov Z1, zaloha1
	mov Z2, zaloha2
	movw Y, Z ; Kopie registru Z do registru Y

	; Jedna rotace retezce
jezdici_text:
	lpm znak, Z ; Znak z adresy v Z uloz do promenne 'znak'
	cpi znak, 0
	breq reset ; Pokud je znak 0, tak je konec
	call obrazovka
	jmp jezdici_text

	; Zobrazi znaky, ktere se vejdou na displej, a pote posune ukazatel na retezci o jeden znak dal
obrazovka:
	ldi i, 0 ; Deklaruje promennou pro inkrement
	ldi pozice, 2 ; Nastavi pozici na displeji na pocatek
	movw Z, Y
	call vykresli
	adiw Y, 1 ; Posune ukazatel na retezci o jeden dal
	ldi i3, 25 ; Nastavi inkrement pro cekaci smycku a zavola ji
	call loop1
	ret

	; Vykresluje znaky dokud nezaplni obraz
vykresli:
	lpm znak, Z+ ; Ulozi znak na adrese v Z do promenne 'znak' a posune ukazatel o jedno dal
	cpi znak, 0
	brne skok
	ldi znak, ' '
skok:
	call show_char ; vykresli znak na displej
	inc pozice ; inkrementuje pozici na displeji
	inc i ; inkrementuje kontrolni promennou pro velikost displeje
	cp i, delka_displej ; pokud je zaplneni cely displej tak se vykreslovani ukonci
	brne vykresli
	ret

	; Cekaci smycka
loop1:
	ldi i1, 50
loop2:
	ldi i2, 50
wait:
	dec i2
    brne wait
    dec i1
    brne loop2
	dec i3
	brne loop1
	ret

