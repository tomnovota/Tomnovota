#include "yellowpages.h"
#include <iostream>

NumberValidator YellowPages::Record::validator = NumberValidator("420", true);

NumberValidator::NumberValidator(const std::string& phoneCode, bool allowLocalNumbers):
    phoneCode(phoneCode), allowLocalNumbers(allowLocalNumbers)
{
}
bool NumberValidator::operator()(const std::string& number)
{
    if (number.size() == 0)
        return false;

    if (number[0] == '0')
    {
        if (!allowLocalNumbers || number.size() != 10)
            return false;
    }
    else
    {
        if (number[0] != '+' ||
            (number.size() != phoneCode.size() + 10) ||
            (number.substr(1, phoneCode.size()) != phoneCode)
            )
            return false;
    }

    for (size_t i = 1; i < number.size(); i++)
        if (!std::isdigit(number[i]))
            return false;

    return true;
}

YellowPages::Record::Record(const std::string& name, const std::string& address)
    :name(name), address(address)
{
}

std::string YellowPages::Record::getNumber(NumberType numberType) const
{
    const auto& it = numbers.find(numberType);
    if (it == numbers.end())
        return "";
    else return it->second;
}

bool YellowPages::Record::addNumber(NumberType numberType, const std::string& number)
{
    if (!validator(number)) return false;
    return numbers.insert(std::make_pair(numberType, number)).second;
}

bool YellowPages::addPerson(const std::string& name, const std::string& address)
{
    if (!names.insert(name).second) return false;
    records.emplace_back(name, address);
    return true;
}

YellowPages::iterator YellowPages::getPerson(const std::string& name)
{
    iterator it;
    for (it = records.begin(); it != records.end(); it++)
        if (it->name == name)
            break;
    return it;
}

void YellowPages::removePerson(const iterator& it)
{
    names.erase(it->name);
    records.erase(it);
}

bool YellowPages::addFamilyMember(const iterator& it, const std::string& name)
{
    if (!names.insert(name).second) return false;
    records.emplace(it, name, it->address);
    return true;
}
