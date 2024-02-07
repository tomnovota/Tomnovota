% (c) FIT CVUT 
% BI-ZNS ZS2021, logicky program: jednoduchy expertni system k identifikaci hmyzu (vyber z mnoziny obsahujici 9 kategorii)
% demonstrace principu
% podrobnosti a vyklad naleznete na strance cviceni 4


% hlavni telo programu, tento kod je zodpovedny za vypsani textu uvadejiciho uzivatele do problematiky, volani dotazu a vypis reseni

main :- identifikace.

identifikace:-
  retractall(known(_,_,_,_)),       
  writeln('Vítá vás jednoduchý expertní systém pro vybraných druhů hmyzu.'),
    writeln('Prosím odpovídejte na dotazy ano nebo ne. Každou odpověď je třeba zakončit tečkou.'), nl,
  druh(X), nl,
  write('Popsaný hmyz je '),write(X),  write('.'), nl.
identifikace:-
  write('Nejsem schopen provést identifikaci druhu.'), 
  rad(X) ,nl,  write('Rad popsaneho hmyzu je '),write(X),  write('.'), nl.
  identifikace:-
  write('Nejsem schopen provést identifikaci radu.'), nl.


% ----------------------
% Báze znalostí
% ----------------------
druh('babočka kopřivová'):-
  rad(motyli),
  skupina(motylicz),
  barva('oranžová se skvrnami').
druh('acraea acrita'):-
  rad(motyli),
  zije('Afrika'),
  barva('oranžová se skvrnami').
druh('bělopásek távolníkový'):-
  rad(motyli),
  skupina(motylicz),
  barva('černá s bílými skvrnami').
druh('modrásek jehlicovitý'):-
  rad(motyli),   
  skupina(motylicz),
  barva('modrá').
druh('bělásek zelný'):-
  rad(motyli),   
  skupina(motylicz),
  barva('bílá').
druh('žluťásek jižní'):-
  rad(motyli),
  barva('žlutá').
druh('včela medonosná'):-
  rad(blanokridli),
  celed(vceloviti),
  obrana('bodnutí žihadlem').
druh('čmelák lesní'):-
  rad(blanokridli),
  celed(vceloviti),
  zije('Česká republika'),
  telo('zavalité').
druh('žlutnatka obecná'):-
  skupina(mouchy),  
  barva('žlutá').
druh('bzučivka obecná'):-
  skupina(mouchy),                 
  barva('černá').
druh('slunéčko sedmitečné'):- 
  kridla('krovky i blanitá'), 
  celed(slunecka),               
  barva('červená s černými tečkami'),
  pocettecek(sedm).


rad(motyli):-
  kridla('velká s šupinkami'),
  velikost('4-5 cm'),
  larva(housenka).
rad(dvoukridli):-
  kridla('blanitá'), 
  kridlapocet('dvě').
rad(blanokridli):-
  kridla('blanitá'),
  kridlapocet('dva páry').

celed(slunecka):-
  dravost(hmyz).
celed(vceloviti):-
  zvuk('bzučí'), 
  opylovac('opylovač'). 

skupina(mouchy):-
  rad(dvoukridli),
  zvuk('bzučí').
skupina(motylicz):-
  zije('Česká republika').


% ----------------------
% Uživatelské rozhraní
% ----------------------

% ziskani hodnoty atributu od uzivatele

kridla(X):- dotaz('Má křídla: ', kridla, X).
kridlapocet(X):-dotaz('Má křídla: ', kridlapocet , X).
barva(X):- dotaz('Převládá na těle jedince barva ', barva, X).
velikost(X):- dotaz('Velikost jedince je ', velikost, X). 
larva(X):- dotaz('Je larva ',larva, X).
zije(X):- dotaz('Žije v: ', zije, X).
obrana(X):- dotaz('Bráni se ', obrana, X).
zvuk(X):- dotaz('Zvuk: ', zvuk, X).
pocettecek(X):- dotaz('Počet teček je ',pocettecek, X).
telo(X):- dotaz('Má tělo ', telo,X).
dravost(X):-dotaz('Loví jiný ', dravy, X).
opylovac(X):-dotaz('Je to ', opyluje, X).

% uzivatelske rozhrani, implementace klauzule dotaz
             
% otestuje, zda je zaznam odpovedi ano pro danou kombinaci atributu a hodnoty jiz v bazi faktu
dotaz(O,X,Y):-
  known(ano,O,X,Y),  !.       
                           
% otestuje, zda je zaznam odpovedi ne pro danou kombinac atribut a hodnoty jiz v bazi faktu
dotaz(O,X,Y):-
  known(ne,O, X,Y),     
  !, fail.
   
     
% otestuje, zda je v bazi faktu zaznam odpovedi ano pro dany atribut bez ohledu na jeho hodnotu

dotaz(O,X,_):-
  known(ano,O,X,_),  !, fail.


% dotaz, zobrazi otazku a nacte hodnotu ze vstupu
dotaz(O,X,Y):-
write(O), write(Y),
write('? (ano nebo ne): '),
read(A),                          
asserta(known(A,O,X,Y)),            
A = ano.      


