#include <iostream>
#include <vector>
#include <cassert>
#include <string>

using namespace std;

template< class T_ >
class CMatrixDiagonal
{
  public:
    CMatrixDiagonal ( size_t size, size_t width )
     : m_Size ( size )
    {
        Resize ( width );
    }
    /*
    ~CMatrixDiagonal ( void );
    CMatrixDiagonal ( const CMatrixDiagonal & src );
    CMatrixDiagonal & operator = ( const CMatrixDiagonal & src );
    */

    bool IsSet ( size_t row, size_t col ) const noexcept
    {
        return max ( row, col ) < m_Size // out of matrix
            && min ( row - col, col - row ) < m_Width; // width bigger than current
    }

    void Resize ( size_t width )
    {
        if ( width > m_Size || width = 0 )
            throw invalid_argument ( "Width bigger than matrix!" );

        if ( width == m_Width )
            m_Arr . resize ( arraySize ( width ) );
        m_Width = width;
    }

    T_ & operator () ( size_t row, size_t col )
    {
        if ( ! IsSet ( row, col ) )
            throw invalid_argument ( "Index out of range!" );

        return m_Arr [ arrayPos ( row, col ) ];
    }

    const T_ & operator () ( size_t row, size_t col ) const
    {
        if ( ! IsSet ( row, col ) )
            throw invalid_argument ( "Index out of range!" );
        
        return m_Arr [ arrayPos ( row, col ) ];
    }

  private:
    vector< T_ > m_Arr;
    size_t m_Size;
    size_t m_Width = 0;

    size_t arraySize ( size_t width ) const
    {
        return width * ( m_Size + m_Size - width + 1 ) - m_Size;
    }

    size_t arrayPos ( size_t row, size_t col ) const
    {
        if ( row == col ) // main diagonal
            return row;

        size_t width_level = min ( row - col, col - row );

        return arraySize ( width_level ) + ( row > col ? col : m_Size - ( m_Width - 1 ) + row );
    }
};

int main ( void )
{
    CMatrixDiagonal< int > a ( 5, 1 );
    assert ( a . IsSet ( 0, 0 ) );
    assert ( a . IsSet ( 1, 1 ) );
    assert ( a . IsSet ( 2, 2 ) );
    assert ( a . IsSet ( 3, 3 ) );
    assert ( a . IsSet ( 4, 4 ) );

    assert ( ! a . IsSet ( 0, 1 ) );
    assert ( ! a . IsSet ( 1, 0 ) );
    assert ( ! a . IsSet ( 5, 0 ) );
    assert ( ! a . IsSet ( 0, 5 ) );
    assert ( ! a . IsSet ( 2, 3 ) );

    a ( 0, 0 ) = 2;
    a ( 2, 2 ) = 4;
    a ( 4, 4 ) = 5;

    bool ok = true;
    try
    {
        a ( 1, 0 ) = 5;
        ok = false;
    }
    catch ( ... )
    {
    }
    assert ( ok );
    try
    {
        a ( 5, 5 ) = 5;
        ok = false;
    }
    catch ( ... )
    {
    }
    assert ( ok );

    CMatrixDiagonal< string > b ( 8, 4 );
    assert ( b . IsSet ( 0, 0 ) );
    assert ( b . IsSet ( 4, 4 ) );
    assert ( b . IsSet ( 7, 7 ) );
    assert ( b . IsSet ( 3, 0 ) );
    assert ( b . IsSet ( 4, 2 ) );
    assert ( b . IsSet ( 1, 4 ) );

    assert ( ! b . IsSet ( 8, 8 ) );
    assert ( ! b . IsSet ( 4, 0 ) );
    assert ( ! b . IsSet ( 1, 5 ) );

    b ( 0, 0 ) = "a";
    b ( 1, 1 ) = "h";
    b ( 2, 2 ) = "o";
    b ( 3, 3 ) = "j";
    b ( 4, 4 ) = "k";
    b ( 5, 5 ) = "o";
    b ( 6, 6 ) = "t";
    b ( 7, 7 ) = "e";
    b ( 2, 0 ) = "x";
    b ( 3, 5 ) = "y";
    b ( 1, 3 ) = "z";

    return 0;
}