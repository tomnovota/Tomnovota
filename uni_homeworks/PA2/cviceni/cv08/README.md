# Cvičení 08

Toto cvičení má ukázat použití polymorfizmu v C++. V souboru `classes.h` je několik odvozených tříd. Základní třída `CEquipment` slouží pro odvození všech dalších prvků v kanclu, a sice `CElectricalDevice`, od kterého odvodíme `CPrinter` a `CScanner`, a následně `CCopier`, což je třída odvozená od `CPrinter` a `CScanner` dohromady. Zde právě vzniká onen nechvalně známý **diamantový problém**.

## Zadání 

Použijte kód ze složky `src` v tomto projektu a upravte, aby prošel testovací fcí `main()`. Některé úkoly budete muset splnit přímo v `main()`.

### Úkol 1 
Do `classes.h` připište čistě virtuální třídu `CEquipmentI` (jakože interface), od které budou všechny další odvozeny a která bude po odvozených třídách požadovat implementaci následujících metod:
*  `int getId()`,
*  `std::string getName()`,
*  `void setName(const std::string& name)`,
*  `void aboutMe()`.

Následně v `main` deklarujte čistokrevné céčkové pole, do kterého lze uložit jakýkoliv objekt implementující tento interface.
Na všech prvcích zavolejte funkci `aboutMe()` a ověřte, zda dává očekávané hodnoty.

### Úkol 1A

Implementujte `operator<<` pro výpis do streamu, který bude umět vypsat všechny třídy odvozené od interfacu `CEquipmentI`.

### Úkol 2
* Využijte knihovní funkci `std::sort` na seřazení tohoto pole podle atributu `m_name`.
* Výsledek vypište.
* Následně z paměti kompletně vymažte pole, protože jsme používali obyčejné pointery.

### Úkol 3
* V `main` deklarujte `std::vector`, do kterého lze uložit jakýkoliv objekt odvozený od `CElectricalDevice`. Zkuste zde udělat vector `unique_ptr` nebo `shared_ptr`, abychom se nemuseli starat o mazání paměti.
* Využijte knihovní funkci `std::sort` na seřazení tohoto vektoru podle atributu `m_power`.
* Výsledek vypište.

### Úkol 4
Pomocí vhodné knihovní funkce spočítejte počet zapnutých spotřebičů ve vektoru.

### Poznámka

Mrkněte, jak volat metody z předků, když je to potřeba (metoda aboutMe() např. v `CElectricalDevice` a dalších).
