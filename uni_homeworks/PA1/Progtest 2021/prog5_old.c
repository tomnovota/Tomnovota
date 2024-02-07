#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int * arr;
    int capacity;
    int len;
} INPUT;

void printInput ( INPUT input )
{
    for ( int i = 0; i < input . len; i ++ )
        printf ( "%d ", input . arr [i] );
    printf ( "\n" );
}

void deleteInput ( INPUT input )
{
    free ( input . arr );
}

void appendInput ( INPUT * input, int x )
{
    if ( input -> len == input -> capacity )
    {
        input -> capacity *= 2;
        input -> arr = ( int * ) realloc ( input -> arr, input -> capacity * sizeof ( int ) );
    }
    input -> arr [ input -> len ] = x;
    input -> len ++;
}

void initInput ( INPUT * input )
{
    input -> len = 0;
    input -> capacity = 100;
    input -> arr = ( int * ) malloc ( input -> capacity * sizeof ( int ) );
}

int error ( void )
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

int readInput ( INPUT * input )
{
    int num;
    char sym;
    printf ( "Vstupni posloupnost:\n" );
    
    while ( scanf ( "%d %c", & num, & sym ) == 2
        && ( sym == ',' || sym == '?' ) )
    {
        if ( sym == '?' && ! input -> len )
        {
            deleteInput ( * input );
            return 0;
        }
        appendInput ( input, num );
        if ( sym == '?' )
            return 1;
    }
    deleteInput ( * input );
    return 0;
}

int getResult ( INPUT input, int x )
{
    for ( int i = 0; i < input . len; i ++ )
    {
        for ( int j = i + 1; j < input . len; j ++ )
        {
            if ( input . arr [i] + input . arr [j] == x )
                return 1;
        }
    }
    return 0;
}

int main ( void )
{
//  -- INIT --
    INPUT input;
    initInput ( & input );

//  -- INPUT --
    if ( ! readInput ( & input ) )
        return error ();

//  -- PROGRAM --
    int num;
    printf ( "Testovane hodnoty:\n" );
    while ( scanf ( "%d", & num ) == 1 )
    {
        if ( getResult ( input, num ) )
            printf ( "> Soucet %d lze dosahnout.\n", num );
        else
            printf ( "> Soucet %d nelze dosahnout.\n", num );
    }

//  -- END --
    deleteInput ( input );
    if ( ! feof ( stdin ) )
        return error ();
    return 0;
}
