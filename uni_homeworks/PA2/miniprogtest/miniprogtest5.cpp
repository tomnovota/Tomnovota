#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

    // helper methods
    int conv () const
    {
        return m_Hour * 3600 + m_Minute * 60 + m_Second;
    }

    static void check ( int& h, int& m, int& s )
    {
        if ( s > 59 )
        {
            s -= 60;
            m ++;
        }
        if ( s < 0 )
        {
            s += 60;
            m --;
        }
        if ( m > 59 )
        {
            m -= 60;
            h ++;
        }
        if ( m < 0 )
        {
            m += 60;
            h --;
        }
        if ( h > 23 )
        {
            h -= 24;
        }
    }

    CTime & check ()
    {
        if ( m_Second > 59 )
        {
            m_Second -= 60;
            m_Minute ++;
        }
        if ( m_Second < 0 )
        {
            m_Second += 60;
            m_Minute --;
        }
        if ( m_Minute > 59 )
        {
            m_Minute -= 60;
            m_Hour ++;
        }
        if ( m_Minute < 0 )
        {
            m_Minute += 60;
            m_Hour --;
        }
        if ( m_Hour > 23 )
        {
            m_Hour -= 24;
        }
        return *this;
    }

public:
    // constructor, destructor
    CTime ()
    : m_Hour(0), m_Minute(0), m_Second(0) {}
    CTime ( int h, int min )
    {
        if ( h < 0 || min < 0 || h > 23 || min > 59 )
            throw "Špatný parametr.";
        m_Hour = h;
        m_Minute = min;
        m_Second = 0;
    }
    CTime ( int h, int min, int s )
    {
        if ( h < 0 || min < 0 || h > 23 || min > 59 || s < 0 || s > 59 )
            throw "Špatný parametr.";
        m_Hour = h;
        m_Minute = min;
        m_Second = s;
    }
    CTime ( CTime& a )
    {
        m_Hour = a . m_Hour;
        m_Minute = a . m_Minute;
        m_Second = a . m_Second;
    }
    ~CTime ()
    {
        m_Hour = 0;
        m_Minute = 0;
        m_Second = 0;
    }
    CTime & operator = ( const CTime&  a )
    {
        m_Hour = a . m_Hour;
        m_Minute = a . m_Minute;
        m_Second = a . m_Second;
        return *this;
    }

    // arithmetic operators
    friend CTime operator + ( const CTime & l, const int r )
    {
        int s = l . m_Second + r;
        int m = l . m_Minute;
        int h = l . m_Hour;
        CTime::check ( h, m, s );

        return CTime ( h, m, s );
    }

    friend CTime & operator += ( CTime & l, const int r )
    {
        l . m_Minute += r;
        return l . check();
    }

    friend CTime operator - ( const CTime & l, const int r )
    {
        int s = l . m_Second - r;
        int m = l . m_Minute;
        int h = l . m_Hour;
        CTime::check ( h, m, s );

        return CTime ( h, m, s );
    }

    friend CTime & operator -= ( CTime & l, const int r )
    {
        l . m_Minute -= r;
        return l . check();
    }

    friend int operator - ( const CTime & l, const CTime & r )
    {
        int tmp = l . conv() - r . conv();
        return tmp < 0 ? - tmp : tmp;
    }

    friend CTime operator ++ ( CTime & l, int )
    {
        CTime tmp = l;
        l . m_Second ++;
        l . check();
        return tmp;
    }

    friend CTime operator -- ( CTime & l, int )
    {
        CTime tmp = l;
        l . m_Second --;
        l . check();
        return tmp;
    }
 
    friend CTime & operator ++ ( CTime & l )
    {
        l . m_Second ++;
        return l . check ();
    }

    friend CTime & operator -- ( CTime & l )
    {
        l . m_Second --;
        return l . check ();
    }

    // comparison operators
    friend bool operator < ( const CTime & l, CTime const & r )
    {
        int x = l . conv ();
        int y = r . conv ();
        if ( x < y )
            return true;
        return false;
    }

    friend bool operator > ( const CTime & l, CTime const & r )
    {
        int x = l . conv ();
        int y = r . conv ();
        if ( x > y )
            return true;
        return false;
    }

    friend bool operator == ( const CTime & l, CTime const & r )
    {
        if ( l . m_Hour == r . m_Hour
            && l . m_Minute == r . m_Minute
            && l . m_Second == r . m_Second )
            return true;
        return false;
    }

    friend bool operator <= ( const CTime & l, CTime const & r )
    {
        if ( l < r || l == r )
            return true;
        return false;
    }

    friend bool operator >= ( const CTime & l, CTime const & r )
    {
        if ( l > r || l == r )
            return true;
        return false;
    }

    friend bool operator != ( const CTime & l, CTime const & r )
    {
        if ( ! ( l == r ) )
            return true;
        return false;
    }

    // output operator
    friend ostream & operator << ( ostream & stream, const CTime & r )
    {
        stream << setw(2) << setfill('0') << r . m_Hour << ':' << setw(2) << setfill('0') << r . m_Minute << ':' << setw(2) << setfill('0') << r . m_Second << flush;
        //cout << setw(2) << setfill('0') << r . m_Hour << ':' << setw(2) << setfill('0') << r . m_Minute << ':' << setw(2) << setfill('0') << r . m_Second << endl;
        return stream;
    }

    friend class ::CTimeTester;
};

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        //cout << "Time. hour = " << time . m_Hour << ", Time . minute = " << time . m_Minute << ", Time . second = " << time . m_Second << endl;
        //cout << "hour = " << hour << ", minute = " << minute << ", second = " << second << endl;
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */
