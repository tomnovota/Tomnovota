#ifndef __PROGTEST__
#include <cstring>
#include <iostream>
using namespace std;

class CLinkedSetTester;
#endif /* __PROGTEST__ */


class CLinkedSet
{
  private:
    struct CNode
    {
        CNode * m_Next;
        char * m_Val;

        CNode ( void )
         : m_Next ( nullptr ), m_Val ( nullptr ) {}

        CNode ( const char * val )
         : m_Next ( nullptr ), m_Val ( new char [ strlen ( val ) + 1 ] )
        {
            memcpy ( m_Val, val, strlen ( val ) + 1 );
        }

        const char * Value () const
        {
            return m_Val;
        }

        CNode * Copy ( void )
        {
            CNode * node = new CNode ( m_Val );
            node -> m_Next = m_Next ? m_Next -> Copy() : nullptr;
            return node;
        }

        void Delete ( void )
        {
            delete [] m_Val;
            if ( m_Next != nullptr )
            {
                m_Next -> Delete();
                delete m_Next;
            }
        }

        bool Less ( const char * val )
        {
            return strcmp ( m_Val, val ) < 0;
        }
    };

    CNode * m_Begin;
    int m_Size = 0;

  public:
    // default constructor
    CLinkedSet ( void )
     : m_Begin ( nullptr ) {}

    // copy constructor
    CLinkedSet ( const CLinkedSet & src )
     : m_Size ( src . Size() )
    {
        if ( src . m_Begin != nullptr )
            m_Begin = src . m_Begin -> Copy();
        else
            m_Begin = src . m_Begin;
    }

    // operator=
    CLinkedSet & operator = ( const CLinkedSet & src )
    {
        if ( this == & src )
            return * this;

        m_Size = src . Size();
        if ( m_Begin != nullptr )
        {
            m_Begin -> Delete();
            delete m_Begin;
        }
        if ( src . m_Begin != nullptr )
            m_Begin = src . m_Begin -> Copy();
        else
            m_Begin = nullptr;
        return * this;
    }

    // destructor
    ~CLinkedSet ( void )
    {
        if ( m_Begin != nullptr )
        {
            m_Begin -> Delete();
            delete m_Begin;
        }
    }

    void print ( void )
    {
        CNode * tmp = m_Begin;
        while ( tmp )
        {
            cout << tmp -> m_Val << endl;
            tmp = tmp -> m_Next;
        }
        cout << "=================" << endl;
    }

    CNode * LowerBound ( const char * value, bool & found ) const
    {
        CNode * tmp = m_Begin;
        CNode * prev = nullptr;
        while ( tmp && tmp -> Less ( value ) )
        {
            prev = tmp;
            tmp = tmp -> m_Next;
        }
        found = tmp ? ( ! strcmp ( tmp -> Value(), value ) ) : false;
        return prev;
    }

    bool Insert ( const char * value )
    {
        bool found;
        CNode * prev = LowerBound ( value, found );
        if ( found )
            return false;

        m_Size ++;
        CNode * tmp = new CNode ( value );
        if ( ! prev ) // first node
        {
            tmp -> m_Next = m_Begin;
            return ( m_Begin = tmp );
        }
        tmp -> m_Next = prev -> m_Next;
        prev -> m_Next = tmp;
        return true;
    }
    
    bool Remove ( const char * value )
    {
        bool found;
        CNode * prev = LowerBound ( value, found );
        if ( ! found )
            return false;

        m_Size --;

        CNode * removed;
        if ( prev == nullptr )
        {
            removed = m_Begin;
            m_Begin = removed -> m_Next;
        }
        else
        {
            removed = prev -> m_Next;
            prev -> m_Next = removed -> m_Next;
        }
        delete [] removed -> m_Val;
        delete removed;
        return true;
    }

    bool Empty () const
    {
        return m_Begin == nullptr;
    }

    size_t Size () const
    {
        return m_Size;
    }

    bool Contains ( const char * value ) const
    {
        bool found;
        LowerBound ( value, found );
        return found;
    }

    friend class ::CLinkedSetTester;
};

#ifndef __PROGTEST__
#include <cassert>

struct CLinkedSetTester
{
    static void test0()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        assert( !x0 . Empty() );
    }

    static void test1()
    {
        CLinkedSet x0;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        CLinkedSet x1 ( x0 );
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

    static void test2()
    {
        CLinkedSet x0;
        CLinkedSet x1;
        assert( x0 . Insert( "Jerabek Michal" ) );
        assert( x0 . Insert( "Pavlik Ales" ) );
        assert( x0 . Insert( "Dusek Zikmund" ) );
        assert( x0 . Size() == 3 );
        assert( x0 . Contains( "Dusek Zikmund" ) );
        assert( !x0 . Contains( "Pavlik Jan" ) );
        assert( x0 . Remove( "Jerabek Michal" ) );
        assert( !x0 . Remove( "Pavlik Jan" ) );
        x1 = x0;
        assert( x0 . Insert( "Vodickova Saskie" ) );
        assert( x1 . Insert( "Kadlecova Kvetslava" ) );
        assert( x0 . Size() == 3 );
        assert( x1 . Size() == 3 );
        assert( x0 . Contains( "Vodickova Saskie" ) );
        assert( !x1 . Contains( "Vodickova Saskie" ) );
        assert( !x0 . Contains( "Kadlecova Kvetslava" ) );
        assert( x1 . Contains( "Kadlecova Kvetslava" ) );
    }

};

int main ()
{
    CLinkedSetTester::test0();
    CLinkedSetTester::test1();
    CLinkedSetTester::test2();
    return 0;
}
#endif /* __PROGTEST__ */
