
% consult("hw01.pl").

% --------------------
% ŘÍDÍCÍ MECHANIZMUS
% --------------------

spustit :-
    identifikace.

zadat :-
    retractall(known(_,_,_)),
    v_poradku,
    nl,
    writeln("============================================."),
    nl,
    writeln("Zadaná účtenka je bez chyby."),
    nl,
    writeln("Kontrolu další účtenky můžete spustit příkazem 'zadat.'").

identifikace :-
    retractall(known(_,_,_)),
    writeln("   ========================================================================="),
    writeln("   |                                                                       |"),
    writeln("   |  VÍTÁ VÁS JEDNODUCHÝ EXPERTNÍ SYSTÉM NA URČOVÁNÍ SPRÁVNOSTI ÚČTENEK.  |"),
    writeln("   |                                                                       |"),
    writeln("   ========================================================================="),
    nl,
    writeln("| ODPOVÍDEJTE NA OTÁZKY |"),
    writeln("v                       v"),
    nl, zadat.

v_poradku :-
    not(chyba_celkova_castka),
    not(chyba_rekapitulace),
    not(chyba_datum),
    not(chyba_ico_dodavatele),
    not(chyba_dic_dodavatele),
    not(chyba_dph),
    not(chyba_cislo_dokladu),
    not(chyba_ico_odberatele),
    not(chyba_dic_odberatele),
    not(chyba_udaje_o_zapisu),
    not(chyba_datum_uskutecneni),
    not(chyba_castka).

% ----------------------------------------
% DOTAZOVACI A VYHODNOCOVACI MECHANIZMUS
% ----------------------------------------

dotaz(O,X) :-
    known(ano, O, X), !.
  
dotaz(O, X) :-
    known(ne, O, X), !,
    fail.
  
dotaz(O, X) :-
    writeln(O),
    write(" ('ano.' nebo 'ne.'): "),
    read(A), nl,
    asserta(known(A, O, X)),
    A = ano.

chyba(O) :-
    writeln("============================================."),
    nl,
    write("Neplatná účtenka! ("),
    write(O),
    writeln(")"),
    nl,
    writeln("Kontrolu další účtenky můžete spustit příkazem 'zadat.'").

% ----------------------
% BÁZE ZNALOSTÍ
% ----------------------

chyba_celkova_castka :-
    not(spravna_celkova_castka),
    chyba("Chybná celková částka.").

chyba_rekapitulace :-
    danovy_doklad,
    not(rekapitulace_dph),
    not(sazba_dph_na_uctence),
    chyba("Chybí rekapitulace DPH.").
    
chyba_datum :-
    not(spravne_datum),
    chyba("Chybí datum.").

spravne_datum :-
    datum, spravny_rok.

chyba_ico_dodavatele :-
    not(ico_dodavatele),
    chyba("Chybí IČO dodavatele.").

chyba_dic_dodavatele :-
    danovy_doklad,
    not(dic_dodavatele),
    chyba("Chybí DIČ dodavatele.").

chyba_dph :-
    potraviny,
    dph_deset,
    dph_deset_konzumace,
    chyba("Chybné sazby DPH.").

chyba_cislo_dokladu :-
    not(cislo_dokladu),
    chyba("Chybí číslo dokladu.").

chyba_ico_odberatele :-
    ico_odberatele,
    not(kontrola_ico),
    chyba("Chybné IČO odběratele.").

chyba_dic_odberatele :-
    danovy_doklad,
    zjednoduseny_danovy_doklad,
    not(kontrola_dic),
    chyba("Chybné DIČ odběratele.").
    
chyba_udaje_o_zapisu :-
    not(udaje_o_zapisu),
    chyba("Chybí údaje o zápisu dodavatele do obchodního nebo živnostenského rejstříku.").

chyba_datum_uskutecneni :-
    not(datum_uskutecneni),
    chyba("Chybí datum uskutečnění zdanitelného plnění.").

chyba_castka :-
    zjednoduseny_danovy_doklad,
    vysoka_celkova_castka,
    chyba("Jedná se o zjednodušený daňový doklad, ale celková částka je přes 10.000 Kč.").

% ---------------------------------------
% DOTAZY UŽIVATELI NA ATOMICKÉ ZNALOSTI
% ---------------------------------------

danovy_doklad :-
    dotaz("Je na dokladu uvedeno DIČ?", danovy_doklad).

zjednoduseny_danovy_doklad :-
    not(dotaz("Je na dokladu uveden název odběratele?", zjednoduseny_danovy_doklad)).

spravna_celkova_castka :-
    dotaz("Spočítejte celkový součet položek na dokladu, je roven celkové částce na dokladu?", spravna_celkova_castka).

sazba_dph_na_uctence :-
    dotaz("Je na účtence uvedena sazba DPH pro celkovou částku?", sazba_dph_na_uctence).

rekapitulace_dph :-
    dotaz("Je na dokladu rekapitulace DPH?", rekapitulace_dph).
    
spravny_rok :-
    dotaz("Je datum v roce 2021?", spravny_rok).

datum :-
    dotaz("Nachází se na účtence datum?", datum).

ico_dodavatele :-
    dotaz("Je uvedeno IČO dodavatele?", ico_dodavatele).
    
dic_dodavatele :-
    dotaz("Je uvedeno DIČ dodavatele?", dic_dodavatele).

potraviny :-
    dotaz("Jsou účtovány potraviny?", potraviny).

dph_deset :-
    dotaz("Jedná se o sazbu DPH 10%?", dph_deset).

dph_deset_konzumace :-
    dotaz("Jsou položky v sazbě DPH 10% konzumovány v prostorách dodavatele?", dph_deset_konzumace).

cislo_dokladu :-
    dotaz("Obsahuje účtenka číslo dokladu?", cislo_dokladu).

ico_odberatele :-
    dotaz("Nachází se na dokladu IČO odběratele?", ico_odberatele).

kontrola_ico :- 
    dotaz("Proveďte kontrolu IČO, je v pořádku?", kontrola_ico).

kontrola_dic :-
    dotaz("Proveďte kontrolu DIČ, je v pořádku?", kontrola_dic).

udaje_o_zapisu :-
    dotaz("Obsahuje účtenka údaje o zápisu dodavatele do obchodního nebo živnostenského rejstříku?", udaje_o_zapisu).

datum_uskutecneni :-
    dotaz("Nachází se na dokladu datum vystavení dokladu?", datum_uskutecneni).

vysoka_celkova_castka :-
    dotaz("Přesahuje celková částka hodnotu 10 000Kč?", vysoka_celkova_castka).

