#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} TARGET;

typedef struct {
    double x;
    double y;
    double radius;
} RADAR;

typedef struct {
    TARGET * arr;
    int capacity;
    int len;
} INPUT_T;

typedef struct {
    RADAR * arr;
    int capacity;
    int len;
} INPUT_R;

int error ( INPUT_R R, INPUT_T T )
{
    free ( R.arr );
    free ( T.arr );
    printf ( "Nespravny vstup.\n" );
    return 0;
}

void initStruct ( int * capacity, int * len )
{
    * capacity = 100;
    * len = 0;
}

int readType ( char * type, double * x, double * y )
{
    if ( scanf ( " %c %lf %lf ", type, x, y ) != 3
        || ( * type != 'R' && * type != 'T') )
        return 0;
    return 1;
}

void appendRadar ( INPUT_R * input )
{
    if ( input->capacity == input->len )
    {
        input->capacity *= 2;
        input->arr = ( RADAR * ) realloc ( input->arr, input->capacity * sizeof ( * input->arr ) );
    }
}

void saveRadar ( INPUT_R * input, double x, double y, double rad )
{
    appendRadar ( input );
    input->arr[input->len] . x = x;
    input->arr[input->len] . y = y;
    input->arr[input->len] . radius = rad;
    (input->len) ++;
}

void appendTarget ( INPUT_T * input )
{
    if ( input->capacity == input->len )
    {
        input->capacity *= 2;
        input->arr = ( TARGET * ) realloc ( input->arr, input->capacity * sizeof ( * input->arr ) );
    }
}

void saveTarget ( INPUT_T * input, double x, double y )
{
    appendTarget ( input );
    input->arr[input->len] . x = x;
    input->arr[input->len] . y = y;
    (input->len) ++;
}

double distance ( RADAR r, TARGET t )
{
    double x = r.x - t.x;
    double y = r.y - t.y;
    return sqrt ( x * x + y * y );
}

int getResult ( RADAR radar, INPUT_T targets )
{
    int count = 0;
    for ( int i = 0; i < targets.len; i++ )
    {
        if ( distance ( radar, targets.arr[i] ) <= radar.radius )
            count ++;
    }
    return count;
}

int main ( void )
{
//    -- INITIALIZATION --
    INPUT_R input_R;
    INPUT_T input_T;
    initStruct ( & input_R.capacity, & input_R.len );
    initStruct ( & input_T.capacity, & input_T.len );
    input_R.arr = ( RADAR * ) malloc ( input_R.capacity * sizeof ( * input_R.arr ) );
    input_T.arr = ( TARGET * ) malloc ( input_T.capacity * sizeof ( * input_T.arr ) );
    char type;
    double x, y, rad;
    int result;
//    -- INPUT --
    printf ( "Seznam cilu a radaru:\n" );
    while ( ! feof ( stdin ) )
    {
        if ( ! readType ( & type, & x, & y ) )
            return error ( input_R, input_T );
        if ( type == 'R' ) // radar
        {
            if ( scanf ( " %lf ", & rad ) != 1
                || rad <= 0 )
                return error ( input_R, input_T );
            saveRadar ( & input_R, x, y, rad );
        } else { // target
            saveTarget ( & input_T, x, y );
        }
    }
//    -- RESULT --
    printf ( "Pokryti:\n" );
    for ( int i = 0; i < input_R.len; i++ )
    {
        result = getResult ( input_R.arr[i], input_T );
        printf ( "Radar #%d: %d\n", i, result );
    }
//    -- END --
    free ( input_R.arr );
    free ( input_T.arr );
    return 0;
}