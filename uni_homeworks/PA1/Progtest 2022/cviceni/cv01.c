#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int error()
{
    printf( "Nespravny vstup.\n" );
    return 0;
}

int nacteni( int * n1, int * b1, int * b2 )
{
    int x;
    printf( "Zadejte cislo:\n" );
    if ( scanf( " %d", &x ) != 1
        || x < 0
    ) return error();
    *n1 = x;
    printf( "Zadejte soustavu, ve které je zadané číslo:\n" );
    if ( scanf( " %d", &x ) != 1
        || x < 2 || x > 36
    ) return error();
    *b1 = x;
    printf( "Zadejte soustavu, do které chcete zadané číslo převést:\n" );
    if ( scanf( " %d", &x ) != 1
        || x < 2 || x > 36
    ) return error();
    *b2 = x;
    return 1;
}

char getSym( int x )
{
    int y = 48, z = 7;
    if ( x > 9 )
    {
        return x + y + z;
    }
    return x + y;
}

int printRes( char * n, int size )
{
    for ( int i = size; i >= 0; i-- )
    {
        printf( "%c", n[i] );
    }
    printf( "\n" );
    return 0;
}

int zbytek( int x )
{
    return x - (x / 10 * 10);
}

int getObraz10 ( int n1, int b1, int b2 )
{
    int mocnina = 1, x10 = 0;
    while ( n1 / 10 > 0 )
    {
        x10 += zbytek( n1 ) * mocnina;
        mocnina *= b1;
        n1 /= 10;
    }
    return ( x10 + (n1 * mocnina) );
}

char * getRes ( int x10, int b, int * size )
{
    int zbytek, el_count = 0;
    char * res = (char *) malloc ( sizeof(*res) );
    while ( x10 / b > 0 )
    {
        zbytek = x10 % b;
        x10 /= b;
        res[ el_count ] = getSym (zbytek);
        el_count++;
        if ( el_count >= *size )
        {
            *size *= 2;
            res = (char *) realloc ( res, *size * sizeof(*res) );
        }
    }
    res[ el_count ] = getSym( x10 );
    *size = el_count;
    return res;
}

int main ( void )
{
    int n1, b1, b2;
    nacteni( &n1, &b1, &b2 );
    int x10 = getObraz10( n1, b1, b2 );
    printf( "Obraz čísla %d v soustavě %d do soustavy 10 je: %d\n", n1, b1, x10 );
    printf( "Obraz čísla %d v soustavě %d do soustavy %d je: ", n1, b1, b2 );
    int size = 1;
    char * n2 = getRes( x10, b2, &size );
    printRes( n2, size );
    free( n2 );
    return 0;
}