# Cvičení 2 - C++ třídy (class)

V tomto cvičení se seznámíme s třídami v C++, tj., definice třídy, její metody, atributy, přístupová práva.

## 1) Zadaní pro společnou práci

V tomto cvičení použijeme příklad z cvičebnice PA2, a sice:

### a) Základní použití třídy
Implementujte třídu `SimpleRegex`, která bude mít metody `setPattern` a `matchText`.
Metoda `setPattern` nastaví pattern (vzor), kterým se bude testovat vstupní řetězec.
Metoda `matchText` bude vracet boolean indikující, zda zadaný text odpovídá patternu.
Pro jednoduchost uvažujte, že pattern může obsahovat pouze zástupný znak **tečka** `.`, kterým je možné nahradit právě *jeden* libovolný znak.

Svůj kód rozdělte do příslušných souborů, tj. definice třídy do `.h` a její implementace do `.cpp`.

### b) Statické atributy
Následne dodělejte počítadlo zavolání metody `matchText`.
Toto počítadlo bude sdílené mezi všemi instancemi třídy `SimpleRegex` a bude tedy udávat,
kolikrát byla zavolána tato metoda v součtu pro všechny instance třídy. Počet volání vracejte pomocí metody `getCounter()`.

Použijte testovací funkci `main()` ze souboru `regexmain.cpp`.


## 2) Zadaní pro samostatnou práci

Proveďte celé zadání bodu **1)** a navíc rozšiřte třídu `SimpleRegex` o možnost přidat do patternu znak **hvězdička** `*`,
který bude zástupným symbolem pro *nezáporný* (tj. i nulový) počet libovolných znaků.

Použijte testovací funkci `main()` ze souboru `regexmain_hard.cpp`.

