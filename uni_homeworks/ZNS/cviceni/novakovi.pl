/* (c) FIT ČVUT v Praze, BI-ZNS 2021
Logický program - reprezentace znalosti o rodine novakovych */

%muzi

muz(adam).
muz(bohumil).
muz(borek).
muz(evzen).
muz(dan).
muz(david).
muz(cyril).

%zeny

zena(anna).
zena(bara).
zena(eva).
zena(dita).
zena(cecilie).

%vztahy manzelstvi

manzel(anna, adam).
manzel(eva, borek).
manzel(bara, evzen).

%rodic(R,D) R je rodicem D

rodic(anna, borek).
rodic(anna, bohumil).
rodic(anna, bara).
rodic(adam, borek).
rodic(adam, bohumil).
rodic(adam, bara).
rodic(eva, dan).
rodic(eva, dita).
rodic(eva, david).
rodic(borek, dan).
rodic(borek, dita).
rodic(borek, david).
rodic(bara, cyril).
rodic(bara, cecilie).
rodic(evzen, cyril).
rodic(evzen, cecilie).

% O je otcem D.
otec(O, D) :- 
  rodic(O, D), muz(O).

% X je synem R.
syn(X, R) :- 
  rodic(R, X), muz(X).

% M je matkou D.
matka(M, D) :- 
  rodic(M, D), zena(M).

dcera(D, R) :- 
  rodic(R, D), zena(D).

sourozenec(X, Y) :- 
  rodic(R, X), rodic(R, Y), X \= Y.

sestra(X,Y) :- 
  sourozenec(X,Y), zena(X).

babicka(B,X) :- 
  rodic(P,X), matka(B,P).

deda(D,X) :- 
  rodic(P,X), otec(D,P).

prarodic(PR,X) :- 
  rodic(P,X),  rodic(PR,P).

vnuk(V,X) :-
  vnouce(V,X), muz(V).

vnucka(V,X) :-
  vnouce(V,X), zena(V).

vnouce(V,X) :-
  rodic(X,C), rodic(C,V).
  
manzelka(X,Y) :-
  manzel(Y,X).
  
dite(X,Y) :-
  rodic(Y,X).

% rekurze
predek(A, S) :- rodic(A, S). 
predek(A, S) :- rodic(A, A2), predek(A2, S).
