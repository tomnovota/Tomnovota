soucet_polozek(50).

celkova_castka(50).

spravna_celkova_castka() :- % 'ano' / 'ne'
    soucet_polozek(A), celkova_castka(A). % Chybná celková částka.