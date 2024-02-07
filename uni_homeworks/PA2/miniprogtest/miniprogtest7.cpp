#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <stack>
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

    friend bool operator <= ( CTimeStamp l, CTimeStamp r )
    {
        return l . date <= r . date;
    }

    friend bool operator >= ( CTimeStamp l, CTimeStamp r )
    {
        return l . date >= r . date;
    }

    bool operator < ( const CTimeStamp & r ) const
    {
        return date < r . date;
    }

    bool InTime ( const CTimeStamp & from, const CTimeStamp & to )
    {
        return date >= from . date && date <= to . date;
    }

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

    int C1 ( void ) { return c1; }
    int C2 ( void ) { return c2; }
    CTimeStamp Time ( void ) { return time; }

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

    bool inTime ( const CTimeStamp & from, const CTimeStamp & to ) const
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

struct TContact
{
    int m_Num;
    CTimeStamp m_Time;

    TContact ( void ) = default;
    TContact ( int num, const CTimeStamp & time )
     : m_Num ( num )
     , m_Time ( time )
     {}

    static bool LessTime ( const TContact & c1, const TContact & c2 )
    {
        return c1 . m_Time < c2 . m_Time;
    }

};

struct TNode
{
    int m_Num;
    vector< TContact > m_Direct;
    vector< int > m_Contacts;

    TNode ( void ) = default;
    TNode ( int num )
     : m_Num ( num )
     {}

    static bool LessNum ( const TNode & node, int num )
    {
        return node . m_Num < num;
    }

    void AddDirect ( const TContact & contact )
    {
        auto pos = lower_bound ( m_Direct . begin(), m_Direct . end(), contact, TContact::LessTime );
        if ( pos == m_Direct . end() || TContact::LessTime ( contact, * pos ) )
            m_Direct . insert ( pos, contact );
    }

    bool AddContact ( int x )
    {
        auto pos = lower_bound ( m_Contacts . begin(), m_Contacts . end(), x );
        if ( pos == m_Contacts . end() || x < * pos )
        {
            m_Contacts . insert ( pos, x );
            return true;
        }
        return false;
    }
};

class CEFaceMask
{
    vector< TNode > m_Tree;

  public:
    // default constructor
    CEFaceMask ( void ) = default;

    // addContact ( contact )
    CEFaceMask & addContact ( CContact contact )
    {
        if ( contact . duplicit )
            return *this;

        AppendTree ( contact . C1(), contact . C2(), contact . Time() );
        AppendTree ( contact . C2(), contact . C1(), contact . Time() );

        return *this;
    }

    void AppendTree ( int num1, int num2, const CTimeStamp & time )
    {
        auto pos = lower_bound ( m_Tree . begin(), m_Tree . end(), num1, TNode::LessNum );
        if ( pos != m_Tree . end() && pos -> m_Num == num1 ) // existing
        {
            pos -> AddDirect ( TContact ( num2, time ) );
        }
        else
        {
            auto inserted = m_Tree . insert ( pos, TNode ( num1 ) );
            inserted -> AddDirect ( TContact ( num2, time ) );
        }
    }

    vector< int > getSuperSpreaders ( const CTimeStamp & from, const CTimeStamp & to )
    {
        vector< int > res;
        size_t max = 0;
        printTree();
        for ( auto & el : m_Tree )
        {
            cout << " EL = " << el . m_Num << endl;
            queue< int > q;
            q . push ( el . m_Num );
            while ( ! q . empty() )
            {
                int x = q . front();
                q . pop();
                cout << "  X = " << x << endl;
                if ( ! el . AddContact ( x ) )
                    continue;

                cout << "   added = " << el . m_Contacts . back() << endl;
                auto pos = lower_bound ( m_Tree . begin(), m_Tree . end(), x, TNode::LessNum );
                for ( auto & neighbor : pos -> m_Direct )
                {
                    if ( neighbor . m_Time . InTime ( from, to ) )
                        q . push ( neighbor . m_Num );
                }
            }
            if ( el . m_Contacts . size() < max )
                continue;
            if ( el . m_Contacts . size() > max )
            {
                max = el . m_Contacts . size();
                res . clear();
            }
            res . push_back ( el . m_Num );
        }
        cout << "MAX = " << max << endl;
        for ( const auto & el : res )
            cout << el << endl;
        cout << "==================" << endl;
        return res;
    }

    void printTree ( void )
    {
        for ( const auto & el : m_Tree )
        {
            cout << el . m_Num << ": ";
            for ( const auto & cont : el . m_Direct )
                cout << cont . m_Num << " ";
            cout << endl;
        }
        cout << "============" << endl;
    }
};

#ifndef __PROGTEST__
int main ()
{
    CEFaceMask test;

    test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 111111111, 222222222 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 222222222 ) )
        . addContact ( CContact ( CTimeStamp ( 2021, 2, 14, 15, 30, 28 ), 222222222, 444444444 ) );
    test . addContact ( CContact ( CTimeStamp ( 2021, 2, 15, 18, 0, 0 ), 555555555, 444444444 ) );
    assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222}) );




  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 111111111, 222222222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 333333333, 222222222 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 14, 15, 30, 28 ), 222222222, 444444444 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 15, 18, 0, 0 ), 555555555, 444444444 ) );
  assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222}) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 20, 18, 0, 0 ), 444444444, 666666666 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 3, 25, 0, 0, 0 ), 111111111, 666666666 ) );
  assert ( test . getSuperSpreaders ( CTimeStamp ( 2021, 1, 1, 0, 0, 0 ), CTimeStamp ( 2022, 1, 1, 0, 0, 0 ) ) == (vector<int> {222222222, 444444444}) );
  return 0;
}
#endif /* __PROGTEST__ */
