#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class TBigram
{
  public:
    // Constructor, ...
    TBigram () {} // implicit
    TBigram ( const unsigned char & a, const unsigned char & b, const unsigned char & c )
    : count (0)
    {
        string tmp;
        tmp = a;
        tmp += b;
        tmp += c;
        bi = tmp;
    }

    // Variables
    string bi;
    int count;

    // Methods
    static bool compare ( const TBigram & x, const TBigram & y )
    {
        return x . count < y . count;
    };
};

class CFreqAnalysis
{
  public:
    // Constructor, ...
    CFreqAnalysis ( const string & fileName )
    {
        file . open ( fileName );
        initDB ();
        output . open ( "reseni.txt" );
    }
    ~CFreqAnalysis ()
    {
        file . close ();        
        output . close ();
    }

    // Variables

    // Methods
    bool getResult ()
    {
        string line;
        string tmp;
        getline ( file, line );
        text = line;
        size = line . size ();
        for ( size_t i = 0; i < size - 3; i ++ )
        {
            tmp = line [i];
            tmp += line [i + 1];
            tmp += line [i + 2];
            increaseScore ( tmp );
        }
        /*if ( ! check() )
            return false;*/
        return true;
    };

    void printResult ( int len )
    {
        sort ( db . begin (), db . end (), &TBigram::compare );
        int i = 0;
        for ( auto c = -- db . end(); i < len; i ++, c -- )
            cout << c -> bi << ": " << c -> count / ((double) size) * 100 << endl;
    };

    void decipher ( int a, int b, int c, int d, int e, int f, int g, int h, int j )
    {
        unsigned char c1, c2, c3;
        unsigned char p1, p2, p3;
        for ( size_t i = 0; i < size - 3; i += 3 )
        {
            c1 = text [i] - 'A';
            c2 = text [i+1] - 'A';
            c3 = text [i+2] - 'A';
            p1 = 'A' + (( a * c1 + b * c2 + c * c3 ) % 26);
            p2 = 'A' + (( d * c1 + e * c2 + f * c3 ) % 26);
            p3 = 'A' + (( g * c1 + h * c2 + j * c3 ) % 26);
            output << p1 << p2 << p3;
        } 
    };

  private:
    // Variables
    ifstream file;
    ofstream output;
    vector <TBigram> db;
    string text;
    size_t size;

    // Methods
    void initDB ()
    {
        string tmp;
        for ( unsigned char c1 = 'A'; c1 <= 'Z'; c1 ++ )
            for ( unsigned char c2 = 'A'; c2 <= 'Z'; c2 ++ )
                for ( unsigned char c3 = 'A'; c3 <= 'Z'; c3 ++ )
                    db . push_back ( TBigram ( c1, c2, c3 ) );
    };

    void increaseScore ( string & sample )
    {
        for ( auto & c : db )
            if ( c . bi == sample )
                (c . count )++;
    };

    bool check () const
    {
        size_t cnt = 0;
        for ( auto & c : db )
            cnt += c . count;
        if ( size != cnt )
            return false;
        return true;
    };
};

int main ( void )
{
    CFreqAnalysis cipher ( "CT.txt" );
    cipher . getResult ();
    cipher . printResult ( 10 ); // frequency of trigrams
    cipher . decipher ( 25, 0, 1, 3, 24, 2, 3, 1, 22 ); // into reseni.txt
    return 0;
}