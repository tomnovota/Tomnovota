#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char error ()
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

char firstLine ( int matrix [][100], int * size )
{
    int num;
    char tmp;
    while ( scanf ( " %d", &num ) == 1 && *size < 100 )
    {
        matrix[0][*size] = num;
        (*size)++;
        if ( scanf ( "%c", &tmp ) == 1 && tmp == '\n' )
            return 1;
        if ( ! isspace(tmp) )
        return 0;
    }
    return 0;
}

char readInput ( int matrix [][100], int * columns, int * lines )
{
    int num;
    char tmp;
    if ( ! firstLine ( matrix, columns ) )
        return 0;
    (*lines)++;
    if ( scanf (" "), feof(stdin) )
        return 1;
    while ( *lines < 100 ) // rows
    {
        for ( int j = 0; j < *columns; j++ ) // columns
        {
            if ( scanf ( " %d", &num ) != 1 )
                return 0;
            matrix[*lines][j] = num;
        }
        if ( scanf ( "%c", &tmp ) != 1 || tmp != '\n' )
            return 0;
        (*lines)++;
        if ( scanf ( " " ), feof(stdin) )
            return 1;
    }
    return 0;
}

char horizont ( int matrix [][100], int columns, int lines )
{
    if ( lines == 1 )
        return 1;
    int upper = lines / 2 - 1;
    int lower = lines % 2 == 0 ? upper + 1 : upper + 2;
    for ( int i = 0; i < columns; i++ )
    {
        for ( int j = 0; j <= upper; j++ )
        {
            if ( matrix [upper - j][i] != matrix [lower + j][i] )
                return 0;
        }
    }
    return 1;
}

char vertical ( int matrix [][100], int columns, int lines )
{
    if ( columns == 1 )
        return 1;
    int left = columns / 2 - 1;
    int right = columns % 2 == 0 ? left + 1 : left + 2;
    for ( int i = 0; i < lines; i++ )
    {
        for ( int j = 0; j <= left; j++ )
        {
            if ( matrix [i][left - j] != matrix [i][right + j] )
                return 0;
        }
    }
    return 1;
}

char middle ( int matrix [][100], int columns, int lines )
{
    if ( lines == 1 || columns == 1 )
        return 1;
    if ( lines != columns )
        return 0;
    int mid = lines / 2;
    int comp = 0;
    if ( lines % 2 == 0 )
    {
        mid--;
        comp++;
    }
    for ( int i = lines % 2; i <= mid; i++ )
    {
        if ( matrix [mid - i][mid - i] != matrix [mid + i + comp][mid + i + comp]
            || matrix [mid - i][mid] != matrix [mid + i + comp][mid]
            || ( comp && (matrix [mid - i][mid + 1] != matrix [mid + i + comp][mid + 1]) )
            || matrix [mid][mid - i] != matrix [mid][mid + i+ comp]
            || ( comp && (matrix [mid + 1][mid - i] != matrix [mid + 1][mid + i + comp]) )
            || matrix [mid + i + comp][mid - i] != matrix [mid - i][mid + i + comp] )
            return 0;
    }
    return 1;
}

int main ( void )
{
    int matrix [100][100];
    int columns = 0, lines = 0;
    char answer [2][4];
    int yes = 0;
    strcpy(answer [0], "ne");
    strcpy(answer [1], "ano");

    printf ( "Matice:\n" );
    if ( ! readInput ( matrix, &columns, &lines ) )
        return error();
    yes = horizont ( matrix, columns, lines );
    printf ( "Symetrie podle horizontalni osy: %s\n", answer[yes] );
    yes = vertical ( matrix, columns, lines );
    printf ( "Symetrie podle vertikalni osy: %s\n", answer[yes] );
    yes = middle ( matrix, columns, lines );
    printf ( "Symetrie podle stredu: %s\n", answer[yes] );
    return 0;
}