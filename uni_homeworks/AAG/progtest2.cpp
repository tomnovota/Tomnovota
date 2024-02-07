#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;
using Symbol = char;
using Word = std::vector<Symbol>;

struct Grammar {
    std::set<Symbol> m_Nonterminals;
    std::set<Symbol> m_Terminals;
    std::vector<std::pair<Symbol, std::vector<Symbol>>> m_Rules;
    Symbol m_InitialSymbol;
};
#endif

// se může hodit namapovat si ta pravidla naopak, když to pak člověk procház

// algoritmus: zjistí zda je jazyk generovaný G prázdný
// algoritmus: vyloučení nedostupných symbolů
// algoritmus: vyloučení zbytečných symbolů (zároveň i nedostupných)
// algoritmus: věta o vyloučení pravidla
// algoritmus: vyloučení epsilon pravidel
// vlastní BG: bez cyklů, bez epsilon, bez zbytečných
// algoritmus: vyloučení jednoduchých pravidel
// věta: bez epsilon pravidel + bez jednoduchých pravidel -> bez cyklů
// algoritmus: převod BG do Chomského n.t.

// algoritmus: CYK
// vstup: BG v Ch.n.t., x řetězec abecedy
// výstup: odpověď zda x náleží L(G)

// rekurzivní BG: existuje pravidlo A -> _A_
// algoritmus: vyloučení rekurzivity zleva

// pravidla ve formě L -> T | NN (normální tvar Chomského)
// zjistit, zda slovo patří do jazyka
// pokud ano, najít posloupnost pravidel generující slovo

struct TLeftSide
{
    TLeftSide (  ) = default;
    TLeftSide ( const TLeftSide & src ) = default;

    TLeftSide ( Symbol n )
            : NonT ( n )
    {}

    TLeftSide ( Symbol n, size_t i )
    : NonT ( n ), Index ( i )
    {}

    Symbol NonT;
    size_t Index;
};

struct TElem
{
    TElem (  ) = default;

    explicit TElem ( TLeftSide n )
    : LeftSide ( n )
    {
        left . reset();
        right . reset();
    }

    TElem ( TLeftSide n, std::shared_ptr< TElem > l, std::shared_ptr< TElem > r )
    : LeftSide ( n ), left ( l ), right ( r )
    {}

    TLeftSide LeftSide;
    std::shared_ptr< TElem > left;
    std::shared_ptr< TElem > right;
};

class FLess
{
  public:
    bool operator () ( const std::shared_ptr< TElem > & l, const std::shared_ptr< TElem > & r ) const
    {
        return l -> LeftSide . NonT < r -> LeftSide . NonT;
    }
};

void print ( const std::map< std::pair< Symbol, Symbol >, vector< TLeftSide > > & rm )
{
    std::cout << "RuleMap:" << std::endl;
    for ( const auto & el : rm )
    {
        std::cout << "  " << el . first . first << el . first . second << " <- ";
        for ( const auto & l : el . second )
            std::cout << l . NonT << "[" << l . Index << "]" << ", ";
        std::cout << std::endl;
    }
}

void print ( const std::vector< std::vector< std::set< std::shared_ptr< TElem >, FLess > > > & Array )
{
    std::cout << "  Array:" << std::endl;
    for ( const auto & l : Array )
    {
        std::cout << " | ";
        for ( const auto & r : l )
        {
            for ( const auto & el : r )
                std::cout << el -> LeftSide . NonT << "[" << el -> LeftSide . Index << "]" << ", ";
            std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

void Result ( std::vector < size_t > & arr, std::shared_ptr< TElem > rule )
{
    if ( ! rule )
        return;

    arr . push_back ( rule -> LeftSide . Index );
    Result ( arr, rule -> left );
    Result ( arr, rule -> right );
}

std::vector < size_t > trace ( const Grammar & G, const Word & w )
{
    if ( w . empty() )
    {
        size_t id = 0;
        for ( const auto & rule : G . m_Rules )
        {
            if ( rule . second . empty() )
                return std::vector < size_t > ({id} );
            ++ id;
        }
        return {};
    }
    std::map< std::pair< Symbol, Symbol >, vector< TLeftSide > > RuleMap;
    std::map< Symbol, vector< TLeftSide > > SimpleRules;

    std::vector< std::vector< std::set< std::shared_ptr< TElem >, FLess > > > Array;
    Array . resize ( w . size() );
    for ( auto & l : Array )
        l . resize ( w . size() );

    size_t id = 0;
    for ( const auto & rule : G . m_Rules )
    {
        if ( rule . second . empty() )
        {
            ++ id;
            continue;
        }

        if ( rule . second . size() < 2 )
        {
            const auto [it, success] = SimpleRules . insert ( std::make_pair ( rule . second [0], vector< TLeftSide > () ) );
            it -> second . emplace_back ( rule . first, id ++ );
            continue;
        }
        const auto [it, success] = RuleMap . insert ( std::make_pair ( std::make_pair ( rule . second [0], rule . second [1] ), vector< TLeftSide > () ) );
        it -> second . emplace_back ( rule . first, id ++ );
    }
    for ( size_t i = 0; i < w . size(); ++ i )
    {
        auto it = SimpleRules . find ( w [ i ] );
        if ( it == SimpleRules . end() )
            return {};

        for ( auto A : it -> second )
            Array [ i ] [ 0 ] . insert ( std::make_shared< TElem > ( A ) );
    }
    for ( size_t i = 1; i < w . size(); i ++ ) // sloupce, délka podřetězce
        for ( size_t j = 0; j < w . size() - i; j ++ ) // řádky, pozice podřetězce v textu
            for ( size_t k = 0; k < i; k ++ ) // v jakém sloupci hledám první část, délka první části podřetězce
                for ( const auto & B: Array [ j ] [ k ] ) // od pozice j celkem k-1 symbolů
                    for ( const auto & C: Array [ j + k + 1 ] [ i - k - 1 ] ) // od pozice j+k, i-k symbolů
                    {
                        auto it = RuleMap . find (std::make_pair ( B -> LeftSide.NonT, C -> LeftSide . NonT ) );
                        if ( it != RuleMap . end() )
                        {
                            for ( auto A: it -> second)
                                Array[j][i].insert(std::make_shared<TElem>( A, B, C ) );
                        }
                    }
    auto it = Array [ 0 ] [ w . size() - 1 ] . find ( std::make_shared< TElem > ( G . m_InitialSymbol ) );
    if ( it == Array [ 0 ] [ w . size() - 1 ] . end() )
        return {};
    std::vector < size_t > res;
    Result ( res, * it );
    return res;
}

#ifndef __PROGTEST__

void printVector(string text, const std::vector<size_t>& vec) {
    std::cout << text;
    std::cout << " {";
    for (const auto& element : vec) {
        std::cout << element << ", ";
    }
    std::cout << "}" << std::endl;
    if ( text == "Actual:" )
        std::cout << std::endl;
}

void print ( const Grammar & G )
{
    size_t i = 0;
    for ( const auto & el : G . m_Rules )
    {
        std::cout << "[" << i ++ << "]" << "  " << el . first << " -> ";
        for ( const auto & l : el . second )
            std::cout << l << ", ";
        std::cout << std::endl;
    }
}

int main()
{
    Grammar g0{
        {'A', 'B', 'C', 'S'},
        {'a', 'b'},
        {
            {'S', {'A', 'B'}},
            {'S', {'B', 'C'}},
            {'A', {'B', 'A'}},
            {'A', {'a'}},
            {'B', {'C', 'C'}},
            {'B', {'b'}},
            {'C', {'A', 'B'}},
            {'C', {'a'}},
        },
        'S'};

    Grammar g1{
        {'A', 'B'},
        {'x', 'y'},
        {
            {'A', {}},
            {'A', {'x'}},
            {'B', {'x'}},
            {'A', {'B', 'B'}},
            {'B', {'B', 'B'}},
        },
        'A'};

    Grammar g2{
        {'A', 'B'},
        {'x', 'y'},
        {
            {'A', {'x'}},
            {'B', {'x'}},
            {'A', {'B', 'B'}},
            {'B', {'B', 'B'}},
        },
        'B'};

    Grammar g3{
        {'A', 'B', 'C', 'D', 'E', 'S'},
        {'a', 'b'},
        {
            {'S', {'A', 'B'}},
            {'S', {'S', 'S'}},
            {'S', {'a'}},
            {'A', {'B', 'S'}},
            {'A', {'C', 'D'}},
            {'A', {'b'}},
            {'B', {'D', 'D'}},
            {'B', {'b'}},
            {'C', {'D', 'E'}},
            {'C', {'b'}},
            {'C', {'a'}},
            {'D', {'a'}},
            {'E', {'S', 'S'}},
        },
        'S'};

    std::cout << "  G0:" << std::endl;
//    print ( g0 );

    printVector("Actual:", trace(g0, {'b', 'a', 'a', 'b', 'a'}));
    printVector("Expected:", {0, 2, 5, 3, 4, 6, 3, 5, 7});

    printVector("Expected:", {});
    printVector("Actual:", trace(g0, {'b'}));

    printVector("Expected:", {});
    printVector("Actual:", trace(g0, {'a'}));

    printVector("Expected:", {});
    printVector("Actual:", trace(g0, {}));

    printVector("Expected:", {1, 4, 6, 3, 4, 7, 7, 7, 7});
    printVector("Actual:", trace(g0, {'a', 'a', 'a', 'a', 'a'}));

    printVector("Expected:", {0, 3, 5});
    printVector("Actual:", trace(g0, {'a', 'b'}));

    printVector("Expected:", {1, 5, 7});
    printVector("Actual:", trace(g0, {'b', 'a'}));

    printVector("Expected:", {});
    printVector("Actual:", trace(g0, {'c', 'a'}));

    //    assert(trace(g0, {'b', 'a', 'a', 'b', 'a'}) == std::vector<size_t>({0, 2, 5, 3, 4, 6, 3, 5, 7}));
//    assert(trace(g0, {'b'}) == std::vector<size_t>({}));
//    assert(trace(g0, {'a'}) == std::vector<size_t>({}));
//    assert(trace(g0, {}) == std::vector<size_t>({}));
//    assert(trace(g0, {'a', 'a', 'a', 'a', 'a'}) == std::vector<size_t>({1, 4, 6, 3, 4, 7, 7, 7, 7}));
//    assert(trace(g0, {'a', 'b'}) == std::vector<size_t>({0, 3, 5}));
//    assert(trace(g0, {'b', 'a'}) == std::vector<size_t>({1, 5, 7}));
//    assert(trace(g0, {'c', 'a'}) == std::vector<size_t>({}));

    std::cout << "  G0:" << std::endl;
    printVector("Expected:", {0});
    printVector("Actual:", trace(g1, {}));

    printVector("Expected:", {1});
    printVector("Actual:", trace(g1, {'x'}));

    printVector("Expected:", {3, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x'}));

    printVector("Expected:", {3, 4, 2, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x', 'x'}));

    printVector("Expected:", {3, 4, 4, 2, 2, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x', 'x', 'x'}));

    printVector("Expected:", {3, 4, 4, 4, 2, 2, 2, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x', 'x', 'x', 'x'}));

    printVector("Expected:", {3, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x', 'x', 'x', 'x', 'x'}));

    printVector("Expected:", {3, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x'}));

    printVector("Expected:", {3, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}));

    printVector("Expected:", {3, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2});
    printVector("Actual:", trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}));

    //    assert(trace(g1, {}) == std::vector<size_t>({0}));
//    assert(trace(g1, {'x'}) == std::vector<size_t>({1}));
//    assert(trace(g1, {'x', 'x'}) == std::vector<size_t>({3, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2}));
//    assert(trace(g1, {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}) == std::vector<size_t>({3, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2}));

std::cout << "==============" << std::endl;

//    assert(trace(g2, {}) == std::vector<size_t>({}));
//    assert(trace(g2, {'x'}) == std::vector<size_t>({1}));
//    assert(trace(g2, {'x', 'x'}) == std::vector<size_t>({3, 1, 1}));
//    assert(trace(g2, {'x', 'x', 'x'}) == std::vector<size_t>({3, 3, 1, 1, 1}));

    std::cout << "  G2:" << std::endl;
    printVector("Expected:", {});
    printVector("Actual:", trace(g2, {}));

    printVector("Expected:", {3, 3, 1, 1, 1});
    printVector("Actual:", trace(g2, {'x', 'x', 'x'}));

//    assert(trace(g3, {}) == std::vector<size_t>({}));
//    assert(trace(g3, {'b'}) == std::vector<size_t>({}));
//    assert(trace(g3, {'a', 'b', 'a', 'a', 'b'}) == std::vector<size_t>({1, 2, 0, 3, 7, 1, 2, 2, 7}));
//    assert(trace(g3, {'a', 'b', 'a', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'a'}) == std::vector<size_t>({1, 1, 0, 4, 8, 11, 12, 0, 5, 6, 11, 11, 0, 4, 9, 11, 7, 11, 7, 2, 2}));

    printVector("Expected:", {1, 2, 0, 3, 7, 1, 2, 2, 7});
    printVector("Actual:", trace(g2, {'a', 'b', 'a', 'a', 'b'}));

    printVector("Expected:", {1, 1, 0, 4, 8, 11, 12, 0, 5, 6, 11, 11, 0, 4, 9, 11, 7, 11, 7, 2, 2});
    printVector("Actual:", trace(g3, {'a', 'b', 'a', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'a'}));




}
#endif

