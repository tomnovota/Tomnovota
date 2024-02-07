# Cvičení 7

Uvožavali jste někdy o navratu na první stupeň základní školy? Vše tam bylo jednoduché, učení nenáročné...
Dneska se takový pocit pokusíme zreplikovat, protože budeme pracovat s geometrickými tvary a počítat jejich obvody a obsahy.

## Něco na rozehřátí
Ve složce [shapes](shapes) jsou připraveny soubory [CCircle.h](shapes%2FCCircle.h), 
[CRectangle.h](shapes%2FCRectangle.h), [CTriangle.h](shapes%2FCTriangle.h) a jejich `.cpp`
protějšky. Jejich implementace je téměř hotová, ovšem chybí implementace metod `getCircumference` a `getContent`,
které mají navrátit obvod a obsah. Tyto metody implementujte do `.cpp` souborů.

## Úkoly na toto cvičení

### 1) Společná třída `CShape`
Metody `getCircumference` a `getContent` bychom rádi volali bez ohledu na to, jakýho typu je tvar a tyto tvary bychm rádi
ukládali do jednoho datového typu. Vytvořte soubory `CShape.h` a `CShape.cpp` a do nich vytvořte abstraktní třídu `CShape`,
která tyto metody bude schopná volat. Ke všem dosavadním tvarům tuto třídu nastavte jako rodiče. Doplntě vhodně k metodám slova `virtual`
a `override`.

### 2) Vytvořte třídu `CShapeVector`
Vytvořte soubory `CShapeVector.h` a `CShapeVector.cpp` a do nich proveďte implementaci třídy.
Tato třída bude udržovat seznam tvarů. Jako kontruktor nám bude stačit defaultní, a bude nám stačit v ní metoda `pushBack`, 
která příjme tvar (referenci pokud možno) a ten do vectoru vloží a metoda `at` (konstantní i nekonstantní varianta),
která nám vrátí referenci (konstantní / nekonstantní) na tvar na zvoleném indexu. Index bude typu `size_t`.

POZOR!!!: Myslete na způsob uložení těchto tvarů a popřípadě způsob jejich smazání. Je to i vhodná příležitost si vyzkoušet
`unique_ptr` a funkci `make_unique`. Bez jeho použití pravděpodobně budete muset implementovat desturktor.

### 3) Přidejte do třídy `CShapeVector` metody `getCircumference` a `getContent`
Tyto metody vrátí součet všech obvodů / obsahů tvarů uložených ve vektoru.

### 4) Vytvořte třídu `CSquare`
No, počkat, ale čtverec je vlatně taky obdelník... Takže určitě budeme chtít mít možnost ctverec uložit do proměnné obdelníku.
Dá se říct, že se vlastně jedná o obdelník, který bude mít vždy stejně dlouhé strany.

Vytvořte tedy třídu `CSquare` v souborech `CSquare.h` a `CSquare.cpp`. Tato třída musí jít uložit do instancí třídy `CRectangle`
a musí umět vše, co umí `CRectangle`. Při vhodném návrhu bude tato třída velice krátká.

### 5) Přetížete operátor pro výpis `<<` nad všemi tvary
Chceme aby každý z našich tvarů do konzole vypsal, ze které třídy pochází, tudíž pro obdelník se vypíše `CRectangle`, pro
kruh `CCircle`, ale hlavně, pro čtverec se vypíše `CSquare`. Chceme aby výpis byl možný nad všemi datovými třídamy.

### 6) Přetížete operátor pro výpis `<<` nad `CShapeVector`
To asi nepotřebuje nijak zvlašť rozebírat... Vypáše seznam tříd uložených ve zvoleném vectoru oddělený čárkou.
