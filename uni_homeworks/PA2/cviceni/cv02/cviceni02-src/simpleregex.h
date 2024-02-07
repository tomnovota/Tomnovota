#ifndef SIMPLEREGEX_H
#define SIMPLEREGEX_H

#include <string>

class SimpleRegex
{
public:
    // Constructor,..
    SimpleRegex ()
    : pattern("\0") {}
    ~SimpleRegex () {}

    // Methods
    void setPattern ( const std::string & pat );
    bool matchText ( const std::string & text ) const;
    static int getCounter ();

private:
    // Variables
    static int count;
    std::string pattern;

    // Methods
    bool checkLetter ( const char a, const char b ) const;
};

#endif // ifndef