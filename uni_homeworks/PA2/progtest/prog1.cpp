#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;
#endif /* __PROGTEST__ */

//istream:: gcount, read, peek

const int TAIL_LEN = 6;
const int BYTE_SIZE = 8;
const size_t BUFFER_SIZE = 100;
const int FIBO_LEN = 30;

const int fiboArr [FIBO_LEN] =
{
    0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, //15
    987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, // 9
    75025, 121393, 196418, 317811, 514229, 832040 // 6
};

int pow2 ( int val )
{
    int res = 1;
    for ( int i = 0; i < val; i ++ )
        res *= 2;
    return res;
}

bool readBit ( const char & bajt , const int & pos )
{
    return bajt & ( 1 << pos );
}

bool readBit ( const int & val, const int & pos )
{
    return val & ( 1 << pos );
}

void accessBit ( int & val, const int & pos )
{
    val = val | ( 1 << pos );
}

void accessBit ( char & val, const int & pos )
{
    val = val | ( 1 << pos );
}

class CUtf8
{
  public:
    // Constructor, ...
    CUtf8 ( const char * fileName, const char * fileName2 )
    : eof (false), error (false)
    {
        input . open ( fileName, ios::binary | ios::in );
        output . open ( fileName2, ios::binary | ios::out );
        if ( ! ( input . is_open() && output . is_open() ) )
            error = true;
    }
    ~CUtf8 ()
    {
        input . close ();
        output . close ();
    }
    
    // Variables
    bool eof;
    bool error;

    // Methods
    bool decodeChar ( int & val )
    {
        val = 0;
        char bajt;
        if ( ! input . get ( bajt ) && input . bad() )
            return false;
        if ( input . eof() )
        {
            eof = true;
            return true;
        }
        if ( ! getVal ( bajt, val ) )
        {
            error = true;
            return false;
        }
        return true;
    };

    bool codeChar ( const int & val )
    {
        int parts = getParts ( val );
        if ( parts == 1 )
        {
            if ( ! output . put ( val ) )
                return false;
            return true;
        }
        if ( ! fillCode ( val, parts ) )
            return false;
        return true;
    };

  private:
    // Variables
    ifstream input;
    ofstream output;

    // Methods

        // CODING
    bool fillCode ( const int & val, const int & parts )
    {
            // -- FIRST BYTE --
        char code = 0x00;
        int rpos = BYTE_SIZE * parts - ( (parts - 1) * 2 + parts + 1 ) - 1;
        int apos = BYTE_SIZE - 1;

        for ( int i = 0; i < parts; i ++, apos -- )
            accessBit ( code, apos );
        apos --;
        for ( ; apos >= 0; rpos --, apos -- )
            if ( readBit ( val, rpos ) )
                accessBit ( code, apos );
        if ( ! output . put ( code ) )
            return false;

            // -- TAILS --
        for ( int i = 1; i < parts; i ++, apos += 2 ) 
        {
            code = 0x00;
            apos = TAIL_LEN - 1;
            accessBit ( code, BYTE_SIZE - 1 ); // HEADLINE
            for ( ; apos >= 0; rpos --, apos -- )
                if ( readBit ( val, rpos ) )
                    accessBit ( code, apos );
            if ( ! output . put ( code ) )
                return false;
        }
        return true;
    };
    
    int getParts ( const int & val )
    {
        int cnt = 31;
        for ( ; cnt > 0; cnt -- )
            if ( readBit ( val, cnt ) )
                break;
        cnt ++;
        return cnt < BYTE_SIZE ? 1 : ( cnt < 12 ? 2 : ( cnt < 17 ? 3 : 4 ) );
    };

        // DECODING
    bool getVal ( char & bajt, int & val )
    {
        int parts;
        if ( ! getPartsCount ( bajt, parts ) )
            return false;

        int maxBit, first;
        initParams ( first, maxBit, parts );
        fillVal ( bajt, first, maxBit, val );
        for ( int i = 1; i < parts; i ++ )
            if ( ! getTail ( maxBit, val ) )
                return false;
        if ( val > 0x10FFFF )
            return false;
        return true;
    };

    bool getPartsCount ( const char & bajt, int & parts )
    {
        parts = 1;
        int current = 7;
        if ( readBit ( bajt, current ) == 0 )
            return true;
        for ( ; readBit ( bajt, -- current ); parts ++ )
            if ( parts >= 4 )
                return false;
        if ( parts < 2 )
            return false;
        return true;
    };

    void initParams ( int & first, int & maxBit, const int & parts )
    {
        if ( parts == 1 )
            first = maxBit = BYTE_SIZE;
        else
        {
            first = BYTE_SIZE - ( parts + 1 );
            maxBit = ( parts - 1 ) * TAIL_LEN + first; // 11 bits
        }
    };

    bool getTail ( int & maxBit, int & val )
    {
        char bajt;
        input . get ( bajt );
        if ( input . eof() || ! checkTail ( bajt ) )
            return false;
        fillVal ( bajt, TAIL_LEN, maxBit, val );
        return true;
    };

    bool checkTail ( const char & bajt )
    {
        if ( ! readBit ( bajt, 7 ) || readBit ( bajt, 6 ) )
            return false;
        return true;
    };

    void fillVal ( const char & bajt, const int & len, int & maxBit, int & val )
    {
        for ( int i = len - 1; i >= 0; i -- )
        {
            maxBit --;
            if ( readBit ( bajt, i ) )
                accessBit ( val, maxBit );
        }
    };
};

class CFibonacci
{
  public:
    // Constructor, ...
    CFibonacci ( const char * fileName, const char * fileName2 )
    : eof (false), error (false), rest (0), code(0)
    {
        input . open ( fileName, ios::binary | ios::in );
        output . open ( fileName2, ios::binary | ios::out );
        if ( ! ( input . is_open() && output . is_open() ) )
            error = true;
        if ( ! input . get ( bajt ) )
            error = true;
    }
    ~CFibonacci ()
    {
        input . close ();
        output . close ();
    }
    
    // Variables
    bool eof;
    bool error;

    // Methods
    bool decodeChar ( int & val )
    {        
        if ( input . eof() )
        {
            eof = true;
            if ( input . bad() )
                return false;
            return true;
        }
        if ( ! getVal ( val ) )
        {
            error = true;
            return false;
        }
        val --;
        if ( val > 0x10FFFF )
            return false;
        return true;
    };

    bool codeChar ( int val )
    {
        val ++; // FIBO
        const int * pos = lower_bound ( &fiboArr[0], &fiboArr[FIBO_LEN - 1], val );
        if ( * pos > val )
            pos --;

        codeRec ( pos, val );
        if ( error )
            return false;
        accessBitF ( true ); // end
        return true;
    };

    bool flush ()
    {
        if ( rest != 0 )
            if ( ! output . put ( code ) )
                return false;
        return true;
    };

  private:
    // Variables
    ifstream input;
    ofstream output;

    char bajt;
    int rest;
    char code;

    // Methods

    // CODING
    void codeRec ( const int * & pos, int & val )
    {
        if ( * pos == 0 || error )
            return;
        if ( val >= * pos )
        {
            val -= * pos;
            pos --;
            codeRec ( pos, val );
            accessBitF ( true );
            if ( * pos == 0 )
                return;
            accessBitF ( false );
            pos --;
        } else {
            pos --;
            codeRec ( pos, val );
            accessBitF ( false );
        }
    };

    void accessBitF ( bool write )
    {
        if ( rest == BYTE_SIZE )
        {
            if ( ! output . put ( code ) )
                error = true;
            rest = 0;
            code = 0x00;
        }
        if ( write )
        {
            accessBit ( code, rest );
        }
        rest ++;
    };

        // DECODING
    bool getVal ( int & val )
    {
        val = 0;
        bool prev_bit = 0, current_bit = readBitF();
        for ( int pos = 1; ! ( prev_bit && current_bit ); pos ++ )
        {
            if ( eof )
                return val == 0 && ! error;
            if ( pos >= FIBO_LEN )
                return false;
            if ( current_bit )
                val += fiboArr [pos];

            prev_bit = current_bit;
            current_bit = readBitF();
            if ( error )
                return false;
        }
        return true;
    };

    bool readBitF ()
    {
        if ( rest == BYTE_SIZE )
        {
            rest = 0;
            if ( ! input . get ( bajt ) && input . bad() )
                error = true;
            if ( input . eof () )
            {
                eof = true;
                return false;
            }
        }
        return readBit ( bajt, rest ++ );
    };
};

bool utf8ToFibonacci ( const char * inFile, const char * outFile )
{
    CUtf8 utf ( inFile, outFile );
    CFibonacci fibo ( inFile, outFile );
    if ( utf . error || fibo . error )
        return false;
    int val;
    while ( utf . decodeChar ( val ) )
    {
        if ( utf . eof )
        {
            if ( ! fibo . flush () )
                return false;
            return true;
        }
        if ( ! fibo . codeChar ( val ) )
            return false;
    }
    return false;
}
  
bool fibonacciToUtf8 ( const char * inFile, const char * outFile )
{
    CUtf8 utf ( inFile, outFile );
    CFibonacci fibo ( inFile, outFile );
    if ( utf . error || fibo . error  )
        return false;
    int val;
    while ( fibo . decodeChar ( val ) )
    {
        if ( fibo . eof )
            return true;
        if ( ! utf . codeChar ( val ) )
            return false;
    }
    return false;
}
  
#ifndef __PROGTEST__

bool identicalFiles ( const char * file1, const char * file2 )
{
    ifstream f1, f2;
    f1 . open ( file1, ios_base::binary ); // first file
    f2 . open ( file2, ios_base::binary ); // second file

    if ( ! f1 . is_open() || ! f2 . is_open() ) // if error while opening files occured
        return false;

    char c1, c2;
    while ( f1 . get ( c1 ) && f2 . get ( c2 ) ) // while characters in both streams
    {
        if ( c1 != c2 ) // character mismatch
            return false;
    }
    f1 . close ();
    f2 . close ();
    return true;
}

int main ( int argc, char * argv [] )
{
    assert ( readBit ( static_cast<char> (0x01), 0 ) == 1 );
    assert ( readBit ( static_cast<char> (0x02), 0 ) == 0 );
    assert ( readBit ( static_cast<char> (0xF0), 3 ) == 0 );
    assert ( readBit ( static_cast<char> (0xF0), 4 ) == 1 );
    assert ( readBit ( static_cast<char> (0xFF), 3 ) == 1 );

    int bajt = 0x00;
    accessBit ( bajt, 4 );
    assert ( bajt == 0x10 );


  assert ( ! fibonacciToUtf8 ( "in_5025084.bin", "output.utf8" ) );

    return 0;


    // PROGTEST
  assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_0.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_1.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_2.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_3.fib" ) );
  assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_4.fib" ) );
  assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );
  assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );
  assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );
 /*!!!*/ assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );
  assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );
 
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
