#pragma once
#include <string>
#include <map>
#include <list>
#include <set>

class NumberValidator {
public:
    /**
    * @param phoneCode Country-specific number prefix (e.g. +44 UK, +421 Slovakia)
    * @param allowLocalNumbers Allow numbers in a local format (0XXXXXXXXX instead of <phoneCode>XXXXXXXXX)
    * For this task, assume non-local numbers always start with '+' (not 00)
    */
    NumberValidator(const std::string& phoneCode, bool allowLocalNumbers);
    // Assume all numbers must have their local part 9 digits long
    bool operator()(const std::string& number);
private:
    std::string phoneCode;
    bool allowLocalNumbers;
};

enum class NumberType
{
    Personal, Home, Work, Fax
};

class YellowPages {
public:
    // Hold information about one person's record in the yellow pages.
    // That means their name, address and phone numbers, each with its number type (maximum one number per each type)
    class Record {
    public:
        std::string name;
        std::string address;

        Record(const std::string& name, const std::string& address);
        // Return an empty string if there is no number with the provided type
        std::string getNumber(NumberType numberType) const; // O(log n)
        bool addNumber(NumberType numberType, const std::string& number);
    private:
        static NumberValidator validator;

        std::map<NumberType, std::string> numbers;
    };

    using iterator = std::list<Record>::iterator;

    bool addPerson(const std::string& name, const std::string& address); // O(log n)
    iterator getPerson(const std::string& name); // O(n)
    // Remove the provided person
    void removePerson(const iterator& it); // O(1)
    // Add a family member immediately after the provided person. Use their address.
    bool addFamilyMember(const iterator& it, const std::string& name); // O(1)
private:
    std::list<Record> records;
    std::set<std::string> names;
};
