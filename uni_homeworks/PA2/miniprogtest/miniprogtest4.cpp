#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class CTimeStamp
{
public:
    // Variables

    // Constructor
    CTimeStamp () {}
    CTimeStamp ( int y, int m, int d, int h, int min, int s )
    {
        date = s + pow(2) * min + pow(4) * h + pow(6) * d + pow(8) * m + pow(10) * y;
    }

    // Methods
    friend bool operator <= ( CTimeStamp l, CTimeStamp r );
    friend bool operator >= ( CTimeStamp l, CTimeStamp r );

private:
    // Variables
    long int date;

    // Methods
    long int pow ( int cnt ) const
    {
        long int base = 1;
        for ( int i = 0; i < cnt; i ++ )
            base *= 10;
        return base;
    };
};

bool operator <= ( CTimeStamp l, CTimeStamp r )
{
    return l . date <= r . date;
}

bool operator >= ( CTimeStamp l, CTimeStamp r )
{
    return l . date >= r . date;
}

class CContact
{
public:
    // Variables
    bool duplicit;

    // Constructor
    CContact ( CTimeStamp ts, int num1, int num2 )
    {
        c1 = num1;
        c2 = num2;
        time = ts;
        if ( num1 == num2 )
            duplicit = true;
        else
            duplicit = false;
    }

    // Methods
    bool isContact ( int sample, int & contact ) const
    {
        if ( sample == c1 )
        {
            contact = c2;
            return true;
        }
        if ( sample == c2 )
        {
            contact = c1;
            return true;
        }
        return false;
    };

    bool inTime ( CTimeStamp from, CTimeStamp to ) const
    {
        if ( time >= from && time <= to )
            return true;
        return false;
    }

    void printContact () const
    {
        cout << "c1: " << c1 << ", c2: " << c2 << endl;
        cout << "duplicit: " << boolalpha << duplicit << endl;
    }

private:
    // Variables
    CTimeStamp time;
    int c1;
    int c2;

    // Methods
};

class CEFaceMask
{
public:
    // Variables

    // Constructor

    // Methods
    CEFaceMask & addContact ( CContact contact )
    {
        if ( ! contact . duplicit )
            contacts . push_back ( contact );
        return * this;
    };

    vector <int> listContacts ( int num ) const
    {
        vector <int> result;
        int tmp;
        for ( auto ct = contacts . begin (); ct != contacts . end (); ct ++ )
        {
            if ( ct -> isContact ( num, tmp )
                && ! inResult ( result, tmp ) )
            {
                result . push_back ( tmp );
            }
        }
        return result;
    };

    vector <int> listContacts ( int num, CTimeStamp from, CTimeStamp to ) const
    {
        vector <int> result;
        int tmp;
        for ( auto ct = contacts . begin (); ct != contacts . end (); ct ++ )
        {
            if ( ct -> inTime ( from, to )
                && ct -> isContact ( num, tmp )
                && ! inResult ( result, tmp ) )
            {
                result . push_back ( tmp );
            }
        }
        return result;
    };

private:
    // Variables
    vector <CContact> contacts;

    // Methods
    bool inResult ( vector <int> & res, int el ) const
    {
        for ( auto c = res . begin (); c != res . end (); c ++ )
            if ( el == * c )
                return true;
        return false;
    }
};

#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;
}
#endif /* __PROGTEST__ */
