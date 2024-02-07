//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.1 - Inferenční systém s dopředným řetězením
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//-------------------------------------------------------------------------

#include "parser.h"
#include "texception.h"

TParserData parser( const char **source)
{
    TParserData p;

    if ((*source) == NULL)
        throw TException("Parser: argument is NULL");

    while (**source == ' ') (*source)++;
    switch (**source) {
        case 0: { p.type = P_NULL; break; }
        case '(': { p.type = P_LEFT; (*source)++; break; }
        case ')': { p.type = P_RIGHT; (*source)++; break; }
        case ',': { p.type = P_COMMA; (*source)++; break; }
        case '=': {
                    p.type = P_EQUAL;
                    (*source)++;
                    if (**source == '=') (*source)++;
                    break;
                  }
        case '\\': {
                    p.type = P_NOTEQUAL;
                    (*source)++;
                    if (**source != '=')
                        throw TException("Parser: Probably you mean '\\='.");
                    (*source)++;
                    break;
                   }
        default:
            if (((**source >= 'A') && (**source <= 'Z')) ||
                ((**source >= 'a') && (**source <= 'z')) ||
                ((**source) == '_')) {
                int num = 1;
                p.value[0] = *((*source)++);
                while (((**source >= 'A') && (**source <= 'Z')) ||
                       ((**source >= 'a') && (**source <= 'z')) ||
                       ((**source >= '0') && (**source <= '9')) ||
                       ((**source) == '_')) {

                    if (num >= MAX_IDENT_LEN-1) {
                        p.value[ MAX_IDENT_LEN-1] = 0;
                        throw TException("Parser: Identifier is too long: " + string( p.value));
                    }
                    p.value[ num++] = *((*source)++);
                }
                p.value[ num] = 0;
                p.type = P_IDENT;
            }
            else
                throw TException("Parser: syntax error\nhere --> " + string( *source));
    }
    return p;
}

void parserCheck( TParserProductType exceptedType, TParserData &data, const char *line)
{
    if (exceptedType != data.type) {
        if (exceptedType == P_LEFT) throw TException("Syntax error: Left bracket is excepted: " + string( line));
        if (exceptedType == P_RIGHT) throw TException("Syntax error: Right bracket is excepted: " + string( line));
        if (exceptedType == P_COMMA) throw TException("Syntax error: Comma is excepted: " + string( line));
        if (exceptedType == P_IDENT) throw TException("Syntax error: An identifier is excepted: " + string( line));
        if (exceptedType == P_NULL) throw TException("Syntax error: End of line is excepted: " + string( line));
        if (exceptedType == P_EQUAL) throw TException("Syntax error: '=' is excepted: " + string( line));
        if (exceptedType == P_NOTEQUAL) throw TException("Syntax error: '\\=' is excepted: " + string( line));
    }
}
