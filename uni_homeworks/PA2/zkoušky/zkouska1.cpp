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

template< typename T_ >
class Array
{
    class Iterator
    {        
      public:
        Iterator ( size_t pos )
         : index ( pos ) {}

        Iterator & operator ++ ( int )
        {
            index ++;
        }        

        T_ & operator * ( void )
        {
            return m_Arr [ index ];
        }

        bool operator == ( const Iterator & it )
        {
            return index == it;
        }

      private:
        size_t index;
    };

  public:
    Array ( void )
    {
        m_Arr = new T_ [ m_Capacity ];
    }

    Array ( const Array & src )
     : m_Size ( src . m_Size ), m_Capacity ( src . m_Capacity )
    {
        delete [] m_Arr;
        m_Arr = new T_ [ m_Capacity ];
        for ( size_t i = 0; i < m_Size; i ++ )
        {
            m_Arr [ i ] = src . m_Arr [ i ];
        }
    }

    Array & operator = ( const Array & src )
    {
        if ( this == & src )
            return *this;

        m_Size = src . m_Size;
        m_Capacity = src . m_Capacity;
        delete [] m_Arr;
        m_Arr = new T_ [ m_Capacity ];
        for ( size_t i = 0; i < m_Size; i ++ )
        {
            m_Arr [ i ] = src . m_Arr [ i ];
        }
    }

    ~Array ( void )
    {
        delete [] m_Arr;
    }

    bool Insert ( size_t pos, const T_ & el )
    {
        if ( pos > m_Size )
            return false;

        if ( m_Size + 1 >= m_Capacity )
            resize();

        for ( size_t i = m_Size; i > pos ; i -- )
        {
            m_Arr [ i ] = m_Arr [ i - 1 ];
        }
        m_Arr [ pos ] = el;
        m_Size ++;
        return true;
    }

    bool Erase ( size_t pos )
    {
        if ( pos >= m_Size )
            return false;
    
        m_Size --;
        for ( size_t i = pos; i < m_Size; i ++ )
        {
            m_Arr [ i ] = m_Arr [ i + 1 ];
        }
        return true;
    }

    size_t Size ( void ) const
    {
        return m_Size;
    }

    const T_ & operator [] ( size_t pos ) const
    {
        if ( pos >= m_Size )
            throw std::invalid_argument();
        return m_Arr [ pos ];
    }

    T_ & operator [] ( size_t pos )
    {
        if ( pos >= m_Size )
            throw std::invalid_argument();
        return m_Arr [ pos ];
    }

    void Print ( void )
    {
        for ( size_t i = 0; i < m_Size; i ++ )
        {
            cout << m_Arr [ i ] << " ";
        }
        cout << endl;
    }

    Iterator begin ( void )
    {
        return Iterator ( 0 );
    }

    Iterator end ( void )
    {
        return Iterator ( m_Size );
    }

  private:
    T_ * m_Arr = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 100;

    void resize ( void )
    {
        m_Capacity *= 2;
        T_ * tmp = new T_ [ m_Capacity ];
        for ( size_t i = 0; i < m_Size; i ++ )
        {
            tmp [ i ] = m_Arr [ i ];
        }
        delete [] m_Arr;
        m_Arr = tmp;
    }
};

int main ( void )
{
    Array< int > a;
    a . Insert ( 0, 1 );
    a . Insert ( 0, 2 );
    a . Insert ( 0, 3 );
    a . Insert ( 3, 10 );
    a . Insert ( 4, 11 );
    a . Insert ( 5, 12 );
    a . Print();
    a . Insert ( 0, 20 );
    a . Insert ( 1, 21 );
    a . Insert ( 2, 22 );
    a . Insert ( 3, 23 );
    a . Print();
    assert ( a . Size() == 10 );
    assert ( ! a . Insert ( 80, 1 ) );

    a . Erase ( 5 );
    a . Print();
    a . Erase ( 5 );
    a . Print();
    a . Erase ( 0 );
    a . Print();

    return 0;
}