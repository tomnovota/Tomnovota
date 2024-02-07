# Cvičení 01

V adresáři `src` je šablona pro příklad na dnešní cvičení. Pomocí `#define` je rozdělena na několik levelů, které budeme postupně plnit.

#### 0) Naklonování projektu z GitLabu

Pomocí příkazové řádky, nutné následně zadat vaše uživatelské jméno a heslo:
```
git clone https://gitlab.fit.cvut.cz/xchludil-tutorials/pa2/cviceni01-paus
```
Nebo pomocí stáhnutí z GitLabu v prohlížeči.


#### 1) Seznámení s C++, řešení bez IDE
Nejprve zkuste splnit `LEVEL0`, tj. kompilaci zdrojáku na příkazové řádce pomocí příkazu
```
g++ -Wall -pedantic -std=c++11 main.cpp -o cviko1
```

#### 2) Úkol na práci se soubory v C++

##### LEVEL1:
* Napište program, ve kterém si **vytvoříte vhodnou strukturu** `data`.
* Vytvořte vlastní hlavičkový a zdrojový soubor pro strukturu `struct.h` a `struct.cpp`.

##### LEVEL2:
* Tento program načte ze standardního vstupu **5 čísel int** a uloží je do této struktury.
* Toto udělejte pomocí funkce `scan_data`, která ma jako parametr strukturu `data`.
* Vytvořte fci `print_data`, která data vypíše na obrazovku.

##### LEVEL3:
* Obsah struktury následně **uložte do binárního souboru** pomocí fce `save_data`.
* Fce vrací `true`, pokud se operace povedla.

##### LEVEL4:
* Poté **přečtěte data z tohoto souboru a uložte je do další struktury** stejného typu pomocí fce `read_data`.
* Tuto další strukturu vypište na stadardní výstup.
* Fce vrací `true`, pokud se operace povedla.
* Napište fci `bool is_equal()` pro porovnání dvou struktur `data`.

##### LEVEL5:
* Napište fci `sort_data`, která data **seřadí podle velikosti vzestupně**.
* Pak strukturu vypište.

##### BONUS:
* Vyřešte načítání a ukládání dat z/do souboru **bez použití cyklu** a jen pomocí jedné souborové operace pro čtení/zápis.

#### 3) Úkol na doma
Vyzkoušejte si práci s různými IDE (CLion, DEV C++, NetBeans, Eclipse, MS Visual Studio...) a zvolte si jej.

