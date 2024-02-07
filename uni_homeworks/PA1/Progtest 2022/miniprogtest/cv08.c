#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int error ( void )
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

int deleteArr ( char *** arr, size_t ma )
{
    for ( size_t i = 0; i < ma; i++ )
        free ( (*arr)[i] );
    free ( *arr );
    return 0;
}

int readInput ( char *** arr, size_t * ma, int * na )
{
    size_t arr_size = 100;
    *arr = ( char ** ) malloc ( arr_size * sizeof(**arr) );

    (*arr)[*ma] = NULL;
    size_t tmp_na = 0;
    int items;
    while ( items = getline( &(*arr)[*ma], &tmp_na, stdin ), items != -1 )
    {
        if ( items == 1 )
        {
            return deleteArr ( arr, *ma+1 );
        }
        if ( *ma == arr_size )
        {
            arr_size *= 2;
            *arr = ( char ** ) realloc ( *arr, arr_size * sizeof(**arr) );
        }
        for ( int i = 0; i < items && (*arr)[*ma][i] != '\n'; i++ )
        {
            if ( (*arr)[*ma][i] != '.' && (*arr)[*ma][i] != '*' && (*arr)[*ma][i] != '\n' )
                return deleteArr ( arr, *ma+1 );
        }
        if ( *na && items != *na )
            return deleteArr ( arr, *ma+1 );
        *na = items;

        (*ma)++;
        (*arr)[*ma] = NULL;
    }
    free((*arr)[*ma]);
    if ( ! *na )
        return deleteArr ( arr, *ma );
    return 1;
}

char getResult ( char ** arr, size_t ma, size_t na, size_t pos1, int pos2 )
{
    int tmp_count = 0;   
    if ( pos1 != 0 && pos2 != 0 && arr[pos1-1][pos2-1] == '*' )
        tmp_count++;
    if ( pos1 != 0 && arr[pos1-1][pos2] == '*' )
        tmp_count++;
    if ( pos1 != 0 && arr[pos1-1][pos2+1] == '*' )
        tmp_count++;
    if ( pos2 != 0 && arr[pos1][pos2-1] == '*' )
        tmp_count++;
    if ( arr[pos1][pos2+1] == '*' )
        tmp_count++;
    if ( pos1 + 1 != ma && pos2 != 0 && arr[pos1+1][pos2-1] == '*' )
        tmp_count++;
    if ( pos1 + 1 != ma && arr[pos1+1][pos2] == '*' )
        tmp_count++;
    if ( pos1 + 1 != ma && arr[pos1+1][pos2+1] == '*' )
        tmp_count++;
    return tmp_count == 0 ? '.' : 48 + tmp_count;
}

int main ( void )
{
    char ** input = NULL;
    size_t ma = 0;
    int na = 0; // ma..počet řádek, na..délka řádky

    printf ( "Zadejte hraci plochu:\n" );
    if ( ! readInput ( &input, &ma, &na ) )
        return error();
    for ( size_t i = 0; i < ma; i++ )
    {
        for ( int j = 0; j < na - 1; j++ )
        {
            if ( input[i][j] == '.' )
                input[i][j] = getResult ( input, ma, na, i, j );
        }
    }

    printf ( "Vyplnena hraci plocha:\n" );
    for ( size_t i = 0; i < ma; i++ )
        printf ( "%s", input[i] );
   
    deleteArr ( &input, ma );
    return 0;
}