// I = { 0, 0 }
// G = { 4, 4 }
// S = { x, y }; x (= [0, 5] y (= [0, 3]
/*
a = {
    "Naplnit"
    { x, y } | if x != 5 -> { 5, y }
    { x, y } | if y != 3 -> { x, 3 }

    "Vylít"
    { x, y } | if x != 0 -> { 0, y }
    { x, y } | if y != 0 -> { x, 0 }

    "Přelít"
    { x, y } | if x != 0 && y != 3 -> { x - pour, y + pour }
    { x, y } | if x != 5 && y != 0 -> { x + pour, y - pour }
}
*/

#include <iostream>
#include <deque>
#include <vector>

class State
{
public:
    // Constructor,..
    State ( int x, int y )
    {
        bucket5 = x;
        bucket3 = y;
    }
    ~State () {}

    // Variables

    // Methods

    State fill5 () const
    {
        return State ( 5, bucket3 );
    };

    State fill3 () const
    {
        return State ( bucket5, 3 );
    };

    State spill5 () const
    {
        return State ( 0, bucket3 );
    };

    State spill3 () const
    {
        return State ( bucket5, 0 );
    };

    State pourOver5 () const
    {
        int tmp = getPour5 ();
        return State ( bucket5 - tmp, bucket3 + tmp );
    };

    State pourOver3 () const
    {
        int tmp = getPour3 ();
        return State ( bucket5 + tmp, bucket3 - tmp );
    };

    bool full5 () const
    {
        if ( bucket5 == 5)
            return true;
        return false;
    };

    bool full3 () const
    {
        if ( bucket3 == 3)
            return true;
        return false;
    };

    bool empty5 () const
    {
        if ( bucket5 == 0)
            return true;
        return false;
    };

    bool empty3 () const
    {
        if ( bucket3 == 0)
            return true;
        return false;
    };

    friend bool operator == ( State l, State r );
    friend std::ostream & operator << ( std::ostream & ost, State c );

private:
    // Variables
    int bucket5;
    int bucket3;

    // Methods
    int getPour5 () const
    {
        int capacity3 = 3 - bucket3;
        if ( bucket5 > capacity3 )
            return capacity3;
        return bucket5; 
    };

    int getPour3 () const
    {
        int capacity5 = 5 - bucket5;
        if ( bucket3 > capacity5 )
            return capacity5;
        return bucket3;
    };
};

bool operator == ( State l, State r )
{
    if ( l . bucket5 == r . bucket5
        && l . bucket3 == r . bucket3 )
        return true;
    if ( l . bucket5 == r . bucket5 && l . bucket5 == 4 )
        return true;
    return false;
}

std::ostream & operator << ( std::ostream & ost, State c )
{
    ost << "bucket5: " << c . bucket5 << ", bucket3: " << c . bucket3 << std::endl;
    return ost;
}

class DieHard
{
public:
    // Constructor,..
    DieHard ()
    : I(0, 0), G(4, 0)
    {
        queue . push_front ( I );
    }
    ~DieHard () {}

    // Variables

    // Methods
    bool start ()
    {
        while ( ! queue . empty() )
        {
            State curr = queue . front();
            std::cout << curr;

            if ( curr == G )
                return true;
            if ( ! curr . full5() )
                queue . push_back ( curr . fill5() );

            if ( ! curr . full3() )            
                queue . push_back ( curr . fill3() );

            if ( ! curr . empty5() )
                queue . push_back ( curr . spill5() );

            if ( ! curr . empty3() )
                queue . push_back ( curr . spill3() );

            if ( ! curr . empty5() && ! curr . full3() )
                queue . push_back ( curr . pourOver5() );

            if ( ! curr . full5() && ! curr . empty3() )
                queue . push_back ( curr . pourOver3() );

            queue . pop_front ();
        }
        return false;
    };

private:
    // Variables
    State I; // start
    State G; // end
    std::deque <State> queue;
    std::vector <State> prev;

    // Methods

};

int main ( void )
{
    DieHard game;
    if ( ! game . start () ) 
        std::cout << "ERROR!" << std::endl;
    std::cout << "OK!" << std::endl;
    return 0;
}