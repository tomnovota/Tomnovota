#include <stdio.h>

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int input( int * x )
{
    if ( scanf( "%d", x ) != 1
        || *x < 1 )
        return 0;
    return 1;
}

void line ( int a, int b )
{
    printf("+");
    for (int i = 0; i < (a * b); i++)
        printf("-");
    printf("+\n");
}

void field (int a, int b)
{
    char sym [2] = {' ', 'X'};
    int odd1 = 0, odd2 = 0;
    for ( int i = 0; i < a * b; i++ )
    {
        odd1 = i % b == 0 ? (! odd1) : odd1;
        odd2 = odd1;
        printf( "|" );
        for ( int j = 0; j < a; j++ )
        {
            odd2 = ! odd2;
            for ( int k = 0; k < b; k++ )
            {
                printf( "%c",sym[odd2] );
            }
        }
        printf( "|\n" );
    }
}

int main ( void )
{
    int a, b;
    printf("Zadejte pocet poli:\n");
    if ( ! input( &a ))
        return error();
    printf("Zadejte velikost pole:\n");
    if ( ! input( &b ))
        return error();
    line(a, b);
    field(a, b);
    line(a, b);
    return 0;
}