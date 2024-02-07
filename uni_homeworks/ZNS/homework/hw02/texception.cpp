//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.1 - Inferenční systém s dopředným řetězením
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//-------------------------------------------------------------------------

#include "texception.h"

const char *TException::what() const throw()
{
    return s.c_str();
}
