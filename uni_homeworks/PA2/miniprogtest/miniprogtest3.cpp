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

const int NUM_SIZE = 9;

class TList
{
    // Locat type declaration
    struct TContact {
        string first_name;
        string last_name;
        string number;
    };

public:
    // Variables
    bool end;

    // Constructor
    TList
    ( const string & fileName )
    : end(false)
    {
        file . open ( fileName );
    }
    ~TList ()
    {
        file . close ();
    }

    // Methods
    bool readData ()
    {
        string line;
        
        while ( getline ( file, line ) )
        {
            //std::cout << "LINE: " << line << std::endl;
            if ( line == "" ) // end of data
            {
                //printData();
                return true;
            }
            if ( ! appendContact ( line ) )
            {
                //std::cout << "FALSE: APPENDCONTACT" << std::endl;
                return false;
            }
        }
        return false;
    };

    bool readQuery ( ostream & out ) // str.contains()
    {
        string line;
        unsigned int pos = 0;
        if ( file . eof () )
        {
            end = true;
            return true;
        }
        getline ( file, line );
        skipWhite ( line, pos );
        if ( pos == line . size() )
        {
            end = true;
            return true;
        }
        if ( file . bad () )
            return false;

        int count = 0;
        for ( auto c = data . begin(); c != data . end(); c ++ )
        {
            //cout << "Line: " << line << " | First: " << c -> first_name << " | Last: " << c -> last_name << endl;
            if ( c -> first_name == line ||
                c -> last_name == line )
            {
                count ++;
                out << c -> first_name << " " << c -> last_name << " " << c -> number << endl;
            }
        }
        out << "-> " << count << endl;
        return true;
    };

    void printData () const
    {
        for ( auto c = data . begin(); c != data . end(); c ++ )
        {
            cout << c -> first_name << endl;
            cout << c -> last_name << endl;
            cout << c -> number << endl;
            cout << "===========" << endl;
        }
    }

private:
    // Variables
    ifstream file;
    vector <TContact> data;
    
    // Methods
    bool appendContact ( const string & line )
    {
        TContact tmp;
        unsigned int from = 0, to;

        bool a, b, c, d, e;

        if ( ! (
            (a = getAtribut ( line, from, to, tmp . first_name )) &&
            (b = getAtribut ( line, from, to, tmp . last_name )) &&
            (c = getAtribut ( line, from, to, tmp . number ))
            )
            || (d = ( skipWhite ( line, from ), line . size() != from )) // not end
            || (e = ! checkNumber ( tmp . number )) )
            {
                //std::cout << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << ", e = " << e << std::endl;                
                return false;
            }
        data . push_back ( tmp );
        return true;
    };

    bool getAtribut ( const string & line, unsigned int & from, unsigned int & to, string & attr )
    {
        skipWhite ( line, from );
        attr = findText ( line, from, to );
        //std::cout << "Attr:" << attr << std::endl;
        if ( attr == "" )
            return false;
        from += to;
        return true;
    }

    string findText ( const string & line, const unsigned int start, unsigned int & end )
    {
        end = 0;
        for ( auto c = line . begin() + start; c != line . end(); c ++, end ++ )
            if ( isspace ( * c ) )
                break;                
        if ( end > 0 )
            return line . substr ( start, end );
        else
            return "";
    };

    void skipWhite ( const string & line, unsigned int & pos )
    {
        for ( auto c = line . begin () + pos; c != line . end (); c ++, pos ++ )
            if ( ! isspace ( * c ) )
                return;
    };

    bool checkNumber ( const string & num )
    {
        if ( num . size () != NUM_SIZE
            || num [0] == '0' )
            return false;
        for ( auto c = num . begin(); c != num . end(); c ++ )
            if ( ! isdigit ( * c ) )
                return false;
        return true;
    }
};

// PROGTEST function
bool report ( const string & fileName, ostream & out )
{
    TList list ( fileName );
    if ( ! list . readData() )
        return false;
    while ( ! list . end )
    {
        if ( ! list . readQuery ( out ) )
            return false;
    }
    return true;
}

#ifndef __PROGTEST__
int main ()
{
    assert ( report( "in_5019603.bin", cout ) == true );
  assert ( report( "tests/test0_in.txt", cout ) == true );
    return 0;


    // PROGTEST
  ostringstream oss;
  assert ( report( "tests/test0_in.txt", std::cout ) == true );
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
