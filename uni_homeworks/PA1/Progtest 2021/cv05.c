#include<stdio.h>
#include<assert.h>

int error()
{
    printf( "Nespravny vstup.\n" );
    return 0;
}

int getNum( int * num )
{
    int x;
    printf( "Rozsah:\n" );
    if ( scanf( " %d", &x ) != 1
        || x < 1 )
        return 0;
    *num = x;
    return 1;
}

int getSize( int num )
{
    int x = 1;
    while ( (num / 10) >= 1 )
        {
            x++;
            num /= 10;
        }
    return x;
}

void space( int count )
{
    for ( int i = 0; i < count; i++ )
        printf( " " );
}

void line( int num, int size )
{
    for ( int i = 1; i < size; i++ )
        printf( "-" );
    printf( "+" );
    for ( int i = 0; i < num; i++ )
    {
        for ( int j = 0; j < size; j++ )
            printf("-");
    }
    printf( "\n" );
}

void intro( int num, int size )
{
    space( size - 1 );
    printf( "|" );
    for ( int i = 0; i < num; i++ )
    {
        space( size - getSize(num-i) );
        printf( "%d", (num-i) );
    }
    printf( "\n" );
    line( num, size );
}

void counts( int num, int size, int iter )
{
    int x;
    for ( int i = 0; i < (num - iter + 1 ); i++ )
    {
        x = iter * (num - i);
        space( size - getSize(x) );
        printf( "%d", x );
    }
}

int makeTable( int num, int size )
{
    intro( num, size );
    for ( int i = 1; i <= num ; i++ )
    {
        space( size - getSize(i) - 1 );
        printf( "%d|", i );
        counts( num, size, i );
        printf( "\n" );
    }
    return 0;
}

int main ( void )
{
    int num;
    if ( ! getNum( &num ) )
        return error();
    return makeTable( num, getSize( num*num ) + 1 );
}