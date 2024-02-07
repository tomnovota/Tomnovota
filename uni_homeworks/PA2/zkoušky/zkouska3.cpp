#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <set>
#include <cassert>

using namespace std;

class CTable
{
  public:
    // CTable ( void );
    // ~CTable ( void );
    // CTable ( const CTable & src );
    // CTable & operator = ( const CTable & src );

    bool Add ( const string & cell, const string & deps )
    {
        string tmp;
        stringstream ss ( cell ), dd ( deps );
        if ( ! readCell ( ss, tmp ) )
            return false;

        const auto [ it, success ] = m_Map . insert ( make_pair ( tmp, vector< string > () ) );
        if ( ! success )
            it -> second . clear();

        while ( readCell ( dd, tmp ) )
        {
            it -> second . push_back ( tmp );
            if ( dd . eof() ) // no separator
                return true;
        }
        return deps . empty();
    }

    bool Detect ( void ) const
    {
        print();
        set< string > closed;
        set< string > opened;

        for ( const auto & el : m_Map )
        {
            if ( DFS ( el . first, closed, opened ) )
                return true;
        }
        return false;
    }

  private:
    map< string, vector< string > > m_Map;

    bool readCell ( stringstream & ss, string & tmp )
    {
        tmp . clear();
        getline ( ss, tmp, ',' );
        if ( ! checkString ( tmp ) )
            return false;
        
        return true;
    }

    bool checkString ( const string & s )
    {
        size_t i = 0, check;
        while ( i < s . size() && isalpha ( s [i] ) )
            i ++;
        
        if ( i == 0 )
            return false;

        check = i;
        while ( i < s . size() && isdigit ( s [i] ) )
            i ++;
        
        return check != i && i == s . size();
    }

    bool DFS ( const string & node, set< string > & closed, set< string > & opened ) const
    {
        opened . insert ( node );
        auto pos = m_Map . find ( node );
        if ( pos != m_Map . end() && pos -> first == node )
        {
            for ( const auto & el : pos -> second )
            {
                if ( ! closed . count ( el ) )
                {
                    if ( opened . count ( el ) || DFS ( el, closed, opened ) )
                        return true;
                }
            }
        }
        opened . erase ( node );
        closed . insert ( node );
        return false;
    }

    void print () const
    {
        cout << "============" << endl;
        for ( const auto & el : m_Map )
        {
            cout << el . first << ": ";
            for ( const auto & dep : el . second )
                cout << dep << " ";
            cout << endl;
        }
    }
};

int main ( void )
{
    CTable a;
    assert ( a . Add ( "a5", "b3,a3,c8" ) );
    assert ( a . Add ( "a4", "b3,a3,c8" ) );
    assert ( a . Add ( "a3", "b1" ) );
    assert ( a . Add ( "b1", "c8" ) );
    assert ( a . Add ( "a4", "a3,c8" ) );

    assert ( ! a . Detect() );

    assert ( a . Add ( "c8", "a5" ) );
    assert ( a . Detect() );

    assert ( ! a . Add ( "a5a", "b3,a3,c8" ) );
    assert ( ! a . Add ( "a5", "b3,a3,c8," ) );
    assert ( ! a . Add ( "a5,", "b" ) );
    assert ( ! a . Add ( "a", "b3,a3,c8" ) );
    assert ( ! a . Add ( "5", "b3,a3,c8" ) );

    return 0;
}