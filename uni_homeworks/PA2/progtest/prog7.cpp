#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template< typename M_ >
class CDuel
{
  public:
    CDuel ( void ) = default;
    ~CDuel ( void ) = default;

    CDuel ( const string & contestant1, const string & contestant2, M_ result )
     : m_Contestant1 ( contestant1 )
     , m_Contestant2 ( contestant2 )
     , m_Result ( result ) {}

    string GetC1 ( void ) const { return m_Contestant1; }
    string GetC2 ( void ) const { return m_Contestant2; }
    M_ GetResult ( void ) const { return m_Result; }

    bool Duplicit ( const string & name1, const string & name2 ) const
    {
        return name1 == m_Contestant1 ? name2 == m_Contestant2 : name2 == m_Contestant1 ? name1 == m_Contestant2 : false;
    }

    bool operator < ( const CDuel< M_ > & duel ) const
    {
        return m_Contestant1 == duel . m_Contestant1 ? m_Contestant2 < duel . m_Contestant2 : m_Contestant1 < duel . m_Contestant1;
    }

  private: // neobsahuje smyčky a n-1 cesta, DFS
    string m_Contestant1;
    string m_Contestant2;
    M_ m_Result;
};

class CNode
{
  public:
    CNode ( void ) = default;

    bool Fresh ( void ) const { return m_Fresh; }
    bool Opened ( void ) const { return m_Opened; }
    bool Closed ( void ) const { return m_Closed; }

    vector< string > & getLosers ( void ) { return m_Losers; }
    string getNext ( void ) const { return m_Next; }
    size_t getPath ( void ) const { return m_Longest_Path; }

    void SetOpened ( void ) { m_Opened = true; m_Fresh = false; }

    void SetClosed ( const string & next, size_t len )
    {
        m_Closed = true;
        m_Opened = false;
        m_Fresh = false;
        m_Next = next;
        m_Longest_Path = len;
    }

    void addLoser ( const string & loser )
    {
        m_Losers . push_back ( loser );
    }

    vector< string > m_Losers;

    string m_Next;
    size_t m_Longest_Path = 0;
    bool m_Fresh = true;
    bool m_Opened = false;
    bool m_Closed = false;
};

template< typename M_ >
class CContest
{
  public:
    CContest ( void ) = default;
    ~CContest ( void ) = default;

    CContest< M_ > & addMatch ( string contestant1, string contestant2, M_ result )
    {
        for ( const auto & el : m_Duels )
            if ( el . Duplicit ( contestant1, contestant2 ) )
                throw logic_error ( "Duplicit." );

        m_Duels . push_back ( CDuel< M_ > ( contestant1, contestant2, result ) );
        return *this;
    }

    bool isOrdered ( std::function< char ( M_ ) > comparator ) const noexcept
    {
        try { results ( comparator ); }
        catch ( const std::logic_error& exception ) { return false; }
        return true;
    }

    list< string > results ( std::function< char ( M_ ) > comparator ) const
    {
        map< string, CNode > search_tree;
        if ( ! constructTree ( comparator, search_tree ) )
            throw logic_error ( "Tie present." );

        list< string > res;
        for ( auto & el : search_tree )
        {
            if ( DFS ( el . first, search_tree ) == search_tree . size() - 1 )
            {
                string contestant = el . first;
                for ( size_t i = 0; i < search_tree . size(); i ++ )
                {
                    res . push_back ( contestant );
                    contestant = search_tree [ contestant ] . getNext();
                }
                return res;
            }
        }
        throw logic_error ( "Unambiguous." );
        return res;
    }

    size_t DFS ( const string & contestant, map< string, CNode > & search_tree ) const
    {
        CNode & current = search_tree [ contestant ];
        current . SetOpened();
        
        string next;
        size_t max = 0, len = 0;
        for ( auto & loser : current . getLosers() )
        {
            if ( search_tree [ loser ] . Opened() ) // cyklus
                throw logic_error ( "Unambiguous." );

            else if ( search_tree [ loser ] . Fresh() )
                len = DFS ( loser, search_tree );

            else if ( search_tree [ loser ] . Closed() )
                len = search_tree [ loser ] . getPath();

            if ( len >= max )
            {
                max = len + 1;
                next = loser;
            }
        }
        current . SetClosed ( next, max );
        return max;
    }

    bool constructTree ( std::function< char ( M_ ) > comparator, map< string, CNode > & search_tree ) const
    {
        for ( const auto & duel : m_Duels )
        {
            char result = comparator ( duel . GetResult() );
            if ( result == 0 )
                return false; // tie

            if ( result > 0 )
                addNode ( duel . GetC1(), duel . GetC2(), search_tree );
            else
                addNode ( duel . GetC2(), duel . GetC1(), search_tree );
        }
        return true;
    }

    void addNode ( const string & winner, const string & loser, map< string, CNode > & search_tree ) const
    {
        const auto & [ it, success ] = search_tree . insert ( make_pair ( winner, CNode () ) );
        it -> second . addLoser ( loser );
    }

  private:
    // Variables
    vector< CDuel< M_ > > m_Duels;

    // Methods

};

#ifndef __PROGTEST__
struct CMatch
{
  public:
    CMatch ( int a, int b )
      : m_A ( a )
      , m_B ( b ) {}
    
    int m_A;
    int m_B;
};

class HigherScoreThreshold
{
  public:
    HigherScoreThreshold ( int diffAtLeast )
      : m_DiffAtLeast ( diffAtLeast ) {}

    int operator () ( const CMatch & x ) const
    {
        return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }

  private:
    int m_DiffAtLeast;    
};

int HigherScore ( const CMatch & x )
{
    return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int main ( void )
{

    /* PROGTEST */
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
