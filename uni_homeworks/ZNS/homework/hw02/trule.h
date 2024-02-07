#ifndef TRULE_H
#define TRULE_H

#include <vector>
#include "tfact.h"
#include <memory>

typedef enum { R_FACT, R_NOT, R_AND, R_OR, R_EQUAL } TRuleItemType;

class TRuleItem
{
public:
    TRuleItem( TRuleItemType type_);
    TRuleItem( TFact *fact_);
    TRuleItem( string leftAtom_, string rightAtom_);
    ~TRuleItem();
    void print();
    void updateListOfArguments( vector<string> &list);
    TRuleItemType getType();
    TFact *translateFact( map<string,string> &dictionary);
    bool compareAtoms( map<string,string> &dictionary);

    TFact GetFact ( void ) { return * fact; }

private:
    TRuleItemType type;
    TFact *fact;
    string leftAtom, rightAtom;
};

using namespace std;

class TRule
{
public:
    TRule( const char *rule);
    ~TRule();
    void print();
    int argNum();
    TFact *evaluate( vector<string> args, vector<TFact *> facts);

// Jak zjistím seznam predikátů použitých v pravidle?

//     Upravte si třídu TRule podle svých záměrů.
//     Pro predikáty se používá třída TFact, stejně jako pro fakty.
//     Jsou schované ve třídě TRuleItem v instancích, které mají nastavený
//     typ na R_FACT. Typ zjistíte voláním TRuleItem::getType().

    vector< shared_ptr< TFact > > Conditions ( void )
    {
        vector< shared_ptr< TFact > > m_conditions;
        for ( auto & item : ruleItems )
        {
            if ( item -> getType() == R_FACT )
                m_conditions . push_back ( make_shared< TFact > ( item -> GetFact() ) );
        }
        return m_conditions;
    }

    vector<string> & GetList ( void ) { return listOfArguments; }

private:
    vector<TRuleItem*> ruleItems;
    TFact *ruleFact;
    vector<string> listOfArguments;
};

vector<string> createListOfAtoms( vector<TFact*> facts);

#endif // TRULE_H
