#define LEVEL0 // zkouska kompilace bez IDE
#define LEVEL1 // definice struktury v externim souboru
#define LEVEL2 // nacteni dat z klavesnice a vypis
#define LEVEL3 // save do binarniho souboru
#define LEVEL4 // nacteni ze souboru
#define LEVEL5 // sorteni dat

#ifdef LEVEL0
#include <iostream>
#include <cassert>

#ifdef LEVEL1
#include "struct.h"
#include "struct.h" //Co kdyz dame 2x stejny include? Jak to udelat, aby to proslo kompilaci?
#endif //L1

int main ()
{
  std::cout << "Level 0 OK" << std::endl;

#ifdef LEVEL1
  data d;
  data e;

  std::cout << "Level 1 OK" << std::endl;

#ifdef LEVEL2
  scan_data(d);
  print_data(d);

  std::cout << "Level 2 OK" << std::endl;

#ifdef LEVEL3
  if(save_data("binary.dat", d))
  {
    std::cout << "Level 3 OK" << std::endl;

#ifdef LEVEL4
    if (read_data("binary.dat", e))
    {
      print_data(e);
      assert(is_equal(d, e));
      std::cout << "Level 4 OK" << std::endl;
#ifdef LEVEL5
      sort_data(e);
      print_data(e);
      std::cout << "Level 5 OK" << std::endl;
#endif //L5
    }
#endif //L4
  }  
#endif //L3
  return 0;
#endif //L2
#endif //L1
}

#endif //L0