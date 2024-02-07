//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.1 - Inferenční systém s dopředným řetězením
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//-------------------------------------------------------------------------

#include <stdlib.h>
#include <string>
#include "tfact.h"
#include "texception.h"
#include "parser.h"

using namespace std;

TFact::TFact( const char *fact)
{
    load( &fact);
}

TFact::TFact( const char **fact)
{
    load( fact);
}

TFact::TFact(string id_, const char **factArguments)
{
    id = id_;
    while (1)
    {
        TParserData p = parser( factArguments);
        parserCheck( P_IDENT, p, *factArguments);
        arguments.push_back( string( p.value));

        p = parser( factArguments);
        if (p.type == P_RIGHT) break;
        parserCheck( P_COMMA, p, *factArguments);
    }
}

TFact::TFact( string id_, vector<string> &arguments_)
{
    id = id_;
    arguments = arguments_;
}

TFact::~TFact()
{
    arguments.clear();
}

void TFact::load( const char **fact)
{
    const char *factOrig = *fact;
    TParserData p = parser( fact);
    parserCheck( P_IDENT, p, factOrig);
    id = string( p.value);

    p = parser( fact);
    parserCheck( P_LEFT, p, factOrig);
    while (1)
    {
        p = parser( fact);
        parserCheck( P_IDENT, p, factOrig);
        arguments.push_back( string( p.value));

        p = parser( fact);
        if (p.type == P_RIGHT) break;
        parserCheck( P_COMMA, p, factOrig);
    }
    p = parser( fact);
    parserCheck( P_NULL, p, factOrig);
}

string TFact::getName()
{
    return id;
}

int TFact::getNumberOfArguments()
{
    return arguments.size();
}

vector<string> & TFact::getArguments()
{
    return arguments;
}

TFact *TFact::translate( map<string, string> &dictionary)
{
    vector<string> args;
    for (auto a = arguments.begin(); a != arguments.end(); a++) {
        args.push_back( dictionary[*a]);
    }
    return new TFact( id, args);
}

bool TFact::compare(TFact *fact)
{
    if (id != fact->getName()) return false;
    vector<string> args = fact->getArguments();
    if (args.size() != arguments.size()) return false;
    auto i = arguments.begin();
    auto j = args.begin();
    while (i != arguments.end()) {
        if ((*i) != (*j)) return false;
        i++;
        j++;
    }
    return true;
}

void TFact::print()
{
    printf("%s(", id.c_str());
    bool prvni = true;
    for (auto i = arguments.begin(); i != arguments.end(); ++i) {
        if (prvni) {
            prvni = false;
            printf("%s", (*i).c_str());
        }
        else
            printf(",%s", (*i).c_str());
    }
    printf(")");
}
