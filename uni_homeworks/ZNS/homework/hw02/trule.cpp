//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.1 - Inferenční systém s dopředným řetězením
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//-------------------------------------------------------------------------

#include "trule.h"
#include "tfact.h"
#include "parser.h"
#include "texception.h"
#include <algorithm>
#include <map>

using namespace std;

TRuleItem::TRuleItem(TRuleItemType type_)
{
    type = type_;
    fact = NULL;
}

TRuleItem::TRuleItem(TFact *fact_)
{
    type = R_FACT;
    fact = fact_;
}

TRuleItem::TRuleItem(string leftAtom_, string rightAtom_)
{
    type = R_EQUAL;
    fact = NULL;
    leftAtom = leftAtom_;
    rightAtom = rightAtom_;
}

TRuleItem::~TRuleItem()
{
    if (fact) {
        delete fact;
        fact = NULL;
    }
}

void TRuleItem::print()
{
    switch (type) {
        case R_FACT: {
            fact->print();
            break;
        }
        case R_NOT: {
            printf("not");
            break;
        }
        case R_AND: {
            printf("and");
            break;
        }
        case R_OR: {
            printf("or");
            break;
        }
        case R_EQUAL: {
            printf("(%s=%s)", leftAtom.c_str(), rightAtom.c_str());
            break;
        }
    }
}

void TRuleItem::updateListOfArguments(vector<string> &list)
{
    switch (type) {
        case R_FACT: {
                        vector<string> a = fact->getArguments();
                        for (auto i=a.begin(); i != a.end(); i++) {
                            if (find( list.begin(), list.end(), *i) == list.end())
                                list.push_back( *i);
                        }
                        break;
                     }
        case R_EQUAL: {
                        if (find( list.begin(), list.end(), leftAtom) == list.end())
                                list.push_back( leftAtom);
                        if (find( list.begin(), list.end(), rightAtom) == list.end())
                                list.push_back( rightAtom);
                        break;
                      }
        default: break;
    }
}

TRuleItemType TRuleItem::getType()
{
    return type;
}

TFact *TRuleItem::translateFact(map<string, string> &dictionary)
{
    if (type != R_FACT) return NULL;
    return fact->translate( dictionary);
}

bool TRuleItem::compareAtoms( map<string, string> &dictionary)
{
    return dictionary[ leftAtom] == dictionary[ rightAtom];
}

TRule::TRule( const char *rule)
{
    string id;
    TParserData p;

    listOfArguments.clear();

    // condition
    while (1) {
        p = parser( &rule);
        parserCheck( P_IDENT, p, rule);
        id = string( p.value);

        if (id == "not") {
            ruleItems.push_back( new TRuleItem( R_NOT));
        }
        else
        {
            p = parser( &rule);
            if (p.type == P_NOTEQUAL) {
                ruleItems.push_back( new TRuleItem( R_NOT));
                p.type = P_EQUAL;
            }
            if (p.type == P_EQUAL) {
                string leftAtom = id;
                p = parser( &rule);
                parserCheck( P_IDENT, p, rule);
                id = string( p.value);
                ruleItems.push_back( new TRuleItem( leftAtom, id));
            }
            else
            {
                parserCheck( P_LEFT, p, rule);
                ruleItems.push_back( new TRuleItem( new TFact( id, &rule)));
            }

            p = parser( &rule);
            parserCheck( P_IDENT, p, rule);
            id = string( p.value);

            if (id == "then") break;

            if (id == "and") {
                ruleItems.push_back( new TRuleItem( R_AND));
            }
            else
            if (id == "or") {
                ruleItems.push_back( new TRuleItem( R_OR));
            }
            else
                throw TException("Missing then/and/or");
        }
    }

    // then
    ruleFact = new TFact( &rule);

    p = parser( &rule);
    parserCheck( P_NULL, p, rule);

    // generate list of arguments
    for (auto r = ruleItems.begin(); r != ruleItems.end(); r++) {
        (*r)->updateListOfArguments( listOfArguments);
    }

    // check for unknown arguments in the fact
    vector<string> args = ruleFact->getArguments();
    for (auto r = args.begin(); r != args.end(); r++) {
        if (find( listOfArguments.begin(), listOfArguments.end(), *r) == listOfArguments.end())
            throw TException("Unknown atom '" + (*r) + "' in the argument of the fact.");
    }
}

TRule::~TRule()
{
    for (auto i = ruleItems.begin(); i != ruleItems.end(); ++i) {
        delete (*i);
    }
    ruleItems.clear();
    listOfArguments.clear();

    delete ruleFact;
    ruleFact = NULL;
}

void TRule::print()
{
    bool prvni = true;
    for (auto i = ruleItems.begin(); i != ruleItems.end(); ++i) {
        if (!prvni) printf(" ");
        else prvni = false;
        (*i)->print();
    }
    printf(" => ");
    ruleFact->print();
}

int TRule::argNum()
{
    return listOfArguments.size();
}

// Pokud je newFact == NULL, potom nelze pravidlo vyhodnotit.
// V opačném případě je vrácen důsledek pravidla.
// Parametr argNames obsahuje vektor s dosazovanými atomy.
// Parametr facts obsahuje všechny doposud známé fakty.
TFact *TRule::evaluate( vector<string> args, vector<TFact*> facts)
{
    if (args.size() != listOfArguments.size())
        throw TException("Internal error: Size of the vector passed as parameter to TRule::evaluate() and size of listOfArguments must be same.");

    map<string,string> dictionary;
    auto key = listOfArguments.begin();
    auto value = args.begin();
    while (key != listOfArguments.end()) {
        dictionary[*key] = *value;
        key++;
        value++;
    }

    bool negation = false;
    bool result = true;

    for (auto r = ruleItems.begin(); r != ruleItems.end(); r++) {
        switch ((*r)->getType()) {
            case R_FACT: {
                TFact *tf = (*r)->translateFact( dictionary);

                result = false;
                for (auto f = facts.begin(); f != facts.end(); f++) {
                    if ((*f)->compare(tf)) {
                        result = true;
                        break;
                    }
                }
                delete tf;

                if (negation) {
                    negation = false;
                    result = !result;
                }
                break;
            }
            case R_NOT: {
                negation = !negation;
                break;
            }
            case R_AND: {
                if (!result) return NULL;
                break;
            }
            case R_OR: {
                if (result) break;
                break;
            }
            case R_EQUAL: {
                result = (*r)->compareAtoms( dictionary);
                if (negation) {
                    negation = false;
                    result = !result;
                }
                break;
            }
        }
    }
    if (!result) return NULL;

    return ruleFact->translate( dictionary);
}

vector<string> createListOfAtoms( vector<TFact*> facts)
{
    vector<string> atoms, atomsInFact;

    for (auto f = facts.begin(); f != facts.end(); f++) {
        atomsInFact = (*f)->getArguments();
        for (auto a = atomsInFact.begin(); a != atomsInFact.end(); a++) {
            if (find( atoms.begin(), atoms.end(), *a) == atoms.end())
                atoms.push_back(*a);
        }
    }
    return atoms;
}
