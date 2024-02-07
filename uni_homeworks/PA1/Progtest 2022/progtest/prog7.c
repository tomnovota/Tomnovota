#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    int A;
    int B;
    int branch;
} SCORE;

void printArr ( int size [], int sh [] [4] )
{
    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j <= size[i]; j++ )
        {
            printf ( "%d ", sh[j][i] );
        }
        printf ("\n");
    }
}

char error ()
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

char validSym ( char sym, char arr [5] )
{
    char options [5] = "NSEW";
    if ( ! strchr ( options, sym )
        || strchr ( arr, sym ) )
        return 0;
    return 1;
}

int selectShoulder ( char sym )
{
    switch ( sym )
    {
        case ('N'): return 0;
        case ('S'): return 1;
        case ('E'): return 2;
        case ('W'): return 3;
    }
    return -1;
}

char readInput ( int shoulder[][4], int size[] )
{
    char sym;
    int value;
    int current;
    char found [5] = "\0\0\0\0";
    int tmp [32];
    for ( int i = 0; i < 4; i++ )
    {
        if ( scanf ( "%c : { %d", &sym, &value ) != 2
            || ! validSym ( sym, found ) )
            return 0;
        found[i] = sym;
        current = selectShoulder ( sym );
        tmp[ size[current]++ ] = value;
        while ( scanf ( " , %d ", &value ) == 1 )
        {
            tmp[ size[current]++ ] = value;
            if ( size[current] == 33 )
                return 0;
        }
        if ( scanf ( " %c ", &sym ) != 1 || sym != '}' )
            return 0;
        size[current]--; // arrange value size to be index of last element
            
        for ( int j = size[current]; j >= 0; j-- ) // reverse array - so edge values are on the end
            shoulder [size[current]-j] [current] = tmp [j];
    }
    if ( ! feof (stdin) )
        return 0;
    return 1;
}

void getCopy ( int copy [], int original [] )
{
    for ( int i = 0; i < 4; i++ )
        copy [i] = original [i];
}

char onlyZeros ( int arr [] )
{
    for ( int i = 0; i < 4; i++ )
    {
        if ( arr [i] >= 0 )
            return 0;
    }
    return 1;
}

char getDirection ( int direction )
{
    switch ( direction )
    {
        case 0: return 'N';
        case 1: return 'S';
        case 2: return 'E';
        case 3: return 'W';
    }
    return 0;
}

void printRound ( int shoulder [] [4], int originalSize [], int roundSize [], char playerB, int bestDirection )
{
    char player = playerB ? 'B' : 'A';
    char direction = getDirection ( bestDirection );
    int position = originalSize [bestDirection] - roundSize [bestDirection];
    int value = shoulder [roundSize [bestDirection]] [bestDirection];

    printf ( "%c: %c[%d] (%d)\n", player, direction, position, value );
}

SCORE selectToken ( int shoulder [] [4], int size [], SCORE * tmpScore, char whoPlays, int root )
{
    SCORE tmpValue = {0, 0};
    if ( onlyZeros ( size ) ) // trivial case, no tokens
        return tmpValue;

    SCORE bestValue;
    int bestShoulder = 0;
    char init = 0;
    int tmp_size [4];

    for ( int i = 0; i < 4; i++ )
    {
        if ( size[i] >= 0 )
        {
            getCopy ( tmp_size, size );
            tmp_size [i]--; // simulates removal of particular token
            tmpValue = selectToken ( shoulder, tmp_size, tmpScore, ! whoPlays, 0 ); // for every possible shoulder gets score
            if ( whoPlays ) // player A on turn
            {
                tmpValue.A += shoulder [size[i]] [i]; // add value of token
                if ( ! init || bestValue.A < tmpValue.A ) // updates bestValue
                {
                    init = 1; // indicates if bestShoulder already assigned
                    bestValue = tmpValue;
                    bestShoulder = i;
                }
            }
            else // player A on turn
            {
                tmpValue.B += shoulder [size[i]] [i]; // add value of token                
                if ( ! init || bestValue.B < tmpValue.B ) // updates bestValue
                {
                    init = 1; // indicates if bestShoulder already assigned
                    bestValue = tmpValue;
                    bestShoulder = i;
                }
            }            
        }
    }
    if ( root )
        bestValue.branch = bestShoulder; // returns bestShoulder for function play()

    return bestValue; // returns best value in current branch
}

void play ( int shoulder [] [4], int originalSize [], int bestScore [], int tokenCount )
{
    int playerB = 0; // indicates who is on turn
    int bestDirection; // best shoulder in current turn
    int size_copy [4]; // temporary array of sizes for recursive function
    int round_size [4]; // array of sizes in current round
    SCORE tmpScore;
    tmpScore.A = tmpScore.B = 0;
    getCopy ( round_size, originalSize );
    for ( int i = 0; i < tokenCount; i++ )
    {
        getCopy ( size_copy, round_size );
        bestDirection = selectToken ( shoulder, size_copy, &tmpScore, 1, 1 ).branch;
        bestScore [playerB] += shoulder [round_size [bestDirection]] [bestDirection];
        printRound ( shoulder, originalSize, round_size, playerB, bestDirection );
        round_size [bestDirection]--;
        playerB = ! playerB; // switch player
    }
}

int main ( void )
{
    int shoulders [32][4]; // NSEW sloupce, 32 řádků
    int size [4]; // NSEW
    for ( int i = 0; i < 4; i++ )
        size[i] = 0;

    int bestScore[2]; // A, B
    bestScore[0] = bestScore[1] = 0;
    
    printf ( "Zetony:\n" );
    if ( ! readInput ( shoulders, size ) )
        return error();

    int tokenCount = 0;
    for ( int i = 0; i < 4; i++ )
        tokenCount += size[i] + 1; // count is increased by one from last index

    play ( shoulders, size, bestScore, tokenCount );
    printf ( "Celkem A/B: %d/%d\n", bestScore[0], bestScore[1] );
    return 0;
}