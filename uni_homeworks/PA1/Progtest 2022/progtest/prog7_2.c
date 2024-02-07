#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int error ()
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

int readInput ( int * count, int * len )
{
    printf ( "Pocet sirek:\n" );
    if ( scanf ( " %d", count ) != 1
        || * count < 1 )
        return 0;
    printf ( "Velikost skoku:\n" );
    if ( scanf ( " %d", len ) != 1
        || * len < 0 )
        return 0;
    return 1;
}

void play ( char * arr, int count, int len, int * total )
{
     
}

int main ( void )
{
    int m_count, j_len, total = 0;
    char * arr;
    if ( ! readInput ( & m_count, & j_len ) )
        return error ();
    arr = ( char * ) malloc ( m_count * sizeof ( * arr ) );
    for ( int i = 0; i < m_count; i++ )
        arr [ i ] = 1;
    if ( m_count % 2 == 0 )
        play ( arr, m_count, j_len, &total );
    printf ( "Celkem reseni: %d\n", total );
    free ( arr );
    return 0;
}