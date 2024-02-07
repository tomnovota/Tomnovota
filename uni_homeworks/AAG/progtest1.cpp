#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include <algorithm>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include <cassert>
using namespace std;

// struct TState
// {
//     TState ( void ) = default;
//     TState ( const State & src )
//      : m_State ( src )
//     {}

//     friend bool operator < ( const TState & l, const TState & r );

//     bool Fresh ( void ) const { return ! ( m_Closed || m_Opened ); }
//     bool Opened ( void ) const { return m_Opened; }
//     bool Closed ( void ) const { return m_Closed; }

//     State m_State;
//     bool m_Opened = false;
//     bool m_Closed = false;
// };

// bool operator < ( const TState & l, const TState & r )
// {
//     return l . m_State < r . m_State;
// }

using State = unsigned int;
using Symbol = char;

struct MISNFA {
    std::set<State> m_States;
    std::set<Symbol> m_Alphabet;
    std::map<std::pair<State, Symbol>, std::set<State>> m_Transitions;
    std::set<State> m_InitialStates;
    std::set<State> m_FinalStates;

    void print ( const char * mess ) const
    {
        cout << mess << endl;
        cout << "Alphabet: ";
        for ( const auto & el : m_Alphabet )
            cout << el << ", ";

        cout << "States: ";
        for ( const auto & el : m_States )
            cout << el << ", ";

        cout << endl << "Final States: ";
        for ( const auto & el : m_FinalStates )
            cout << el << ", ";

        cout << endl << "Initial_States: ";
        for ( const auto & el : m_InitialStates )
            cout << el << ", ";

        cout << endl << "Transitions: " << endl;
        for ( const auto & el : m_Transitions )
        {
            cout << "( " << el . first . first << ", " << el . first . second << " ) -> " << "{ ";
            for ( const auto & el2 : el . second )     
                cout << el2 << ", ";
            cout << " }" << endl;
        }
        cout << endl;
    }
};

struct DFA {
    std::set<State> m_States;
    std::set<Symbol> m_Alphabet;
    std::map<std::pair<State, Symbol>, State> m_Transitions;
    State m_InitialState;
    std::set<State> m_FinalStates;

    void print ( const char * mess ) const
    {
        cout << mess << endl;
        cout << "Alphabet: ";
        for ( const auto & el : m_Alphabet )
            cout << el << ", ";

        cout << "States: ";
        for ( const auto & el : m_States )
            cout << el << ", ";

        cout << endl << "Final States: ";
        for ( const auto & el : m_FinalStates )
            cout << el << ", ";

        cout << endl << "Initial_State: " << m_InitialState ;

        cout << endl << "Transitions: " << endl;
        for ( const auto & el : m_Transitions )
            cout << "( " << el . first . first << ", " << el . first . second << " ) -> " << el . second << endl;

        cout << endl;
    }

    bool operator==(const DFA& dfa)
    {
        return std::tie(m_States, m_Alphabet, m_Transitions, m_InitialState, m_FinalStates) == std::tie(dfa.m_States, dfa.m_Alphabet, dfa.m_Transitions, dfa.m_InitialState, dfa.m_FinalStates);
    }
};

#endif

State NewState ( const set< State > & Q )
{
    State res = - 1;
    while ( Q . find ( res ) != Q . end() )
        -- res;
    return res;
}

State TSubset_CTR = 0;

void WidenInitial ( MISNFA & progress, const set< State > & p, const State & q0, const Symbol & a )
{
    auto it = progress . m_Transitions . lower_bound ( make_pair ( q0, a ) ); // transition (q0, a) -> {}
    if ( it != progress . m_Transitions . end() &&
        ( it -> first ) == make_pair ( q0, a ) ) // transition (q0, a) -> P2 exists
    {
        for ( const auto & el : p ) // transition (q0, a) -> P1 + P2
            it -> second . insert ( el );
    }
    else
        progress . m_Transitions . insert ( it, make_pair ( make_pair ( q0, a ), p ) ); // transition (q0, a) -> P1

}

void InitialReduction ( const MISNFA & nfa, MISNFA & progress )
{
    progress = nfa;
    if ( nfa . m_InitialStates . size() == 1 ) // already 1 initial state
        return;

    State q0 = NewState ( nfa . m_States ); // new initial state

    // State q0 = * ( nfa . m_InitialStates . begin() ); // new initial state
    progress . m_InitialStates . clear();
    progress . m_InitialStates . insert ( q0 ); // only 1 initial state

    auto q = nfa . m_InitialStates . begin();
    for ( ; q != nfa . m_InitialStates . end(); ++ q ) // iter through all initial states
    {
        if ( nfa . m_FinalStates . find ( * q ) != nfa . m_FinalStates . end() ) // initial state is finale state
            progress . m_FinalStates . insert ( q0 ); // make new initial state finale

        for ( const auto & a : nfa . m_Alphabet ) // iter through alphabet
        {
            auto it = nfa . m_Transitions . find ( make_pair ( * q, a ) ); // transition (q, a) -> {}
            const auto & p = it -> second; // {}
            if ( it != nfa . m_Transitions . end() ) // transition (q, a) -> P1 exists
                WidenInitial ( progress, p, q0, a );
        }
    }
}

struct TSubset
{
    TSubset ( void ) = default;

    TSubset ( const State & q )
    : m_Value ( TSubset_CTR ++ )
    {
        m_Set . insert ( q );
    }

    TSubset ( const TSubset & src )
    : m_Set ( src . m_Set )
    , m_Value ( src . m_Value )
    {}

    TSubset ( TSubset && src )
    : m_Set ( src . m_Set )
    , m_Value ( src . m_Value )
    {}

    void Append ( const set< State > & p )
    {
        for ( const auto & el : p )
            m_Set . insert ( el );
    }

    void Assign ( void )
    {
        m_Value = TSubset_CTR ++;
    }

    State Transform ( void ) const
    { return m_Value; }

    set< State > & Set ( void )
    { return m_Set; }

    friend bool operator == ( const TSubset & l, const TSubset & r );
    friend bool operator < ( const TSubset & l, const TSubset & r );

    set< State > m_Set;
    State m_Value = 0;
};

bool operator == ( const TSubset & l, const TSubset & r )
{
    return l . m_Set == r . m_Set;
}

bool operator < ( const TSubset & l, const TSubset & r )
{
    return l . m_Set < r . m_Set;
}

bool InNewStates ( const deque< TSubset > & new_states, const TSubset & state )
{
    for ( const auto & el : new_states )
        if ( el == state )
            return true;

    return false;
}

void FindNewStates ( deque< TSubset > & new_states, const MISNFA & progress, DFA & dfa, set< TSubset > & seen )
{
    auto q_ = new_states . front();
    auto & q_set = q_ . Set();
    new_states . pop_front();

    for ( const auto & p : q_set )
    {
        if ( progress . m_FinalStates . find ( p ) != progress . m_FinalStates . end() )
        {
            dfa . m_FinalStates . insert ( q_ . Transform() );
            break;
        }
    }

    for ( const auto & a : progress . m_Alphabet )
    {
        TSubset dest_state;
        for ( const auto & p : q_set )
        {
            auto it = progress . m_Transitions . find ( make_pair ( p, a ) ); // transition (p, a) -> {}
            if ( it != progress . m_Transitions . end() ) // transition (p, a) -> P1 exists
            {
                dest_state . Append ( it -> second );
            }
        }
        if ( dest_state . Set() . empty() )
            continue;

        // cout << "q_ : { ";
        // for ( const auto & el : q_set )
        //     cout << el << ", ";
        // cout << " }, " << a << " -> { ";
        // for ( const auto & el : dest_state . Set() )
        //     cout << el << ", ";
        // cout << " }" << endl;

        auto it = seen . find ( dest_state );
        if ( it == seen . end() ) // new destination state
        {
            dest_state . Assign(); // assign number
            dfa . m_Transitions . insert ( make_pair ( make_pair ( q_ . Transform(), a ), dest_state . Transform() ) );
            dfa . m_States . insert ( dest_state . Transform() );
            new_states . push_back ( dest_state );
            seen . insert ( dest_state );
        }
        else // destination state already exists
            dfa . m_Transitions . insert ( make_pair ( make_pair ( q_ . Transform(), a ), it -> Transform() ) );
    }
}

DFA SubsetDeter ( const MISNFA & progress )
{
    DFA dfa;
    deque< TSubset > new_states;
    set< TSubset > seen;
    TSubset q0 ( * (progress . m_InitialStates . begin()) );

    dfa . m_Alphabet = progress . m_Alphabet; // alphabet
    dfa . m_States . insert ( q0 . Transform() ); // initial state
    dfa . m_InitialState = q0 . Transform();

    new_states . push_back ( q0 ); // first new state
    seen . insert ( q0 );
    while ( ! new_states . empty() )
    {
        FindNewStates ( new_states, progress, dfa, seen );
    }
    return dfa;
}

struct BFS
{
    BFS ( const set< State > & src )
    {
        for ( const auto & el : src )
            m_Set . insert ( make_pair ( el, false ) );
    }

    bool Fresh ( const State & st ) const
    {
        return ! m_Set . find ( st ) -> second;
    }

    void Open ( const State & st )
    {
        m_Set . find ( st ) -> second = true;
    }

    map< State, bool > m_Set;
};

void dosazitelny ( DFA & dfa )
{
    set< State > F_;
    set< State > Q_;
    map< pair< State, Symbol >, State > T_;

    BFS bfs ( dfa . m_States );
    deque< State > que;
    bfs . Open ( dfa . m_InitialState );
    que . push_back ( dfa . m_InitialState );

    while ( ! que . empty() )
    {
        auto q = que . front();
        que . pop_front();

        Q_ . insert ( q );
        if ( dfa . m_FinalStates . find ( q ) != dfa . m_FinalStates . end() )
            F_ . insert ( q );

        for ( const auto & a : dfa . m_Alphabet )
        {
            auto it = dfa . m_Transitions . find ( make_pair ( q, a ) );
            if ( it != dfa . m_Transitions . end() )
            {
                auto p = it -> second;
                T_ . insert ( make_pair ( make_pair ( q, a ), p ) );
                if ( bfs . Fresh ( p ) )
                {
                    que . push_back ( p );
                    bfs . Open ( p );
                }
            }
        }
    }
    dfa . m_FinalStates = move ( F_ );
    dfa . m_States = move ( Q_ );
    dfa . m_Transitions = move ( T_ );
}

map< State, set< pair< State, Symbol > > > Transform ( const map< pair< State, Symbol >, State > & t )
{
    map< State, set< pair< State, Symbol > > > res;
    for ( const auto & el : t )
    {
        auto it = res . find ( el . second );
        if ( it != res . end() )
        {
            it -> second . insert ( el . first ); 
        }
        else
        {
            res . insert ( make_pair ( el . second, set< pair< State, Symbol > >( { el . first } ) ) );
        }
    }
    return res;
}

void uzitecny ( DFA & dfa )
{
    set< State > Q_;
    map< pair< State, Symbol >, State > T_;
    Q_ . insert ( dfa . m_InitialState );

    map< State, set< pair< State, Symbol > > > T_tmp = Transform ( dfa . m_Transitions );
    BFS bfs ( dfa . m_States );
    deque< State > que;

    for ( const auto & el : dfa . m_FinalStates )
    {
        bfs . Open ( el );
        que . push_back ( el );
    }

    while ( ! que . empty() )
    {
        auto p = que . front();
        que . pop_front();
        Q_ . insert ( p );
        auto it = T_tmp . find ( p );
        if ( it != T_tmp . end() )
        {
            for ( const auto & q : it -> second )
            {
                T_ . insert ( make_pair ( make_pair ( q . first, q . second ), p ) );
                if ( bfs . Fresh ( q . first ) )
                {
                    que . push_back ( q . first );
                    bfs . Open ( q . first );
                }
            }
        }
    }
    dfa . m_States = move ( Q_ );
    dfa . m_Transitions = move ( T_ );
}

// ošetřit prázdný jazyk
DFA determinize ( const MISNFA & nfa )
{
    // nfa . print ( "Zadání" );
    MISNFA progress;
    InitialReduction ( nfa, progress );
    // progress . print ( "Initial" );
    DFA dfa = SubsetDeter ( progress );
    // dfa . print ( "Subset" );
    dosazitelny ( dfa );
    // dfa . print ( "dosazitelny" );
    uzitecny ( dfa );
    // dfa . print ( "uzitecny" );
    return dfa;
}

#ifndef __PROGTEST__

MISNFA in = {
    {0, 1, 2},
    {'a', 'b', 'c'},
    {
        {{0, 'a'}, {0}},
        {{1, 'b'}, {1}},
        {{2, 'c'}, {2}}
    },
    {0, 1, 2},
    {0, 1, 2},
};

MISNFA in0 = {
    {0, 1, 2},
    {'e', 'l'},
    {
        {{0, 'e'}, {1}},
        {{0, 'l'}, {1}},
        {{1, 'e'}, {2}},
        {{2, 'e'}, {0}},
        {{2, 'l'}, {2}},
    },
    {0},
    {1, 2},
};
DFA out0 = {
    {0, 1, 2},
    {'e', 'l'},
    {
        {{0, 'e'}, 1},
        {{0, 'l'}, 1},
        {{1, 'e'}, 2},
        {{2, 'e'}, 0},
        {{2, 'l'}, 2},
    },
    0,
    {1, 2},
};
MISNFA in1 = {
    {0, 1, 2, 3},
    {'g', 'l'},
    {
        {{0, 'g'}, {1}},
        {{0, 'l'}, {2}},
        {{1, 'g'}, {3}},
        {{1, 'l'}, {3}},
        {{2, 'g'}, {1}},
        {{2, 'l'}, {0}},
        {{3, 'l'}, {1}},
    },
    {0},
    {0, 2, 3},
};
DFA out1 = {
    {0, 1, 2, 3},
    {'g', 'l'},
    {
        {{0, 'g'}, 1},
        {{0, 'l'}, 2},
        {{1, 'g'}, 3},
        {{1, 'l'}, 3},
        {{2, 'g'}, 1},
        {{2, 'l'}, 0},
        {{3, 'l'}, 1},
    },
    0,
    {0, 2, 3},
};
MISNFA in2 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    {'l', 'm'},
    {
        {{0, 'l'}, {1}},
        {{0, 'm'}, {2}},
        {{1, 'l'}, {3}},
        {{2, 'l'}, {4}},
        {{2, 'm'}, {5}},
        {{3, 'l'}, {3}},
        {{4, 'l'}, {3}},
        {{4, 'm'}, {6}},
        {{5, 'l'}, {7}},
        {{5, 'm'}, {6}},
        {{6, 'l'}, {7}},
        {{6, 'm'}, {8}},
        {{7, 'l'}, {9}},
        {{7, 'm'}, {10}},
        {{8, 'l'}, {6}},
        {{8, 'm'}, {10}},
        {{9, 'l'}, {7}},
        {{9, 'm'}, {8}},
        {{10, 'l'}, {11}},
        {{10, 'm'}, {4}},
        {{11, 'l'}, {12}},
        {{11, 'm'}, {9}},
        {{12, 'l'}, {6}},
        {{12, 'm'}, {10}},
    },
    {0},
    {1, 3},
};
DFA out2 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    {'l', 'm'},
    {
        {{0, 'l'}, 1},
        {{0, 'm'}, 2},
        {{1, 'l'}, 3},
        {{2, 'l'}, 4},
        {{2, 'm'}, 5},
        {{3, 'l'}, 3},
        {{4, 'l'}, 3},
        {{4, 'm'}, 6},
        {{5, 'l'}, 7},
        {{5, 'm'}, 6},
        {{6, 'l'}, 7},
        {{6, 'm'}, 8},
        {{7, 'l'}, 9},
        {{7, 'm'}, 10},
        {{8, 'l'}, 6},
        {{8, 'm'}, 10},
        {{9, 'l'}, 7},
        {{9, 'm'}, 8},
        {{10, 'l'}, 11},
        {{10, 'm'}, 4},
        {{11, 'l'}, 12},
        {{11, 'm'}, 9},
        {{12, 'l'}, 6},
        {{12, 'm'}, 10},
    },
    0,
    {1, 3},
};
MISNFA in3 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
    {'a', 'b'},
    {
        {{0, 'a'}, {1}},
        {{0, 'b'}, {2}},
        {{1, 'a'}, {3}},
        {{1, 'b'}, {4}},
        {{2, 'a'}, {5}},
        {{2, 'b'}, {6}},
        {{3, 'a'}, {7}},
        {{3, 'b'}, {8}},
        {{4, 'a'}, {9}},
        {{5, 'a'}, {5}},
        {{5, 'b'}, {10}},
        {{6, 'a'}, {8}},
        {{6, 'b'}, {8}},
        {{7, 'a'}, {11}},
        {{8, 'a'}, {3}},
        {{8, 'b'}, {9}},
        {{9, 'a'}, {5}},
        {{9, 'b'}, {5}},
        {{10, 'a'}, {1}},
        {{10, 'b'}, {2}},
        {{11, 'a'}, {5}},
        {{11, 'b'}, {5}},
    },
    {0},
    {5, 6},
};
DFA out3 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
    {'a', 'b'},
    {
        {{0, 'a'}, 1},
        {{0, 'b'}, 2},
        {{1, 'a'}, 3},
        {{1, 'b'}, 4},
        {{2, 'a'}, 5},
        {{2, 'b'}, 6},
        {{3, 'a'}, 7},
        {{3, 'b'}, 8},
        {{4, 'a'}, 9},
        {{5, 'a'}, 5},
        {{5, 'b'}, 10},
        {{6, 'a'}, 8},
        {{6, 'b'}, 8},
        {{7, 'a'}, 11},
        {{8, 'a'}, 3},
        {{8, 'b'}, 9},
        {{9, 'a'}, 5},
        {{9, 'b'}, 5},
        {{10, 'a'}, 1},
        {{10, 'b'}, 2},
        {{11, 'a'}, 5},
        {{11, 'b'}, 5},
    },
    0,
    {5, 6},
};
MISNFA in4 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {'e', 'j', 'k'},
    {
        {{0, 'e'}, {1}},
        {{0, 'j'}, {2}},
        {{0, 'k'}, {3}},
        {{1, 'e'}, {2}},
        {{1, 'j'}, {4}},
        {{1, 'k'}, {5}},
        {{2, 'e'}, {6}},
        {{2, 'j'}, {0}},
        {{2, 'k'}, {4}},
        {{3, 'e'}, {7}},
        {{3, 'j'}, {2}},
        {{3, 'k'}, {1}},
        {{4, 'e'}, {4}},
        {{4, 'j'}, {8}},
        {{4, 'k'}, {7}},
        {{5, 'e'}, {4}},
        {{5, 'j'}, {0}},
        {{5, 'k'}, {4}},
        {{6, 'e'}, {7}},
        {{6, 'j'}, {8}},
        {{6, 'k'}, {4}},
        {{7, 'e'}, {3}},
        {{7, 'j'}, {1}},
        {{7, 'k'}, {8}},
        {{8, 'e'}, {2}},
        {{8, 'j'}, {4}},
        {{8, 'k'}, {9}},
        {{9, 'e'}, {4}},
        {{9, 'j'}, {0}},
        {{9, 'k'}, {4}},
    },
    {0},
    {1, 6, 8},
};
DFA out4 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {'e', 'j', 'k'},
    {
        {{0, 'e'}, 1},
        {{0, 'j'}, 2},
        {{0, 'k'}, 3},
        {{1, 'e'}, 2},
        {{1, 'j'}, 4},
        {{1, 'k'}, 5},
        {{2, 'e'}, 6},
        {{2, 'j'}, 0},
        {{2, 'k'}, 4},
        {{3, 'e'}, 7},
        {{3, 'j'}, 2},
        {{3, 'k'}, 1},
        {{4, 'e'}, 4},
        {{4, 'j'}, 8},
        {{4, 'k'}, 7},
        {{5, 'e'}, 4},
        {{5, 'j'}, 0},
        {{5, 'k'}, 4},
        {{6, 'e'}, 7},
        {{6, 'j'}, 8},
        {{6, 'k'}, 4},
        {{7, 'e'}, 3},
        {{7, 'j'}, 1},
        {{7, 'k'}, 8},
        {{8, 'e'}, 2},
        {{8, 'j'}, 4},
        {{8, 'k'}, 9},
        {{9, 'e'}, 4},
        {{9, 'j'}, 0},
        {{9, 'k'}, 4},
    },
    0,
    {1, 6, 8},
};
MISNFA in5 = {
    {0, 1, 2, 3},
    {'e', 'n', 'r'},
    {
        {{0, 'e'}, {1}},
        {{0, 'n'}, {1}},
        {{0, 'r'}, {2}},
        {{1, 'e'}, {2}},
        {{1, 'n'}, {3}},
        {{1, 'r'}, {3}},
        {{2, 'e'}, {3}},
        {{2, 'n'}, {3}},
        {{2, 'r'}, {1}},
        {{3, 'e'}, {1}},
        {{3, 'n'}, {1}},
        {{3, 'r'}, {2}},
    },
    {0},
    {0, 3},
};
DFA out5 = {
    {0, 1, 2, 3},
    {'e', 'n', 'r'},
    {
        {{0, 'e'}, 1},
        {{0, 'n'}, 1},
        {{0, 'r'}, 2},
        {{1, 'e'}, 2},
        {{1, 'n'}, 3},
        {{1, 'r'}, 3},
        {{2, 'e'}, 3},
        {{2, 'n'}, 3},
        {{2, 'r'}, 1},
        {{3, 'e'}, 1},
        {{3, 'n'}, 1},
        {{3, 'r'}, 2},
    },
    0,
    {0, 3},
};
MISNFA in6 = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {'e', 't', 'x'},
    {
        {{0, 'e'}, {1}},
        {{0, 't'}, {2}},
        {{0, 'x'}, {3}},
        {{1, 'e'}, {1}},
        {{1, 't'}, {4}},
        {{1, 'x'}, {5}},
        {{2, 'e'}, {3}},
        {{2, 't'}, {6}},
        {{2, 'x'}, {2}},
        {{3, 'e'}, {3}},
        {{3, 't'}, {7}},
        {{3, 'x'}, {4}},
        {{4, 'e'}, {4}},
        {{4, 't'}, {4}},
        {{4, 'x'}, {7}},
        {{5, 'e'}, {5}},
        {{5, 't'}, {5}},
        {{5, 'x'}, {5}},
        {{6, 'e'}, {5}},
        {{6, 't'}, {2}},
        {{6, 'x'}, {0}},
        {{7, 'e'}, {5}},
        {{7, 't'}, {5}},
        {{7, 'x'}, {5}},
    },
    {0},
    {0, 3},
};
DFA out6 = {
    {0, 1, 2, 3},
    {'e', 't', 'x'},
    {
        {{0, 't'}, 1},
        {{0, 'x'}, 2},
        {{1, 'e'}, 2},
        {{1, 't'}, 3},
        {{1, 'x'}, 1},
        {{2, 'e'}, 2},
        {{3, 't'}, 1},
        {{3, 'x'}, 0},
    },
    0,
    {0, 2},
};
MISNFA in7 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    {'d', 'm', 't'},
    {
        {{0, 'd'}, {2}},
        {{0, 'm'}, {0}},
        {{0, 't'}, {3}},
        {{1, 'd'}, {9}},
        {{1, 'm'}, {0}},
        {{1, 't'}, {2}},
        {{2, 'd'}, {3}},
        {{2, 'm'}, {7}},
        {{4, 'd'}, {7}},
        {{4, 'm'}, {1}},
        {{5, 'd'}, {5}},
        {{5, 'm'}, {5}},
        {{5, 't'}, {0}},
        {{6, 'd'}, {7}},
        {{8, 'm'}, {7}},
        {{8, 't'}, {7}},
        {{9, 'd'}, {7}},
        {{9, 'm'}, {1}},
        {{10, 'd'}, {7}},
    },
    {1},
    {0, 5, 6, 10},
};
DFA out7 = {
    {0, 1, 2},
    {'d', 'm', 't'},
    {
        {{0, 'd'}, 1},
        {{0, 'm'}, 2},
        {{1, 'm'}, 0},
        {{2, 'm'}, 2},
    },
    0,
    {2},
};
MISNFA in8 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    {'h', 'm', 'q'},
    {
        {{1, 'h'}, {4}},
        {{1, 'm'}, {3}},
        {{1, 'q'}, {2}},
        {{2, 'h'}, {0}},
        {{2, 'm'}, {0}},
        {{2, 'q'}, {0}},
        {{3, 'q'}, {4}},
        {{4, 'h'}, {7}},
        {{4, 'm'}, {0}},
        {{4, 'q'}, {8}},
        {{5, 'q'}, {9}},
        {{6, 'h'}, {5}},
        {{6, 'm'}, {8}},
        {{6, 'q'}, {6}},
        {{7, 'h'}, {7}},
        {{7, 'q'}, {8}},
        {{9, 'q'}, {4}},
        {{10, 'h'}, {0}},
        {{10, 'm'}, {0}},
        {{10, 'q'}, {0}},
    },
    {1},
    {0, 5, 6},
};
DFA out8 = {
    {0, 1, 2, 3, 4},
    {'h', 'm', 'q'},
    {
        {{0, 'h'}, 1},
        {{0, 'm'}, 2},
        {{0, 'q'}, 3},
        {{1, 'm'}, 4},
        {{2, 'q'}, 1},
        {{3, 'h'}, 4},
        {{3, 'm'}, 4},
        {{3, 'q'}, 4},
    },
    0,
    {4},
};
MISNFA in9 = {
    {0, 1, 2, 3, 4},
    {'h', 'l', 'w'},
    {
        {{0, 'l'}, {1, 2, 3}},
        {{0, 'w'}, {4}},
        {{1, 'h'}, {1}},
        {{1, 'l'}, {3, 4}},
        {{1, 'w'}, {0, 2}},
        {{2, 'h'}, {3}},
        {{2, 'l'}, {1}},
        {{2, 'w'}, {0}},
        {{3, 'h'}, {3}},
        {{3, 'l'}, {0, 3}},
        {{3, 'w'}, {0, 2}},
        {{4, 'l'}, {1, 2, 3}},
        {{4, 'w'}, {4}},
    },
    {1},
    {0, 1, 2, 3, 4},
};
DFA out9 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
    {'h', 'l', 'w'},
    {
        {{0, 'h'}, 0},
        {{0, 'l'}, 1},
        {{0, 'w'}, 2},
        {{1, 'h'}, 3},
        {{1, 'l'}, 4},
        {{1, 'w'}, 5},
        {{2, 'h'}, 3},
        {{2, 'l'}, 6},
        {{2, 'w'}, 7},
        {{3, 'h'}, 3},
        {{3, 'l'}, 8},
        {{3, 'w'}, 2},
        {{4, 'h'}, 9},
        {{4, 'l'}, 10},
        {{4, 'w'}, 5},
        {{5, 'h'}, 3},
        {{5, 'l'}, 6},
        {{5, 'w'}, 7},
        {{6, 'h'}, 9},
        {{6, 'l'}, 11},
        {{6, 'w'}, 2},
        {{7, 'l'}, 6},
        {{7, 'w'}, 12},
        {{8, 'h'}, 3},
        {{8, 'l'}, 4},
        {{8, 'w'}, 5},
        {{9, 'h'}, 9},
        {{9, 'l'}, 13},
        {{9, 'w'}, 2},
        {{10, 'h'}, 9},
        {{10, 'l'}, 10},
        {{10, 'w'}, 5},
        {{11, 'h'}, 9},
        {{11, 'l'}, 10},
        {{11, 'w'}, 5},
        {{12, 'l'}, 6},
        {{12, 'w'}, 12},
        {{13, 'h'}, 3},
        {{13, 'l'}, 4},
        {{13, 'w'}, 5},
    },
    0,
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
};
MISNFA in10 = {
    {0, 1, 2, 3, 4, 5, 6},
    {'j', 'k', 'w'},
    {
        {{0, 'j'}, {0, 5}},
        {{0, 'k'}, {1, 2}},
        {{0, 'w'}, {2}},
        {{1, 'j'}, {0, 1, 2}},
        {{1, 'k'}, {4, 5}},
        {{1, 'w'}, {2}},
        {{2, 'j'}, {0, 1, 2}},
        {{2, 'w'}, {0}},
        {{3, 'j'}, {0, 2}},
        {{3, 'k'}, {4, 6}},
        {{3, 'w'}, {0}},
        {{4, 'j'}, {5}},
        {{5, 'j'}, {5}},
        {{6, 'j'}, {0, 2}},
        {{6, 'k'}, {3, 4}},
        {{6, 'w'}, {0}},
    },
    {2},
    {0, 1, 3, 6},
};
DFA out10 = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {'j', 'k', 'w'},
    {
        {{0, 'j'}, 1},
        {{0, 'w'}, 2},
        {{1, 'j'}, 3},
        {{1, 'k'}, 4},
        {{1, 'w'}, 5},
        {{2, 'j'}, 6},
        {{2, 'k'}, 7},
        {{2, 'w'}, 0},
        {{3, 'j'}, 3},
        {{3, 'k'}, 4},
        {{3, 'w'}, 5},
        {{4, 'j'}, 3},
        {{4, 'w'}, 5},
        {{5, 'j'}, 3},
        {{5, 'k'}, 7},
        {{5, 'w'}, 5},
        {{6, 'j'}, 6},
        {{6, 'k'}, 7},
        {{6, 'w'}, 0},
        {{7, 'j'}, 1},
        {{7, 'w'}, 5},
    },
    0,
    {1, 2, 3, 4, 5, 6, 7},
};
MISNFA in11 = {
    {0, 1, 2, 3, 4, 5, 6},
    {'b', 'i', 'r'},
    {
        {{0, 'b'}, {2}},
        {{0, 'i'}, {1, 2, 4}},
        {{0, 'r'}, {0}},
        {{1, 'b'}, {1, 2, 5}},
        {{1, 'i'}, {0}},
        {{1, 'r'}, {1, 6}},
        {{2, 'b'}, {2, 4}},
        {{2, 'r'}, {1, 2}},
        {{3, 'b'}, {2}},
        {{3, 'i'}, {2}},
        {{3, 'r'}, {1, 3, 4}},
        {{4, 'r'}, {6}},
        {{5, 'b'}, {2}},
        {{5, 'i'}, {1, 2, 4}},
        {{5, 'r'}, {0}},
        {{6, 'r'}, {6}},
    },
    {1},
    {0, 1, 2, 5},
};
DFA out11 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
    {'b', 'i', 'r'},
    {
        {{0, 'b'}, 1},
        {{0, 'i'}, 2},
        {{0, 'r'}, 3},
        {{1, 'b'}, 4},
        {{1, 'i'}, 5},
        {{1, 'r'}, 6},
        {{2, 'b'}, 7},
        {{2, 'i'}, 8},
        {{2, 'r'}, 2},
        {{3, 'b'}, 1},
        {{3, 'i'}, 2},
        {{3, 'r'}, 3},
        {{4, 'b'}, 4},
        {{4, 'i'}, 5},
        {{4, 'r'}, 6},
        {{5, 'b'}, 4},
        {{5, 'i'}, 5},
        {{5, 'r'}, 6},
        {{6, 'b'}, 4},
        {{6, 'i'}, 5},
        {{6, 'r'}, 6},
        {{7, 'b'}, 9},
        {{7, 'r'}, 10},
        {{8, 'b'}, 4},
        {{8, 'i'}, 2},
        {{8, 'r'}, 11},
        {{9, 'b'}, 9},
        {{9, 'r'}, 11},
        {{10, 'b'}, 4},
        {{10, 'i'}, 2},
        {{10, 'r'}, 11},
        {{11, 'b'}, 4},
        {{11, 'i'}, 2},
        {{11, 'r'}, 11},
    },
    0,
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
};
MISNFA in12 = {
    {0, 1, 2, 3, 4, 5, 6},
    {'l', 'q', 't'},
    {
        {{0, 'l'}, {2, 4, 5}},
        {{0, 'q'}, {2}},
        {{0, 't'}, {1}},
        {{1, 'l'}, {0, 2}},
        {{1, 'q'}, {1, 4}},
        {{1, 't'}, {0, 2}},
        {{2, 'l'}, {5}},
        {{2, 'q'}, {0, 4}},
        {{2, 't'}, {0}},
        {{3, 'l'}, {3, 4}},
        {{3, 'q'}, {0}},
        {{3, 't'}, {0, 2}},
        {{4, 't'}, {4}},
        {{5, 'l'}, {0, 2}},
        {{5, 'q'}, {4, 5}},
        {{5, 't'}, {0, 2}},
        {{6, 'l'}, {4, 6}},
        {{6, 'q'}, {0}},
        {{6, 't'}, {0, 2}},
    },
    {2, 4},
    {0, 1, 3, 5, 6},
};
DFA out12 = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18},
    {'l', 'q', 't'},
    {
        {{0, 'l'}, 1},
        {{0, 'q'}, 2},
        {{0, 't'}, 2},
        {{1, 'l'}, 3},
        {{1, 'q'}, 4},
        {{1, 't'}, 3},
        {{2, 'l'}, 5},
        {{2, 'q'}, 6},
        {{2, 't'}, 7},
        {{3, 'l'}, 5},
        {{3, 'q'}, 8},
        {{3, 't'}, 9},
        {{4, 'l'}, 3},
        {{4, 'q'}, 4},
        {{4, 't'}, 8},
        {{5, 'l'}, 10},
        {{5, 'q'}, 11},
        {{5, 't'}, 8},
        {{6, 'l'}, 1},
        {{6, 'q'}, 2},
        {{6, 't'}, 12},
        {{7, 'l'}, 3},
        {{7, 'q'}, 7},
        {{7, 't'}, 8},
        {{8, 'l'}, 5},
        {{8, 'q'}, 8},
        {{8, 't'}, 13},
        {{9, 'l'}, 14},
        {{9, 'q'}, 15},
        {{9, 't'}, 16},
        {{10, 'l'}, 14},
        {{10, 'q'}, 14},
        {{10, 't'}, 16},
        {{11, 'l'}, 14},
        {{11, 'q'}, 5},
        {{11, 't'}, 17},
        {{12, 'l'}, 5},
        {{12, 'q'}, 6},
        {{12, 't'}, 18},
        {{13, 'l'}, 14},
        {{13, 'q'}, 15},
        {{13, 't'}, 17},
        {{14, 'l'}, 14},
        {{14, 'q'}, 14},
        {{14, 't'}, 17},
        {{15, 'l'}, 10},
        {{15, 'q'}, 13},
        {{15, 't'}, 8},
        {{16, 'l'}, 14},
        {{16, 'q'}, 17},
        {{16, 't'}, 16},
        {{17, 'l'}, 14},
        {{17, 'q'}, 17},
        {{17, 't'}, 17},
        {{18, 'l'}, 3},
        {{18, 'q'}, 7},
        {{18, 't'}, 3},
    },
    0,
    {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18},
};
MISNFA in13 = {
    {0, 1, 2, 3, 4, 5, 6},
    {'o', 'r'},
    {
        {{0, 'o'}, {0, 1, 4}},
        {{0, 'r'}, {5}},
        {{1, 'o'}, {4}},
        {{1, 'r'}, {2}},
        {{2, 'o'}, {0, 1}},
        {{2, 'r'}, {0, 5}},
        {{3, 'r'}, {2, 5}},
        {{5, 'o'}, {0, 1}},
        {{5, 'r'}, {0, 5}},
        {{6, 'r'}, {2}},
    },
    {2, 5},
    {0},
};
DFA out13 = {
    {0, 1, 2, 3},
    {'o', 'r'},
    {
        {{0, 'o'}, 1},
        {{0, 'r'}, 2},
        {{1, 'o'}, 3},
        {{1, 'r'}, 0},
        {{2, 'o'}, 3},
        {{2, 'r'}, 2},
        {{3, 'o'}, 3},
        {{3, 'r'}, 0},
    },
    0,
    {1, 2, 3},
};

int main()
{
    determinize ( in ) . print("");
    assert(determinize(in1) == out1);
    assert(determinize(in2) == out2);
    assert(determinize(in3) == out3);
    assert(determinize(in4) == out4);
    assert(determinize(in5) == out5);
    assert(determinize(in6) == out6);
    assert(determinize(in7) == out7);
    assert(determinize(in8) == out8);
    assert(determinize(in9) == out9);
    assert(determinize(in10) == out10);
    assert(determinize(in11) == out11);
    assert(determinize(in12) == out12);
    assert(determinize(in13) == out13);

    return 0;
}
#endif
