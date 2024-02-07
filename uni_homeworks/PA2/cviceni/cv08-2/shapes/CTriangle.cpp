#include "CTriangle.h"

double CTriangle::getCircumference() const
{
    return x + y + z;
}

double CTriangle::getContent() const
{
    double s = ( x + y + z ) / 2;
    return s * ( s - x ) * ( s - y ) * ( s - z );    
}

