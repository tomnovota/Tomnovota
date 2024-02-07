# ÚVODNÍ STRÁNKA

### Tabulkový editor:
  - čtvercová tabulka složená z buněk
  - buňka obsahuje hodnotu nebo výraz
  - buňku lze použít ve výrazu jiné buňky
  - v případě vzorce:
    - buňka aktualizuje svou hodnotu na základě vzorce
    - výraz s cyklem je zakázaný
    - syntakticky nesprávný výraz je zakázaný

### Funkcionality:
  - čísla: sčítání, odčítání, násobení, dělení
  - řetězce: spojení, opakování
  - 3 jednoduché funkce: ABS, UPPER, LOWER
  - základní operace se mohou vnořovat pomocí závorek

### Práce s tabulkou:
  - obnovení tabulky ze souboru (včetně vzorců)
  - uložit tabulku do souboru v čitelné podobě
  - vypsat tabulku s hodnotami nebo vzorci do terminálu
  - v případě, že je v buňce výraz se špatnými operandy, vypíše se do buňky "#ERROR"
  - v případě, že buňka obsahuje výraz obsahující prázdnou buňku, bude při výpisu hodnot tato buňka také prázdná

## PRÁCE PROGRAMU

#### NAČTENÍ KONFIGURACE (implicitně):
- velikost nové tabulky
- znak oddělení řádků
- znak oddělení sloupců
- zdroj příkazů
- cíl výstupu

#### VÝBĚR Z MOŽNOSTÍ NAČTENÍ TABULKY
* 1, načtení existující - výzva na jméno souboru - načtení
* 2, vytvoření nové
        
#### UPRAVOVÁNÍ TABULKY:
- vstup od uživatele dokud uživatel nezavolá "quit"
    - po příkazu "quit" - dialog s možností uložení tabulky do souboru a případnou výzvu o název souboru

- PŘÍKAZY:
      - přiřadit buňce výraz s vnořenými výrazy pomocí závorek:
        - XX = ...

        operandy:
          - číslo .. 15 (samostatné číslice)
          - text .. "ahoj" (znaky ohraničené znakem '"')
          - buňka .. E7 (název buňky)
            - sloupce pojmenovány A - Z, AA - AZ, BA - BZ, ...
            - řádky očíslovány od 1

        operátory:
          - číslo + číslo (sčítání čísel)
          - číslo - číslo (odečítání čísel)
          - číslo * číslo (násobení čísel)
          - číslo / číslo (dělení čísel)
          - text CONCAT text (konkatenace řetězců)
          - text REPEAT číslo (opakování řetězce)

        funkce:
          - ABS číslo (absolutní hodnota)
          - UPPER text .. (převod písmenných znaků řetězce na uppercase)
          - LOWER text .. (převod písmenných znaků řetězce na lowercase)

      - vymazat obsah buňky
        - samotná buňka: delete XX
        - rozsah buněk: delete XX : XX
        - celá tabulka: delete

      - výpis tabulky:
        - pouze vzorce: printf
        - vyhodnocené buňky: print
          - samotná buňka: print(f) XX
          - rozsah buněk: print(f) XX : XX
          - celá tabulka: print(f)

## POLYMORFIZMUS
- Výraz, který je přiřazený buňce bude reprezentovaný stromem, kde uzly budou potomci abstraktního datového typu (CExpression), který umožní zachytit složený výraz a prioritu operátorů.
- Společným rozhraním tohoto abstraktního datového typu bude metoda Evaluate, která vrátí výslednou hodnotu daného uzlu (CValue).
- Polymorfizmus spočívá v tom, že tato metoda Evaluate bude volána nad každým uzlem, kterým můžou být různí potomci (operátory, konstanty, odkazy na buňky,...).
- Např. operátor plus zavolá Evaluate() na své dva operandy a následně je sečte a výsledek pošle volajícímu.
- Nebo samotný list (CExprValue) stromu výrazu, který vrátí přímo hodnotu (CValue), kterou má v sobě uloženou.

- Dalším příkladem polymorfismu je metoda SetOperands u třídy COperator.
- Potomky jsou unární a binární operátory, které mají vlastní implementaci této metody, přitom se při zpracování výrazu volají polymorfně.

- Dále je polymorfismus uplatněn také ve funktoru CPrint, který tvoří společné rozhraní pro potomky CPrintValue a CPrintFormula.
- Tam se volá polymorfní metoda GetContent, vracející buď řetězec reprezentující vzorec uložený do buňky (třída CPrintFormula), nebo výslednou hodnotu buňky (třída CPrintValue).

### ALGORITMY

##### Zpracování složených výrazů se závorkami
 - Výrazy jsou zpracovány ShuntingYard algoritmem.
 - Funkce a operátory krát a děleno mají nejvyšší prioritu, operátory plus, minus a konkatenace a opakování řetězce mají nižší.
 - Nejnižší prioritu mají v mé implementace závorky z toho důvodu, že se vždycky levá závorka posílá na zásobník operátorů a chceme výrazy uvnitř závorky začít vyzvedávat až po čtení pravé závorky.

##### Ošetření cyklů ve výrazech:
 - Při čtení výrazu se ukládá k buňce informace, na kterých buňkách je závislá
 - Po zpracování výrazu se z aktuálně upravované buňky pošleme DFS a pokud dorazíme na tu samou buňku, nalezli jsme cyklus a tento výraz nedovolíme uložit

##### Vyhodnocení hodnoty buněk (před výpisem tabulky):
 - Pomocí TopSort algoritmu se určí pořadí buněk, ve kterém se mají vyhodnotit tak, aby měla každá buňka zaručené, že buňky, na kterých je závislá, budou vyhodnocené

### Ukázka
```
Do you want to load existing table?
> yes
Enter file name: 
> examples/basic.save
Edit table:
>> printf
  | A               | B                                    | C                                          | D               | E                   | 
-------------------------------------------------------------------------------------------------------------------------------------------------
1 |  = 50           |  = "text"                            |  = "ahoj " REPEAT ( ( 13 + A1 - 53 ) / 5 ) |  = B1 CONCAT E1 |  = C1 CONCAT "text" | 
-------------------------------------------------------------------------------------------------------------------------------------------------
2 |  = B1 CONCAT C1 |  = A1 * ( ( 20 + 30 ) / 5 - 3 ) / 35 |  = A1 + D2                                 |  = -40          |                     | 
-------------------------------------------------------------------------------------------------------------------------------------------------
3 |                 |                                      |                                            |                 |                     | 
-------------------------------------------------------------------------------------------------------------------------------------------------
4 |  = B1           |                                      |                                            |                 |                     | 
-------------------------------------------------------------------------------------------------------------------------------------------------
5 |                 |                                      |                                            |                 |                     | 
-------------------------------------------------------------------------------------------------------------------------------------------------
>> print 
  | A              | B    | C          | D                  | E              | 
------------------------------------------------------------------------------
1 | 50             | text | ahoj ahoj  | textahoj ahoj text | ahoj ahoj text | 
------------------------------------------------------------------------------
2 | textahoj ahoj  | 10   | 10         | -40                |                | 
------------------------------------------------------------------------------
3 |                |      |            |                    |                | 
------------------------------------------------------------------------------
4 | text           |      |            |                    |                | 
------------------------------------------------------------------------------
5 |                |      |            |                    |                | 
------------------------------------------------------------------------------
>> delete A2 : C2
>> printf
  | A     | B         | C                                          | D               | E                   | 
------------------------------------------------------------------------------------------------------------
1 |  = 50 |  = "text" |  = "ahoj " REPEAT ( ( 13 + A1 - 53 ) / 5 ) |  = B1 CONCAT E1 |  = C1 CONCAT "text" | 
------------------------------------------------------------------------------------------------------------
2 |       |           |                                            |  = -40          |                     | 
------------------------------------------------------------------------------------------------------------
3 |       |           |                                            |                 |                     | 
------------------------------------------------------------------------------------------------------------
4 |  = B1 |           |                                            |                 |                     | 
------------------------------------------------------------------------------------------------------------
5 |       |           |                                            |                 |                     | 
------------------------------------------------------------------------------------------------------------
>> A2 = A1 / ( A1 + D2 )
>> print
  | A    | B    | C          | D                  | E              | 
--------------------------------------------------------------------
1 | 50   | text | ahoj ahoj  | textahoj ahoj text | ahoj ahoj text | 
--------------------------------------------------------------------
2 | 5    |      |            | -40                |                | 
--------------------------------------------------------------------
3 |      |      |            |                    |                | 
--------------------------------------------------------------------
4 | text |      |            |                    |                | 
--------------------------------------------------------------------
5 |      |      |            |                    |                | 
--------------------------------------------------------------------
>> quit
Do you want to save table?
> yes
Enter file name: 
> table.save
```