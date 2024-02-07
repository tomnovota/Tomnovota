% (c) FIT CVUT, 
% BI-ZNS ZS2021, logicky program: jednoduchy expertni system k identifikaci dopravniho prostredku
% kodovani cestiny UTF8
% demonstrace principu
% podrobnosti naleznete na strance cviceni 4


% hlavni telo programu, tento kod je zodpovedny za vypsani textu uvadejiciho uzivatele do problematiky, volani dotazu a vypis reseni

main :- identifikace.

identifikace:-
  retractall(known(_,_,_,_)),     
  writeln('Vítá vás jednoduchý expertní systém pro rozpoznávání dopravního prostředku.'),
    writeln('Prosím odpovídejte na dotazy ano nebo ne. Každou odpověď je třeba zakončit tečkou.'),
  druh(X),
  write('Popsaný dopravni prostredek je '),write(X),  write('.'), nl.
identifikace:-
  write('Nejsem schopen provést identifikaci.'),nl.

 
% baze znalosti

druh(cervene_auto):-
  komunikace(silnice), velikost(maly), barva(cervena). 
druh(modre_auto):-
  komunikace(silnice), velikost(maly), barva(modra). 
druh(modre_nakladni_auto):-
  komunikace(silnice), velikost(velky), barva(modra). 
druh(cervena_lokomotiva):-
  komunikace(zeleznice), velikost(velky), barva(cervena). 
druh(modra_lokomotiva):-
  komunikace(zeleznice), velikost(velky), barva(modra). 

% zavolani dotazu (otázka, identifikátor atributu, hodnota)
   
komunikace(X):- dotaz('Kde se dorpavni prostredek pohybuje: ', komunikace, X).
velikost(X):- dotaz('Jakou má velikost, je ',velikost,X).
barva(X):- dotaz('Jakou má barvu: ',barva,X).

% otestuje, zda je zaznam odpovedi ano pro danou kombinaci atributu a hodnoty jiz v bazi faktu
dotaz(O,X,Y):-
  known(ano,O,X,Y),  !.       
                           
% otestuje, zda je zaznam odpovedi ne pro danou kombinac atribut a hodnoty jiz v bazi faktu
dotaz(O,X,Y):-
  known(ne,O, X,Y),     
  !, fail.
       
% otestuje, zda je v bazi faktu zanznam odpovedi ano pro dany atribut bez ohledu na jeho hodnotu
dotaz(O,X,_):-
  known(ano,O,X,_),  !, fail.

% dotaz, zobrazi otazku a nacte hodnotu ze vstupu, parametry predikátu dotaz: otázka, identifikátor atributu, hodnota
% otazku zadavame jako retezec

dotaz(O,X,Y):-
write(O), write(Y),
write('? (ano nebo ne): '),
read(A),                          
asserta(known(A,O,X,Y)),            
A = ano.      



