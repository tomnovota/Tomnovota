#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <cxxabi.h>

#define PRINT 0

/*
CEquipment
    |
CElectricalDevice
    /       \
CPrinter    CScanner
    \       /
    CCopier
*/

class CEquipment
{
  protected:
    int m_id;
    std::string m_name;

  public:
    CEquipment(int id, const std::string &name) : m_id(id), m_name(name)
    {
        if ( PRINT )
            std::cout << "CEquipment(" << id << ");" << std::endl;
    }
    int getId() const override { return m_id; }
    std::string getName() const override { return m_name; }
    void setName(const std::string &name) override { m_name = name; }
    std::ostream &aboutMe(std::ostream &os) const override
    {
        int status;
        return os << "I am " << abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status) << ", id=" << m_id << ", my name is " << m_name;
    }
    ~CEquipment()
    {
        if (PRINT)
            std::cout << "CEquipment says bye bye" << std::endl;
    }
};

class CElectricalDevice : public CEquipment
{
protected:
    int m_power;
    bool m_running;

public:
    CElectricalDevice(int id, const std::string &name, int power) : CEquipment(id, name), m_power(power), m_running(false)
    {
        if (PRINT)
            std::cout << "CElectricalDevice(" << id << ", " << power << ", " << m_running << ");" << std::endl;
    }
    int getPower() const { return m_power; }
    bool running() const { return m_running; }
    void switchOn() { m_running = true; }
    void switchOff() { m_running = false; }
    std::ostream &aboutMe(std::ostream &os) const override
    {
        CEquipment::aboutMe(os);
        return os << ", power=" << m_power << ", running=" << m_running;
    }
    ~CElectricalDevice()
    {
        if (PRINT)
            std::cout << "CElectricalDevice says bye bye" << std::endl;
    }
};

class CPrinter : public virtual CElectricalDevice
{
protected:
    int m_printParams;

public:
    CPrinter(int id, const std::string &name, int power, int params) : CElectricalDevice(id, name, power), m_printParams(params)
    {
        if (PRINT)
            std::cout << "CPrinter(" << id << ", " << power << ", " << m_running << ", " << params << ");" << std::endl;
    }
    void setPrintParams(int params) { m_printParams = params; }
    void print(int data) const { std::cout << data << std::endl; }
    std::ostream &aboutMe(std::ostream &os) const override
    {
        CElectricalDevice::aboutMe(os);
        return os << ", params=" << m_printParams;
    }
    ~CPrinter()
    {
        if (PRINT)
            std::cout << "CPrinter says bye bye" << std::endl;
    }
};

class CScanner : public virtual CElectricalDevice
{
protected:
    int m_scannedData;

public:
    CScanner(int id, const std::string &name, int power) : CElectricalDevice(id, name, power)
    {
        if (PRINT)
            std::cout << "CScanner(" << id << ", " << power << ", " << m_running << ");";
    }
    int getScannedData() const { return m_scannedData; }
    void scan() { m_scannedData = 42; }
    std::ostream &aboutMe(std::ostream &os) const override
    {
        CElectricalDevice::aboutMe(os);
        return os << ", scannedData=" << m_scannedData;
    }
    ~CScanner()
    {
        if (PRINT)
            std::cout << "CScanner says bye bye" << std::endl;
    }
};

class CCopier : public CPrinter, public CScanner
{
public:
    CCopier(int id, const std::string &name, int power, int params) : CElectricalDevice(id, name, power),
                                                                      CPrinter(id, name, power, params),
                                                                      CScanner(id, name, power)
    {
        if (PRINT)
            std::cout << "CCopier(" << id << ", " << power << ", " << params << ");";
    }
    void copy()
    {
        scan();
        print(m_scannedData);
    }
    std::ostream &aboutMe(std::ostream &os) const override
    {
        CElectricalDevice::aboutMe(os);
        return os << ", scannedData=" << m_scannedData << ", params=" << m_printParams;
    }
    ~CCopier()
    {
        if (PRINT)
            std::cout << "CCopier says bye bye" << std::endl;
    }
};

#endif
