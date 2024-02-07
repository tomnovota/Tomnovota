#include <iostream>
#include <cassert>
#include "vector.h"

#define TEST3

int main()
{
    std::cout << " -- Tutorial 3 test set START --" << std::endl;

#if defined TEST1
    std::cout << " -- Flag TEST1 enabled --" << std::endl;
#elif defined TEST2
    std::cout << " -- Flag TEST2 enabled --" << std::endl;
#elif defined TEST3
    std::cout << " -- Flag TEST3 enabled --" << std::endl;
#elif defined TEST4
    std::cout << " -- Flag TEST4 enabled --" << std::endl;
#else
    std::cout << " -- No test flag enabled. Compile using `-D TESTn` where n=[1,4] --" << std::endl;
#endif

#if defined TEST1 || defined TEST2 || defined TEST3 || defined TEST4

    std::cout << "Task 1 test" << std::endl;

    Vector vector;

    assert(vector.size() == 0);
    assert(vector.empty());

#if defined TEST2 || defined TEST3 || defined TEST4

    std::cout << "Task 2 test" << std::endl;

    for (int i = 0; i < 50; i++)
        vector.push_back(i);
    assert(vector.size() == 50);

    vector.pop_back();
    assert(vector.size() == 49);

    for (int i = 0; i < 49; i++)
        assert(vector.at(i) == i);

    vector.at(0) = 100;
    assert(vector.at(0) == 100);

#if defined TEST3 || defined TEST4

    std::cout << "Task 3 test" << std::endl;

    assert(vector.count(0) == 0);
    assert(vector.count(100) == 1);
    for (int i = 1; i < 49; i++)
        assert(vector.count(i) == 1);
    vector.push_back(100);
    vector.push_back(100);
    vector.push_back(100);
    assert(vector.count(100) == 4);
    assert(vector.size() == 52);

    vector.insert(0, -1);
    assert(vector.at(0) == -1);
    assert(vector.at(1) == 100);
    assert(vector.count(-1) == 1);
    assert(vector.size() == 53);

    vector.insert(51, -1);
    assert(vector.at(51) == -1);
    assert(vector.at(50) == 100);
    assert(vector.at(52) == 100);
    assert(vector.count(-1) == 2);
    assert(vector.size() == 54);

    vector.insert(54, -1);
    assert(vector.at(53) == 100);
    assert(vector.at(54) == -1);
    assert(vector.count(-1) == 3);

    vector.erase(54);
    assert(vector.at(53) == 100);
    assert(vector.count(-1) == 2);
    assert(vector.size() == 54);

    vector.erase(51);
    assert(vector.at(51) == 100);
    assert(vector.count(-1) == 1);
    assert(vector.size() == 53);

    vector.erase(0);
    assert(vector.at(0) == 100);
    assert(vector.count(-1) == 0);
    assert(vector.size() == 52);


#if defined TEST4

    std::cout << "Task 4 test" << std::endl;

    assert(vector.data() != nullptr);
    assert(Vector().data() == nullptr);
    int* data = vector.data();
    data++;
    assert(vector.data() != data);
    data--;
    assert(vector.data() == data);

    // TODO: Sort the array
    // NOTE: use std::sort

    assert(find_index(vector, 0) == -1);
    assert(find_index(vector, 100) == 48);
    vector.erase(48);
    assert(find_index(vector, 100) == 48);
    assert(find_index(vector, 1) == 0);

#endif
#endif
#endif
#endif

    std::cout << " -- Tutorial 3 test set END --" << std::endl;
}
