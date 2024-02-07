#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
#define EXTENDED_SYNTAX

class CRange
{
  public:
    // constructor
    CRange ( long long lo, long long hi );
    CRange ( long long val );

    // Methods - ukázka
    friend bool operator == ( const CRange& l, const CRange& r );
    friend ostream& operator << ( ostream& oss, const CRange& r );

    bool contains ( const CRange& val ) const;
    bool common ( const CRange& val ) const;
    bool commonLight ( const CRange& val ) const;
    bool commonUpper ( const CRange& val );
    bool commonLower ( const CRange& val );
    long long getLo () const { return lo; }
    long long getHi () const { return hi; }
    void setLo ( long long val ) { lo = val; }
    void setHi ( long long val ) { hi = val; }

    static bool compare ( const CRange& x, const CRange& y ) // ošetřit MIN,MAX
    {
        return x.getHi() == LLONG_MAX ? false : x . getHi() + 1 < y . getLo();
    }

    static bool compareLight ( const CRange& x, const CRange& y ) // ošetřit MIN,MAX
    {
        return x . getHi() < y . getLo();
    }

  private:
    // Variables
    long long lo;
    long long hi;

    // Methods
};

class CRangeList
{
  public:
    // constructor
    CRangeList ();
    CRangeList ( const long long a );
    CRangeList ( const CRange& a );
    CRangeList ( std::initializer_list<CRange> values )
    {
        for ( auto c : values )
        {
            append ( c );
        }
    }
    /*
    CRangeList ( const CRangeList& a );
    ~CRangeList ();
    CRangeList& operator = ( const CRangeList& r );
    */

    bool includes ( const CRange& a ) const;

    friend CRangeList& operator += ( CRangeList& l, const CRangeList& r );
    friend CRangeList& operator -= ( CRangeList& l, const CRangeList& r );

    friend bool operator == ( const CRangeList& l, const CRangeList& r );
    friend bool operator != ( const CRangeList& l, const CRangeList& r );

//    friend CRangeList operator + ( const CRangeList& l, const CRange& r );
//    friend CRangeList operator - ( const CRangeList& l, const CRange& r );

    friend ostream& operator << ( ostream& oss, const CRangeList& r );

    void print ()
    {
        cout << "Výpis!" << endl;
        for ( const auto& c : list )
            cout << c << endl;
    }

    class Iterator
    {
      public:
        Iterator ( std::vector<CRange>::iterator begin )
        : current ( begin ) {}

        Iterator operator ++ ()
        {
            ++ current;
            return *this;
        }

        bool operator != ( const Iterator& other ) const
        {
            return current != other . current;
        }

        CRange& operator * () const
        {
            return *current;
        }

      private:
        std::vector<CRange>::iterator current; // iterator for current position
    };

    Iterator begin ()
    {
        return Iterator ( list . begin () );
    }

    Iterator end ()
    {
        return Iterator ( list . end() );
    }

  private:
    // Variables
    vector<CRange> list;

    // Methods
    void append ( const CRange& val );
    bool erase ( const CRange& val );
};

CRange::CRange ( long long lo, long long hi )
{
    if ( lo > hi )
        throw std::logic_error ("");
    this -> lo = lo;
    this -> hi = hi;
}

CRange::CRange ( long long val )
{
    this -> lo = val;
    this -> hi = val;
}

bool operator == ( const CRange& l, const CRange& r )
{
    if ( l . getLo() == r . getLo() && l . getHi() == r . getHi() )
        return true;
    return false;
}

CRangeList operator + ( const CRange& l, const CRange& r )
{
    CRangeList tmp ( l );
    tmp += r;
    return tmp;
}

CRangeList operator + ( const CRangeList& l, const CRange& r )
{
    CRangeList tmp = l;
    tmp += r;
    return tmp;
}

CRangeList operator - ( const CRange& l, const CRange& r )
{
    CRangeList tmp ( l );
    tmp -= r;
    return tmp;
}

CRangeList operator - ( const CRangeList& l, const CRange& r )
{
    CRangeList tmp = l;
    tmp -= r;
    return tmp;
}

CRangeList::CRangeList () {}

CRangeList::CRangeList ( long long val )
{
    list . push_back ( CRange ( val ) );
}

CRangeList::CRangeList ( const CRange& a )
{
    list . push_back ( a );
}

bool CRangeList::includes ( const CRange& val ) const
{
    const auto& c = lower_bound ( list . begin(), list . end(), val, CRange::compare );
    if ( c != list . end() && c -> contains ( val ) )
        return true;
    return false;
}

void CRangeList::append ( const CRange& val )
{
    const auto& c = lower_bound ( list . begin(), list . end(), val, CRange::compare );
    if ( c != list . end() && c -> common ( val ) )
    {
        CRange tmp ( min ( c -> getLo(), val . getLo() ), max ( c -> getHi(), val . getHi() ) );
        list . erase ( c );
        append ( tmp );
        return;
    } else
        list . insert ( c, val );
}

bool CRange::contains ( const CRange& val ) const
{
    if ( lo <= val . getLo() && hi >= val . getHi() )
        return true;
    return false;
}

bool CRange::common ( const CRange& val ) const
{
    if ( ( lo >= val . getLo() && ( val . getHi() == LLONG_MAX ? true : lo <= val . getHi() + 1 ) )
      || ( val . getLo() >= lo && ( hi == LLONG_MAX ? true : val . getLo() <= hi + 1 ) ) )
        return true;
    return false;
}

bool CRange::commonLight ( const CRange& val ) const
{
    if ( ( lo >= val . getLo() && lo <= val . getHi() )
      || ( val . getLo() >= lo && val . getLo() <= hi ) )
        return true;
    return false;
}

bool CRange::commonUpper ( const CRange& val )
{
    if ( val . getLo() > lo && val . getLo() <= hi )
        return true;
    return false;
}

bool CRange::commonLower ( const CRange& val )
{
    if ( val . getHi() < hi && val . getHi() >= lo )
        return true;
    return false;
}

bool CRangeList::erase ( const CRange& val )
{
    const auto& c = lower_bound ( list . begin(), list . end(), val, CRange::compareLight );
    if ( c != list . end() && c -> commonLight ( val ) )
    {
        if ( val . contains ( *c ) )
        {
            list . erase ( c );
            return true;
        }
        CRange tmp1 = *c;
        CRange tmp2 = *c;
        list . erase ( c );
        if ( tmp1 . commonUpper ( val ) && val . getLo() != LLONG_MIN )
        {
            tmp1 . setHi ( val . getLo() - 1 );
            append ( tmp1 );
        }
        if ( tmp2 . commonLower ( val ) && val . getHi() != LLONG_MAX )
        {
            tmp2 . setLo ( val . getHi() + 1 );
            append ( tmp2 );
        }
        return true;
    }
    return false;
}

CRangeList& operator += ( CRangeList& l, const CRangeList& r )
{
    for ( const auto& c : r . list )
        l . append ( c );
    return l;
}

CRangeList& operator -= ( CRangeList& l, const CRangeList& r )
{
    for ( const auto& c : r . list )
        while ( l . erase ( c ) );
    return l;
}


bool operator == ( const CRangeList& l, const CRangeList& r )
{
    if ( l . list . size() != r . list . size() )
        return false;
    auto c = l . list . begin();
    auto d = r . list . begin();
    for ( ; c != l . list . end(); c ++, d ++ )
    {
        if ( ! (*c == *d) )
            return false;        
    }
    return true;
}

bool operator != ( const CRangeList& l, const CRangeList& r )
{
    if ( l == r )
        return false;
    return true;
}

ostream& operator << ( ostream& oss, const CRange& r )
{
    ios_base::fmtflags settings = oss . flags();
    oss << dec;
    if ( r . getLo() == r . getHi() )
    {
        oss << r . getLo();
        oss . flags ( settings );
        return oss;
    }
    oss << "<" << r . lo << ".." << r . hi << ">";
    oss . flags ( settings );
    return oss;
}

ostream& operator << ( ostream& oss, const CRangeList& r )
{
    ios_base::fmtflags settings = oss . flags();
    oss << dec;
    oss << "{";
    for ( auto c = r . list . begin(); c != r . list . end(); c ++ )
    {
        if ( c + 1 != r . list . end() )
            oss << *c << ",";
        else
            oss << *c << "}";            
    }
    if ( r . list . empty() )
        oss << "}";
    oss . flags ( settings );
    return oss;
}

#ifndef __PROGTEST__
string toString ( const CRangeList& x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int main ( void )
{

    // PROGTEST
  CRangeList a, b;

  assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
  a = CRange ( 5, 10 );
  a += CRange ( 25, 100 );
  assert ( toString ( a ) == "{<5..10>,<25..100>}" );
  a += CRange ( -5, 0 );
  a += CRange ( 8, 50 );
  assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
  a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
  a += CRange ( 106, 119 ) + CRange ( 152, 158 );
  assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
  a += CRange ( -3, 170 );
  a += CRange ( -30, 1000 );
  assert ( toString ( a ) == "{<-30..1000>}" );
  b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
  a += b;
  assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( -400, -400 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  try
  {
    a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  b = a;
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 2600, 2700 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 15, 15 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
  assert ( !( a == b ) );
  assert ( a != b );
  assert ( b . includes ( 15 ) );
  assert ( b . includes ( 2900 ) );
  assert ( b . includes ( CRange ( 15, 15 ) ) );
  assert ( b . includes ( CRange ( -350, -350 ) ) );
  assert ( b . includes ( CRange ( 100, 200 ) ) );
  assert ( !b . includes ( CRange ( 800, 900 ) ) );
  assert ( !b . includes ( CRange ( -1000, -450 ) ) );
  assert ( !b . includes ( CRange ( 0, 500 ) ) );
  a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
  assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b += a;
  assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b -= a;
  assert ( toString ( b ) == "{}" );
  b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
  assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
  b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
  assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
#ifdef EXTENDED_SYNTAX
  CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
