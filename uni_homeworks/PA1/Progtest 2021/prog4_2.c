#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int lenght;
    int count;
} best;

typedef struct {
    char sym;
    int pos1;
    int pos2;
} box;

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int getSize( int * size )
{
    int x;

    printf( "Rozmery pole:\n" );
    if ( scanf( " %d", &x ) != 1
        || x < 3 || x > 26 )
        return error();
    * size = x;
    return 1;
}

void zeroArr( box ** arr, int size )
{
    box * tmp;
    for ( int i = 0; i < size; i++ )
    {
        tmp = arr[i];
        for ( int j = 0; j < size; j++ )
//            {
            tmp[j].sym = 0;
//            printf( "%d", tmp[j].sym );}
//        printf( "\n" );
    }
}

box ** arrInit ( int size )
{
    box ** tmp = (box **) malloc ( size * sizeof( box * ) );
    for ( int i = 0; i < size; i++ )
    {
        tmp[i] = (box *) malloc( size * sizeof( box ) );
    }
    zeroArr( tmp, size );
    return tmp;
}

int arrDestroy ( box ** arr, int size )
{
    for ( int i = 0; i < size; i++ )
    {
        free ( arr[i] );
    }
    free ( arr );
    return 0;
}

int getPos ( unsigned char * pos1, int * pos2, int size )
{
    unsigned char a;
    int b;

    if ( scanf( " %c%d", &a, &b ) != 2
        || a < 97 || a > (96 + size)
        || b < 1 || b > size
        ) return 0;
    * pos1 = a;
    * pos2 = b;
    return 1;
}

box * convert ( unsigned char p1, int p2, box ** arr )
{
    int pos1 = p1 - 97;
    int pos2 = p2 - 1;
    box * tmp = &(arr[pos1] [pos2]);
    tmp->pos1 = pos1;
    tmp->pos2 = pos2;
    return tmp;
}

int getLenght ( box ** arr, int size, box * current, int d1, int d2 )
{
    int len = 1, x = current->pos1 + d1, y = current->pos2 + d2;
    char sym = current->sym;
    while ( x >= 0 && y >= 0 && x < size && y < size && arr[x][y].sym == sym )
    {
        len++;
        x += d1;
        y += d2;
    }
    d1 *= -1;
    d2 *= -1;
    while ( x >= 0 && y >= 0 && x < size && y < size && arr[x][y].sym == sym )
    {
        len++;
        x += d1;
        y += d2;
    }
    return len;
}

best max ( int a, int b, int c, int d )
{
    best tmp = { 0, 0 };
    if ( a > tmp.lenght )
        tmp.lenght = a;
    if ( b > tmp.lenght )
        tmp.lenght = b;
    if ( c > tmp.lenght )
        tmp.lenght = c;
    if ( d > tmp.lenght )
        tmp.lenght = d;
    if ( a == tmp.lenght )
        tmp.count += 1;
    if ( b == tmp.lenght )
        tmp.count += 1;
    if ( c == tmp.lenght )
        tmp.count += 1;
    if ( d == tmp.lenght )
        tmp.count += 1;
    return tmp;
}

best findScore( box ** arr, int size, box * current )
{
    return max ( 
        getLenght( arr, size, current, 1, 0 ),
        getLenght( arr, size, current, 1, 1 ),
        getLenght( arr, size, current, 0, 1 ),
        getLenght( arr, size, current, -1, 1 ) );
}

void compareBest ( best currentScore, best * bestScore )
{
    if ( currentScore.lenght > bestScore->lenght )
    {
        *bestScore = currentScore;
    }
    if ( currentScore.lenght == bestScore->lenght )
        bestScore->count += currentScore.count;
}

int writeMove ( int size, box ** arr, unsigned char p1, int p2, char sym, best * bestScore )
{
    box * pos = convert( p1, p2, arr );
    if ( pos->sym )
        return 0;
    pos->sym = sym;
    best currentScore = findScore( arr, size, pos );
    compareBest( currentScore, bestScore );
    return 1;
}

char getSym ( int turn )
{
    if ( turn )
        return 'O';
    return 'X';
}

best * getScore ( int turn, best * scoreX, best * scoreO )
{
    if ( turn )
        return scoreO;
    return scoreX;
}

int play ( int size, box ** arr )
{
    unsigned char pos1;
    int pos2, turn = 0;
    best scoreX = {0, 0}, scoreO = {0, 0};
    best * scoreNow;
    char sym;

    while ( ! feof(stdin) )
    {
        sym = getSym( turn );
        scoreNow = getScore( turn, &scoreX, &scoreO );

        if ( ! getPos( &pos1, &pos2, size ) )
            return error();
        if ( ! writeMove( size, arr, pos1, pos2, sym, scoreNow) )
            return error();
        printf( "Hrac %c: nejdelsi %d (%d x)\n", sym, scoreNow->lenght, scoreNow->count );
        turn = ! turn;
    }
    return 0;
}

int main ( void )
{
    int size;
    box ** arr;
    
    if ( ! getSize( &size ) )
        return 0;
    arr = arrInit( size );
    play( size, arr );
    return arrDestroy( arr, size );
}

/*
a-z ( 97-122 )
1-9( 49-57 )
*/

/*
best sloupec ( best * tmpBest, char * tmp, char ** arr, int size, int sym )
{
    int i = 0, lenght = 0, count = 0;
    while ( i < ( size - 1 ) )
    {
        while ( tmp[i] == sym )
        {
            lenght++;
            i++;
        }
        if ( lenght > tmpBest->lenght )
        {
            tmpBest->lenght = lenght;
            tmpBest->count = 1;
        }
        else if ( lenght == tmpBest->lenght )
            tmpBest->count++;
        i++;
    }
}

void svisle ( best * score, char ** arr, int size, int sym )
{
    best tmpBest = {0, 0};
    char * tmp;
    for ( int i = 0; i < size; i++ )
    {
        tmp = arr[i];
    }
}

void vodorovne ( best * score, char ** arr, int size, int sym )
{
    
}

void sikmoP ( best * score, char ** arr, int size, int sym )
{
    
}

void sikmoL ( best * score, char ** arr, int size, int sym )
{
    
}
*/