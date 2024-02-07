//-------------------------------------------------------------------------
//
//  BI-ZNS: Šablona pro úlohu č.1 - Inferenční systém s dopředným řetězením
//  (c) 2021 Ladislava Smítková Janků <ladislava.smitkova@fit.cvut.cz>
//
//-------------------------------------------------------------------------
/*
          Často kladené otázky
          --------------------

Jak projdu všecha pravidla uložená ve vektoru rules?

    for (auto rule = rules.begin(); rule != rules.end(); rule++) {
        ...
    }

Jak zjistím počet argumentů pravidla?

    int numOfRuleArgs = (*rule)->argNum();

Jak vyhodnotím pravidlo ?

    TFact *newFact = (*rule)->evaluate( argNames, facts);

    Pokud je newFact == NULL, potom nelze pravidlo vyhodnotit.
    V opačném případě je vrácen důsledek pravidla.
    Parametr argNames obsahuje vektor s dosazovanými atomy.
    Parametr facts obsahuje všechny doposud známé fakty.

Jak porovnám dva fakty?

    f1->compare(f2)

    Porovnává se název faktů i názvy jejich parametrů.

Jak jsou pravidla v paměti organizovaná?

    Všechna pravidla jsou uložena ve vektoru instancí třídy TRule.
    Každé pravidlo (třída TRule) obsahuje vektor instancí třídy TRuleItem.
    Tento vektor vytvořil parser tak, aby namodeloval chování pravidla.
    Zjednodušeně řečeno se jedná o sekvenci faktů a operací. Tato sekvence
    se prochází při každém vyhodnocování pravidla.

Jak zjistím seznam predikátů použitých v pravidle?

    Upravte si třídu TRule podle svých záměrů.
    Pro predikáty se používá třída TFact, stejně jako pro fakty.
    Jsou schované ve třídě TRuleItem v instancích, které mají nastavený
    typ na R_FACT. Typ zjistíte voláním TRuleItem::getType().

===================================================================*/

#include "eval.h"
#include "parser.h"
#include "texception.h"
#include <string.h>

//#define VERBOSE

void loadRule( vector<TFact*> &facts, vector<TRule*> &rules, const char *line)
{
    const char *lineOrig = line;
    TParserData p = parser( &line);
    parserCheck( P_IDENT, p, lineOrig);
    if (strcmp( p.value, "if") != 0) {
        // this is a fact
        facts.push_back( new TFact( lineOrig));
        return;
    }

    // this is a rule
    try {
        rules.push_back( new TRule( line));
    }
    catch (TException e) {
        throw TException("Parse error on this rule: " + string( lineOrig) + "\n" + string( e.what()));
    }
}

// Argumenty a_i termu _t_ obsaženého v pravidle mohou být proměnné a konstanty.
// Argumenty b_i termu _s_ z báze faktů mohou být pouze konstanty.

// Unifikace termu _t_ z pravidla a termu _s_ z báze faktů v této úloze obsahuje evaluaci:

// 1. Je název termů stejný? +
// 2. Je počet argumentů termů t a s stejný? +
// 3. Pro každý a_i termu t a b_i termu s vyhodnoďte: +
//     jestliže a_i je proměnná:
//        - je a_i vázána na jinam než na b_i?  (pokud není, tak je možné povést substituci a_i=b_i)
//     jestliže a_i je konstanta
//        - je  a_i rovna b_i?

struct TPair
{
    TPair ( void ) = default;
    TPair ( string v, string c )
    : variable ( v ), constant ( c )
    {}
    
    friend bool operator < ( const TPair & l, const TPair & r );
    friend bool operator == ( const TPair & l, const TPair & r );

    string variable;
    string constant;
};

bool operator < ( const TPair & l, const TPair & r )
{
    return l . variable < r . variable;
}

bool operator == ( const TPair & l, const TPair & r )
{
    return l . variable == r . variable;
}

struct TDirectory_Var_Const
{
    bool Find ( string variable, string constant )
    {
        auto it = find ( m_set . begin(), m_set . end(), TPair ( variable, "" ) );
        if ( it != m_set . end() )
            return constant == ( it -> constant );

        m_set . push_back ( TPair ( variable, constant ) );
        return true;
    }

    string GetUnify ( string var )
    {
        auto it = find ( m_set . begin(), m_set . end(), TPair ( var, "" ) );
        return it -> constant;
    }

    void reset ( void ) { m_set . clear(); }

    vector< TPair > m_set;
};

bool FactExists ( TFact * fact, vector< TFact * > & facts )
{
    for ( auto & f : facts )
    {
        if ( f -> compare ( fact ) )
            return true;
    }
    return false;
}

bool CompareVarConst ( vector< string > & a, vector< string > & b, TDirectory_Var_Const & v_c )
{
    for ( size_t i = 0; i < a . size(); i ++ )
    {
        if ( ! v_c . Find ( a [i], b [i] ) ) // aktuální fakt není vhodný
        {
            return false;
        }
    }
    return true;
}

vector< TDirectory_Var_Const > rek ( deque< shared_ptr< TFact > > conditions, vector< TFact * > & facts, TDirectory_Var_Const var_const )
{
    if ( conditions . empty() )
        return vector< TDirectory_Var_Const > ( { var_const } ); // triviální podmínka

    vector< TDirectory_Var_Const > result;
    shared_ptr< TFact > condition = conditions . front();
    conditions . pop_front ();
    TDirectory_Var_Const v_c;
    for ( auto & fact : facts )
    {
        if ( condition -> getName() == fact -> getName()
            && condition -> getNumberOfArguments() == fact -> getNumberOfArguments() )
        {
            v_c = var_const;
            auto & a = condition -> getArguments();
            auto & b = fact -> getArguments();
            if ( ! CompareVarConst ( a, b, v_c ) )
                continue;
            
            auto args = rek ( conditions, facts, v_c );
            for ( auto & arg : args )
                result . push_back ( arg );
        }
    }
    return result;
}

vector< vector< string > > GetArguments ( vector< TDirectory_Var_Const > & options, TRule * rule )
{
    vector< vector< string > > args;
    for ( auto & opt : options )
    {
        args . push_back ( vector< string > () );
        auto & args_opt = args . back();
        auto & list = rule -> GetList();
        for ( auto & var : list )
        {
            args_opt . push_back ( opt . GetUnify ( var ) );
        }        
    }
    return args;
}

void solve ( vector< TFact * > & facts, vector< TRule * > & rules, vector< string > atoms )
{
    bool new_fact_added;
    while ( true )
    {
        new_fact_added = false;
        for ( auto rule = rules . begin(); rule != rules . end(); rule ++ )
        {
// 1, Porovnání (matching): zjišťuje se, u kterých pravidel jsou levé části (předpoklady) splněné, porovnáváním s fakty v bázi faktů.

            // získání všech možných unifikací pro toto pravidlo
            auto conditions = ( * rule ) -> Conditions();
            vector< TDirectory_Var_Const > options = rek ( deque< shared_ptr< TFact > > ( conditions . begin(), conditions . end() ), facts, TDirectory_Var_Const () );
            vector< vector< string > > args_opts = GetArguments ( options, * rule );
            for ( auto & args : args_opts )
            {
                if ( TFact * new_fact = ( * rule ) -> evaluate ( args, facts ) ) // consequence of satisfied rule
                {
// 2, Výběr pravidel, jejichž aplikace má za následek odvození nového faktu do báze faktů.
                    if ( FactExists ( new_fact, facts ) )
                        continue; // skip the fact

// 4, Provedení (execution): Vybrané pravidlo má za následek přidání/odebrání faktu / znalosti
                    facts . push_back ( new_fact );
                    new_fact_added = true;
                }
            }
        }
// 5, Opakování výše, dokud lze nějaké fakty odvodit.
        if ( ! new_fact_added )
            break;
    }
}

// Jak zjistím počet argumentů pravidla?
//     int numOfRuleArgs = (*rule)->argNum();

// Jak vyhodnotím pravidlo ?
//     TFact *newFact = (*rule)->evaluate( argNames, facts);

//     Pokud je newFact == NULL, potom nelze pravidlo vyhodnotit.
//     V opačném případě je vrácen důsledek pravidla.
//     Parametr argNames obsahuje vektor s dosazovanými atomy.
//     Parametr facts obsahuje všechny doposud známé fakty.

    // ======================================================================
    // Zde doplňte vhodný kód, který provede doplnění faktů ve vektoru facts.
    // Vektor rules obsahuje pravidla a vektor atoms seznam všech atomů.
    // Po dokončení této procedury musí být ve vektoru facts všechny fakty,
    // které lze podle daných pravidel odvodit.
    // Při implementaci se nemusíte omezovat pouze na tuto metodu. Pokud se
    // vám to hodí, upravte si (třeba v zájmu unifikace) i třídu TRule.
    // ======================================================================
