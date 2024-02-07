#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int * arr;
    int capacity;
    int length;
} ARRAY;

int error ( ARRAY arr )
{
    free ( arr.arr );
    printf ( "Nespravny vstup.\n" );
    return 0;
}

void appendArray ( ARRAY * input )
{
    if ( input->capacity == input->length )
    {
        input->capacity *= 2;
        input->arr = ( int * ) realloc ( input->arr, input->capacity * sizeof ( *(input->arr) ) );
    }
}

int readInput ( ARRAY * input )
{
    char sym;
    int val;
    if ( scanf ( " %c", &sym ) != 1
        || sym != '{' )
        return 0;

    while ( scanf ( " %d %c ", &val, &sym ) == 2
        && (sym == ',' || sym == '}') )
    {
        appendArray ( input );
        input->arr [input->length] = val;
        (input->length)++;
        if ( sym == '}' )
            return 1;
    }
    return 0;
}

int readInterval ( int * from, int * to )
{
    char sym;
    if ( scanf ( " %c", &sym ) != 1
        || ( sym != '<' && sym != '(' ) )
        return 0;
    if ( scanf ( " %d;%d", from, to ) != 2 )
        return 0;
    if ( sym == '(' )
        (*from)++;
    if ( scanf ( " %c ", &sym ) != 1
        || ( sym != '>' && sym != ')' ) )
        return 0;
    if ( sym == ')' )
        (*to)--;
    return 1;
}

int getResult ( ARRAY input, int from, int to )
{
    int result = 0;
    int index = 0;
    while ( index < input.length && from > input.arr[index] )
        index++;
    if ( index == input.length )
        return 0;

    for ( int i = index; i < input.length && to >= input.arr[i]; i++ )
        result++;

    return result;
}

int myCmp ( const void * a, const void * b )
{
    int * x = ( int * ) a;
    int * y = ( int * ) b;
    return ( *y < *x ) - ( *x < *y );
}

int main ( void )
{
    ARRAY input;
    input.capacity = 100;
    input.length = 0;
    input.arr = ( int * ) malloc ( input.capacity * sizeof ( *(input.arr) ) );

    printf ( "Pocty bodu:\n" );
    if ( ! readInput ( &input ) )
        return error( input );
    qsort ( input.arr, input.length, sizeof ( *(input.arr) ), myCmp );
    printf ( "Intervaly:\n" );
    int count, from, to;
    while ( ! feof ( stdin ) )
    {
        if ( ! readInterval ( &from, &to ) )
            return error( input );
        count = getResult ( input, from, to );
        printf ( " -> %d\n", count );
    }
    free ( input.arr );
    return 0;
}