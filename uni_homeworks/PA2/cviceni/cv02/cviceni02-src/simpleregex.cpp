#include "simpleregex.h"

int SimpleRegex::count = 0;

void SimpleRegex::setPattern ( const std::string & pat )
{
    pattern = pat;
}

bool SimpleRegex::matchText ( const std::string & text ) const
{
    for ( size_t i = 0; i < pattern . size (); i ++ )
    {
        if ( ! checkLetter ( text [i], pattern [i] ) )
            return false;
    }
    return true;
}

bool SimpleRegex::checkLetter ( const char a, const char b ) const
{
    if ( a != b )
        return false;
    return true;
}

int SimpleRegex::getCounter ()
{
    return count;
}
