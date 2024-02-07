#include <stdio.h>

int error ()
{
    printf( "Nespravny vstup.\n" );
    return 0;
}

int getDay ( int x )
{
    switch (x)
    {
        case 0: return 5;
        case 1: return 6;
        case 2: return 0;
        case 3: return 1;
        case 4: return 2;
        case 5: return 3;
        case 6: return 4;   
    }
    return 0;
}

int isLeap ( int y )
{
    if ( ( y % 4 == 0 && y % 100 != 0) || y % 400 == 0 )
        return 1;
    return 0;
}

int main ( void )
{
    int d, m, y;
    if ( scanf( "%d.%d.%d", &d, &m, &y ) != 3
        || d < 1 || m < 1 || y < 1600
        || d > 31 || m > 12 )
        return error();
    
    int count = 0;
    int i = 1600;
    while ( i < y )
    {
        count += 365 + isLeap(i);
        count %= 7;
        i++;
    }

    int x = 31;
    i = 1;
    while ( i < m )
    {
        count += x;
        x = i == 2 ? 29 : ( x < 8 ? (x % 2 == 0 ? 30 : 31) : (x % 2 == 0 ? 31 : 30) );
        i++;
    }
    count += d - 1;
    printf( "Count = %d.\n", count );
    printf( "%d.%d.%d byl %d.\n", d, m, y, getDay( count % 7 ) );
    
    return 0;
}