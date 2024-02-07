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

int error ( INPUT in1 )
{
    free ( in1.arr );
    printf ( "Nespravny vstup.\n" );
    return 0;
}

void appendInput ( INPUT * input )
{
    if ( input->capacity == input->len )
    {
        input->capacity *= 2;
        input->arr = ( int * ) realloc ( input->arr, input->capacity * sizeof ( * input->arr ) );
    }
}

void initInput ( INPUT * input )
{
    input->capacity = 100;
    input->len = 0;
    input->arr = ( int * ) malloc ( input->capacity * sizeof ( * input->arr ) );
}

void insertNum ( INPUT * input, int x )
{
    appendInput ( input );
    input->arr[input->len] = x;
    input->len ++;
}

int readMince ( INPUT * input )
{
    int num;
    while ( ! feof ( stdin ) )
    {
        if ( scanf ( " %d ", & num ) != 1
            || num < 0 )
            return 0;
        if ( num == 0 )
            return 1;
        insertNum ( input, num );
    }
    return 0;
}

int readCastka ( int * input )
{
    if ( scanf ( " %d ", input ) != 1
        || * input < 0 )
        return 0;
    return 1;
}

int intCmp ( const void * a, const void * b )
{
    int * x = ( int * ) a;
    int * y = ( int * ) b;
    return ( * y < * x ) - ( * x < * y );
}

int getResult ( INPUT input, int x )
{
    if ( input.arr[0] > x )
        return 0;

    for ( int i = 0; i < input . len; i ++ )
    {
        if ( input . arr [i] > x )
            continue;
        if ( input . arr [i] % x == 0 ) // exactly value
            return input . arr [i] / x;
    }

    int best = 0;
    int tmp_res;
    for ( int i = input . len - 1; i >= 0; i -- )
    {
        if ( input . arr [i] > x )
            continue;
        tmp_res = getResult ( input, x - input . arr [i] );
        if ( tmp_res && ( tmp_res < best || ! best ) )
            best = tmp_res;
    }
    return 1 + best;
}

int main ( void )
{
//  -- Initialize --
    INPUT mince;
    initInput ( & mince );
    int val;
    int result;
//  -- Read Input --
    printf ( "Mince:\n" );
    if ( ! readMince ( & mince ) )
        return error ( mince );
    qsort ( mince.arr, mince.len, sizeof ( * mince.arr ), intCmp );
    //for ( int i = 0; i < mince . len; i ++ )
    //    printf ( "%d ", mince . arr [i] );
    //printf ("\n" );

    printf ( "Castky:\n" );
//  -- Program --
    while ( ! feof ( stdin ) )
    {
        if ( ! readCastka ( & val ) )
            return error ( mince );
        result = getResult ( mince, val );
        if ( ! result )
            printf ( "= nema reseni\n" );
        else
            printf ( "= %d\n", result );
    }
//  -- Finish --
    free ( mince.arr );
    return 0;
}