#include <stdio.h>

int error()
{
    printf( "Spatny vstup.\n" );
    return 0;
}

int f1 ()
{
    int a, b, c, max, min, med;
    if ( scanf( "%d %d %d", &a, &b, &c ) != 3 )
        return error();
    max = a > b ? (a > c ? a : c) : ( b > c ? b : c );
    min = a < b ? (a < c ? a : c) : ( b < c ? b : c );
    med = a < b ? (a > c ? a : (b < c ? b : c) ) : ( b > c ? b : (a < c ? a : c) );
    printf ( "Max = %d, Med = %d, Min = %d\n", max, med, min );
    return 0;
}

int f2 ()
{
    int a, b, c, d;
    if ( scanf("%d:%d", &a, &b) != 2
    || a < 0 || b < 0 || a > 23 || b > 59 )
        return error();
    if ( scanf("%d:%d", &c, &d) != 2
    || c < 0 || d < 0 || c > 23 || d > 59 )
        return error();
    if ( b > d )
    {
        d += 60;
        a += 1;
    }
    if ( a > c )
    {
        c += 24;
    }
    printf( "Interval je dlouhy %2d:%02d.\n", c-a, d-b );
    return 0;
}

int main (void)
{
    return f2();
}