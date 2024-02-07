#include <stdio.h>
#include <math.h>
//#include <stdlib.h>
#include <limits.h>

int error( void )
{
    printf( "Nespravny vstup.\n" );
    return 0;
}

int sumOfDiv ( int x )
{
    int sum = 1;
    int sq_x = sqrt(x);
    for ( int i = 2; i <= sq_x; i++ )
    {
        if( x % i == 0 )
        {
            if( i == x / i )
                sum += i;
            else
                sum += i + (x / i);
        }
    }
    return sum;
}

char isPrime ( int x )
{
    if( x < 2 )
        return 0;
    for ( int i = 2; i < x; i++ )
    {
        if( x % i == 0 )
            return 0;
    }
    return 1;
}

void getArr ( char arr[], int size )
{
    int sq_max = sqrt(INT_MAX);
    for ( int i = 2; i < size; i++ )
        arr[i] = 1;
    arr[0] = 0;
    arr[1] = 0;

    for( int i = 2; i < size; i++ )
    {
        if ( arr[i] )
        {
            if ( i <= sq_max )
                for ( int j = i * i; j < size; j += i)
                    arr[j] = 0;
        }
    }
}

int getNums ( int from, int to, char q )
{
    char arr[2000000];
    int size = 2000000;
    getArr( arr, size );
    int count = 0;
    for ( int i = from; i <= to; i++ )
    {
        int tmp = sumOfDiv( i );
        if( tmp >= size && isPrime(tmp) )
        {
            count++;
            if ( q )
                printf("%d\n", i);
        }
        if ( arr[tmp] )
        {
            count++;
            if ( q )
                printf("%d\n", i);
        }
    }
    return count;
}

int main ( void )
{
    int lo, hi, count;
    char sym;
    printf("Intervaly:\n");
    while ( scanf( " %c %d %d", &sym, &lo, &hi ) == 3
            && (sym == '#' || sym == '?') 
            && lo > 0 && hi >= lo )
    {
        char q = sym == '?' ? 1 : 0;
        count = getNums( lo, hi, q );
        printf("Celkem: %d\n", count);
    }
    if ( ! feof(stdin) )
        return error();
    return 0;
}