#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char ** arr;
    int rows;
    int columns;
    int rows_size;
} MATRIX;

int error ( void )
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

void freeInput ( MATRIX input )
{
    for ( int i = 0; i < input . rows; i++ )
        free ( input . arr [i] );
    free ( input .arr );
}

void printInput ( MATRIX input )
{
    for ( int i = 0; i < input . rows; i ++ )
        printf ( "%s\n", input . arr [i] );
}

void initInput ( MATRIX * input )
{
    input -> columns = 0;
    input -> rows = 0;
    input -> rows_size = 100;
    input -> arr = ( char ** ) malloc ( 100 * sizeof ( char * ) );
}

void appendInput ( MATRIX * input )
{
    if ( input -> rows == input -> rows_size )
    {
        input -> rows_size *= 2;
        input -> arr = ( char ** ) realloc ( input -> arr, input -> rows_size * sizeof ( char * ) );
    }
}

int readInput ( MATRIX * input )
{
    int items;
    char * line = NULL;
    size_t nr = 0;

    printf ( "Hlavolam:\n" );
    while ( ( items = getline ( & line, & nr, stdin ) ) > 0 )    
    {
        if ( ! input -> columns ) // first
            input -> columns = items;
        else if ( input -> columns != items ) // not square
        {
            free ( line );
            freeInput ( * input );
            return 0;
        }
        input -> arr [ input -> rows ] = strndup ( line, -- items ); // without newline
        input-> rows ++;
        // prepare for next loop
        free ( line );
        line = NULL;
        nr = 0;
    }
    free ( line );
    input -> columns --; // without newline
    if ( ! input -> columns || input -> columns != input -> rows ) // no input or not square
    {
        freeInput ( * input );
        return 0;
    }
    return 1;
}

int main ( void )
{
//  -- INITIALIZATION --
    MATRIX input;
    initInput ( & input );

//  -- READ INPUT --
    if ( ! readInput ( & input ) )
        return error ();

//  -- PROGRAM --
    
    freeInput ( input );
    return 0;
}