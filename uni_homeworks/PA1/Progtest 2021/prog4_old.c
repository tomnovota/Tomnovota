#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_SIZE 26

int error ( void )
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

void initGround ( char ground [][MAX_SIZE], int size )
{
    for ( int i = 0; i < size; i ++ )
    {
        for ( int j = 0; j < size; j ++ )
            ground [i][j] = 0;
    }
}

int readInput ( int * x )
{
    printf ( "Rozmery pole:\n" );
    if ( scanf ( "%d", x ) != 1
        || * x < 3 || * x > MAX_SIZE )
        return 0;
    return 1;
}

int convertCol ( char x )
{
    return x - 97;
}

int readMove ( int * row, int * col, char ground [][MAX_SIZE], int size )
{
    char sym;
    if ( scanf ( " %c%d ", & sym, row ) != 2
        || sym < 'a'
        || sym > 'a' + size - 1
        || * row < 1
        || * row > size )
        return 0;
    -- * row; // start 0
    * col = convertCol ( sym );
    if ( ground [* row][* col] ) // already played
        return 0;
    return 1;
}

void updateScore ( int score, int * len, int * count )
{
    if ( score > * len )
    {
        * len = score;
        * count = 1;
    } else if ( score == * len )
        ++ * count;
}

int inBorder ( int size, int row, int col )
{
    if (   row >= 0
        && col >= 0
        && row < size
        && col < size )
        return 1;
    return 0;    
}

int findRow ( char ground [][MAX_SIZE], int size, int row, int col, char sym, int dir1, int dir2 )
{
    int len = 1;
    int tmp_row = row + dir1;
    int tmp_col = col + dir2;
    while ( inBorder ( size, tmp_row, tmp_col ) && ground [tmp_row][tmp_col] == sym )
    {
        len ++;
        tmp_row += dir1;
        tmp_col += dir2;
    }
    tmp_row = row - dir1;
    tmp_col = col - dir2;
    while ( inBorder ( size, tmp_row, tmp_col ) && ground [tmp_row][tmp_col] == sym )
    {
        len ++;
        tmp_row -= dir1;
        tmp_col -= dir2;
    }
    return len;
}

void getResult ( char ground [][MAX_SIZE], int size, int row, int col, char sym, int * len, int * count )
{
    updateScore ( findRow ( ground, size, row, col, sym, 1, 0 ), len, count ); // nahoru
    updateScore ( findRow ( ground, size, row, col, sym, 0, 1 ), len, count ); // vodorovně
    updateScore ( findRow ( ground, size, row, col, sym, -1, -1 ), len, count ); // levo šikmo
    updateScore ( findRow ( ground, size, row, col, sym, -1, 1 ), len, count ); // pravo šikmo
    if ( * len == 1 )
        * count -= 3;
}

int main ( void )
{
//  -- INIT --
    int size, row, col, lenX = 0, countX = 0, lenO = 0, countO = 0;
    int player = 0;
    char name [2] = { 'X', 'O' };
    char ground[MAX_SIZE][MAX_SIZE];

//  -- INPUT --
    if ( ! readInput ( & size ) )
        return error ();
    initGround ( ground, size );

//  -- PROGRAM --
    while ( ! feof ( stdin ) )
    {
        if ( ! readMove ( & row, & col, ground, size ) )
            return error ();
        ground [row][col] = name [player]; // played move
        if ( ! player ) // X
        {
            getResult ( ground, size, row, col, name [player], & lenX, & countX );
            printf ( "Hrac 'X': nejdelsi %d (%d x)\n", lenX, countX );
        } else {
            getResult ( ground, size, row, col, name [player], & lenO, & countO );
            printf ( "Hrac 'O': nejdelsi %d (%d x)\n", lenO, countO );
        }
        player = ! player; // switch player
    }
    return 0;
}