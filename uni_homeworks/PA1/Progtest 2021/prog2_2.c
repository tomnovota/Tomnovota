#include <stdio.h>
#include <malloc.h>
#include <assert.h>

int error () 
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

int getNums ( int * a, int * b )
{
    int c, d;
    if ( scanf( " %d %d", &c, &d ) != 2
        || c < 0 || d <= 0) 
        return error();
    *a = c;
    *b = d;
    return 1;
}

int getFirst( int a, int b, int * i, int * start )
{
    while ( (a/b) >= 10 )
    {
        a /= 10;
        (*i)++;
    }
    int x = a;
    while ( (x/10) >= 1 )
    {
        x /= 10;
        (*start)++; // o jedno místo dál od první číslice příkladu
    }
    return a;
}

int getNext( int a, int pos )
{
    for ( int i = 0; i < pos; i++ )
        a /= 10;
    return ( a - ((a/10)*10) );
}

void mezera( int x, int trans )
{
    int deg = 1;
    while ( (x/10) >= 1 )
    {
        deg++;
        x /= 10;
    }
    for ( int i = 0; i < ( trans - deg ); i++ )
    {
        printf( " " );
    }
}

int calc ( int a, int b )
{
    int count = 0, start = 1, y;
    int x = getFirst( a, b, &count, &start );
    for ( int i = 0; i <= count; i++ )
    {
        y = -(x/b)*b;
        if ( y == 0 )
            mezera( -y, start + i + 1 );
        else
            mezera( -y, start + i );
        printf ( "%d\n", y );
        if ( i < count )
        {
            x = ( (x+y) * 10) + getNext( a, count - i - 1 );
            mezera( x, start + i + 2 );
            printf ( "%d\n", x );
        }
        else
        {
            mezera( x+y, start + i + 1 );
            printf( "%d zbytek\n", x+y );
        }
    }
    return 0;
}

int main (void)
{
    int a, b;
    printf( "Zadejte dve cisla:\n" );
    if ( ! getNums(&a, &b) ) return 0;
    printf( " %d : %d = %d\n", a, b, a/b );
    return calc(a, b);
}