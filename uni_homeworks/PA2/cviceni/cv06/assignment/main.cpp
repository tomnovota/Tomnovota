#include <cassert>
#include <iostream>
#include <sstream>
#include "sharedptr.h"
#include "yellowpages.h"

#define MIN_TEST 1
#define MAX_TEST 5
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

template <typename T>
bool expect(const T& value, const std::string& expected)
{
    std::ostringstream oss;
    oss << value;
    auto actual = oss.str();

    if (actual != expected)
    {
        std::cout << "Actual  : " << actual << std::endl;
        std::cout << "Expected: " << expected << std::endl;
        return false;
    }
    return true;
}

#if TEST >= 1
void foo(SharedPtr ptr, size_t oldRefCount)
{
    assert(ptr.refCount() == oldRefCount + 1);
}
#endif


int main()
{
    std::cout << " -- Tutorial 6 test set START --" << std::endl;
#if TEST >= MIN_TEST && TEST <= MAX_TEST
    std::cout << " -- Flag TEST=" << TEST << " enabled --" << std::endl;
#else
    std::cout << " -- No test flag enabled. Set a test flag in Makefile to TEST=[" STR(MIN_TEST) "-" STR(MAX_TEST) "]. --" << std::endl;
#endif

#if TEST >= 1
    SharedPtr ptr, ptr2, ptr3;

    SharedPtr ptr4 = make_shared < int >(42);
    assert(ptr4.refCount() == 1 && ptr4 && *ptr4 == 42);

    ptr = ptr;
    assert(ptr == ptr && &(*ptr) == nullptr);

    ptr = ptr4;
    assert(ptr == ptr4 &&
        ptr4.refCount() == 2 && ptr.refCount() == 2 &&
        *ptr == 42 && *ptr4 == 42);

    ptr2 = ptr4;
    assert(ptr == ptr2 && ptr == ptr4 &&
        ptr4.refCount() == 3 && ptr.refCount() == 3 && ptr2.refCount() == 3 &&
        *ptr == 42 && *ptr4 == 42 && *ptr2 == 42);

    ptr = nullptr;
    ptr2 = nullptr;

    assert(ptr.refCount() == 1 && !ptr);
    assert(ptr2.refCount() == 1 && !ptr2);
    assert(ptr4.refCount() == 1 && ptr4 && *ptr4 == 42);

    ptr = ptr4 = make_shared < int >(142);

    assert(ptr4.refCount() == 2 && ptr.refCount() == 2 &&
        ptr4 && *ptr4 == 142 &&
        ptr && *ptr == 142);

    foo(ptr4, ptr4.refCount());
    assert(ptr4.refCount() == 2);

    std::cout << "TEST 1 OK" << std::endl;
#endif

#if TEST >= 2
    NumberValidator ukValidator("44", false);
    assert(ukValidator("+44000000001"));
    assert(!ukValidator(""));
    assert(!ukValidator("1"));
    assert(!ukValidator("+"));
    assert(!ukValidator("+1"));
    assert(!ukValidator("+123"));
    assert(!ukValidator("a"));
    assert(!ukValidator("asd"));
    assert(!ukValidator("asdasdasd"));
    assert(!ukValidator("+a4000000001"));
    assert(!ukValidator("+4400000000a"));
    assert(!ukValidator("+440000000001"));
    assert(!ukValidator("123456789"));
    assert(!ukValidator("+4412345679"));

    NumberValidator usValidator("1", true);
    assert(usValidator("+1989878767"));
    assert(!usValidator("+122233344"));
    assert(!usValidator("+12223334455"));
    assert(usValidator("0987654321"));
    assert(!usValidator("09876543211"));
    assert(!usValidator("098765432"));

    NumberValidator skValidator("421", true);
    assert(skValidator("+421989878767"));
    assert(!skValidator("+42122233344"));
    assert(!skValidator("+4212223334455"));
    assert(skValidator("0987654321"));
    assert(!skValidator("09876543211"));
    assert(!skValidator("098765432"));

    std::cout << "TEST 2 OK" << std::endl;
#endif

#if TEST >= 3
    YellowPages::Record person1("John Doe", "Young street, Toronto");
    assert(expect(person1.getNumber(NumberType::Home), ""));
    assert(expect(person1.getNumber(NumberType::Personal), ""));
    assert(!person1.addNumber(NumberType::Home, "+421123456789"));
    assert(person1.addNumber(NumberType::Home, "+420123456789"));
    assert(!person1.addNumber(NumberType::Home, "+420123456789"));
    assert(person1.addNumber(NumberType::Fax, "0234567890"));
    assert(expect(person1.getNumber(NumberType::Personal), ""));
    assert(expect(person1.getNumber(NumberType::Home), "+420123456789"));
    assert(person1.getNumber(NumberType::Home) != "0123456789");
    assert(expect(person1.getNumber(NumberType::Fax), "0234567890"));
    assert(person1.getNumber(NumberType::Fax) != "+421234567890");

    std::cout << "TEST 3 OK" << std::endl;
#endif

#if TEST >= 4
    YellowPages zlateStranky;
    assert(zlateStranky.addPerson("Bob Wide", "Arizona, Texas"));
    assert(zlateStranky.addPerson("Beff Jezos", "Bahamas 123"));
    for (int i = 0; i < 100; i++)
        assert(zlateStranky.addPerson(
            "Abraham's child number " + std::to_string(i),
            "East street " + std::to_string(i)
        ));
    const auto it = zlateStranky.getPerson("Bob Wide");
    assert(it->addNumber(NumberType::Personal, "+420123123123"));
    assert(it->addNumber(NumberType::Fax, "+420123123123"));
    assert(!it->addNumber(NumberType::Fax, "+420123123123"));
    assert(it->addNumber(NumberType::Home, "0123123123"));
    assert(!it->addNumber(NumberType::Home, "+420123123123"));
    assert(!it->addNumber(NumberType::Work, "+421123123123"));
    assert(!it->addNumber(NumberType::Work, "+42123123123"));
    assert(!it->addNumber(NumberType::Work, "+4123123123"));
    assert(expect(it->address, "Arizona, Texas"));
    assert(zlateStranky.addFamilyMember(it, "Filomena Wide"));
    assert(zlateStranky.addFamilyMember(it, "Gazelle Wide"));
    assert(zlateStranky.addFamilyMember(it, "Hugo Wide"));
    assert(zlateStranky.addFamilyMember(it, "Ivan Wide"));
    assert(zlateStranky.addFamilyMember(it, "Jeffrey Wide"));
    assert(expect(it->getNumber(NumberType::Personal), "+420123123123"));
    assert(expect(it->getNumber(NumberType::Fax), "+420123123123"));
    assert(expect(it->getNumber(NumberType::Home), "0123123123"));
    assert(expect(it->getNumber(NumberType::Work), ""));
    zlateStranky.removePerson(it);
    assert(it != zlateStranky.getPerson("Bob Wide"));
    const auto it2 = zlateStranky.getPerson("Hugo Wide");
    assert(expect(it2->address, "Arizona, Texas"));

    std::cout << "TEST 4 OK" << std::endl;
#endif

#if TEST >= 5
    assert(zlateStranky.addPerson("Bob Wide", "Arizona, Texas"));
    assert(!zlateStranky.addPerson("Bob Wide", "Arizona, Texas"));
    assert(!zlateStranky.addPerson("Beff Jezos", "Bahamas 123"));
    assert(!zlateStranky.addPerson("Hugo Wide", "Bahamas 123"));
    assert(!zlateStranky.addFamilyMember(it2, "Filomena Wide"));
    assert(!zlateStranky.addFamilyMember(it2, "Gazelle Wide"));
    std::cout << "TEST 5 OK" << std::endl;
#endif

    std::cout << " -- Tutorial 6 test set END --" << std::endl;
}
