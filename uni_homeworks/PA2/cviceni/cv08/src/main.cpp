#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <sstream>
#include <memory>
#include "classes.h"

#define UKOL1  // Container for CEquipmentI
// #define UKOL1A // Operator <<
// #define UKOL2  // Name Sorting
// #define UKOL3  // Power Sorting
// #define UKOL4  // Count If

constexpr int n = 10;

// LEVEL1A - Operator << pro veskere vybaveni

int main()
{
#ifdef UKOL1
    // Zde udelat cistokrevne c++ pole s nazvem 'array'
    // pro interface pro vybaveni a vlozit nasledujici polozky do pole
    // CEquipment(1000, "Rohozka");
    // CElectricalDevice(1001, "Sbijecka", 150);
    // CEquipment(1002, "Stul");
    // CElectricalDevice(1003, "Lampa", 60);
    // CPrinter(1004, "BT-100", 40, 666);
    // CScanner(1005, "Canon", 60);
    // CCopier(1006, "Xerox 200", 100, 777);
    // CPrinter(1007, "Epson 1234", 80, 666);
    // CScanner(1008, "Minolta", 55);
    // CCopier(1009, "Xerox 100", 99, 777);

    for (int i = 0; i < n; i++)
    {
        array[i]->aboutMe(std::cout); // Vypis spravneho aboutMe()
        std::cout << std::endl;
    }
#endif

    std::cout << "----------------------------" << std::endl;

#ifdef UKOL1A
    // udelat operator<< pro vypis do streamu pro ruzne vybaveni

    std::ostringstream sstr;
    std::unique_ptr<CEquipmentI> e1{new CScanner(666, "Xerox 2", 100)};
    std::unique_ptr<CEquipmentI> e2{new CCopier(777, "Xerox 100", 80, 500)};
    sstr << *e1 << "; " << *e2;
    std::cout << *e1 << "; " << *e2 << std::endl;
    assert(sstr.str() == "I am CScanner, id=666, my name is Xerox 2, power=100, running=0, scannedData=0; I am CCopier, id=777, my name is Xerox 100, power=80, running=0, scannedData=0, params=500");

#endif

    std::cout << "----------------------------" << std::endl;

#ifdef UKOL2
    //
    //  Zde pridat kod pro sorteni celeho pole podle nazvu vybaveni (name)
    //

    std::cout << "Sorting..." << std::endl;

    for (int i = 0; i < n; i++)
    {
        std::cout << *(array[i]) << std::endl;
    }

    assert(array[9]->getId() == 1006);

    //
    //  Protoze jsme pouzivali obyc pointery, je treba zde smazat zabranou pamet
    //

#endif

    std::cout << "----------------------------" << std::endl;

#ifdef UKOL3

    //
    //  Zde udelat vector smart pointeru s nazvem 'devices' (unique nebo shared) na CElectricalDevice
    //  a vlozit do nej nasladujici hodnty
    //

    // CElectricalDevice(1001, "Sbijecka", 230));
    // CElectricalDevice(1000, "Fen", 65));
    // CElectricalDevice(1002, "Sporak", 500));
    // CElectricalDevice(1003, "Lampa", 60));
    // CPrinter(1004, "BT-100", 40, 666));
    // CScanner(1005, "Canon", 60));
    // CCopier(1006, "Xerox 200", 100, 777));
    // CPrinter(1007, "Epson 1234", 80, 666));
    // CScanner(1008, "Minolta", 55));
    // CCopier(1009, "Xerox 100", 99, 777));

    for (auto &x : devices)
    {
        std::cout << *x << std::endl;
    }

    //
    // A pole sortit podle prikonu (m_power)
    //
    std::cout << "Sorting..." << std::endl;

    for (auto &x : devices)
    {
        std::cout << *x << std::endl;
    }

    assert(devices[9]->getId() == 1002);
#endif

#ifdef UKOL4

    for (auto &x : devices)
        if (x->getId() % 2)
            x->switchOn();

    //
    //  Zde spocitat zapnute pristroje a ulozit do promenne 'c'
    //

    std::cout << "Switched On: " << c << std::endl;

    assert(c == 5);

#endif

    return 0;
}
