#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_MAX 101

typedef struct
{
    char name[CHAR_MAX];
    int count;
} PRODUCT;

void swap ( PRODUCT * a, PRODUCT * b)
{
    PRODUCT tmp = * a;
    * a = * b;
    * b = tmp;
}

int alreadyEx ( int celkem, PRODUCT * zaznamy, char tmp[] )
{
    for ( int i = 0; i < celkem; i++ )
    {
        if ( ! strcmp ( tmp, zaznamy[i].name ) )
        {
            zaznamy[i].count += 1;
            while ( i != 0 &&
                   zaznamy[i].count > zaznamy[i-1].count )
            {
                swap ( &zaznamy[i], &zaznamy[i-1] );
                i--;
            }
        return 1;
        }
    }
    return 0;
}

int readPlus ( PRODUCT * zaznamy, int * celkem )
{
    char tmp [CHAR_MAX];
    if ( scanf ( "%100s", tmp ) != 1
        || strlen ( tmp ) > 99)
        return 0;

    if ( alreadyEx ( * celkem, zaznamy, tmp ) )
        return 1;

    strncpy ( zaznamy[* celkem].name, tmp, CHAR_MAX ) ;
    zaznamy[* celkem].count = 1;
    (* celkem)++;
    return 1;
}

void readHash ( PRODUCT * zaznamy, int celkem, int N, char sym )
{
    int j = 0;
    int kusy = 0;
    if ( N > celkem )
        N = celkem;
    while ( j < N )
    {
        int n = j;
        while ( zaznamy[j].count == zaznamy[n].count )
        {
            j++;
        }
        int hodnota = zaznamy[n].count;
        kusy += hodnota * (j-n);
        if ( sym == '#' )
        {
            if ( n + 1 < j )
            {
                for ( int i = n; i < j; i++ )
                    printf ( "%d.-%d. %s, %dx\n", n+1, j, zaznamy[i].name, hodnota );
            }
            else
                printf ( "%d. %s, %dx\n", n+1, zaznamy[n].name, hodnota );
        }
    }
    printf ( "Nejprodavanejsi zbozi: prodano %d kusu\n", kusy );
}

int readSym ( char * sym )
{
    char a;
    if ( scanf ( " %c ", &a ) != 1
        || ( a != '+'
            && a != '#'
            && a != '?' ) )
        return 0;
    * sym = a;
    return 1;
}

int readN ( int * N )
{
    int a;
    if ( scanf ( "%d", &a ) != 1
        || a < 1)
        return 0;
    * N = a;
    return 1;
}

int main ( void )
{
    int N, celkem = 0;
    char sym;
    PRODUCT * zaznamy = NULL;

    printf ( "Pocet sledovanych:\n" );
    if ( ! readN ( &N ) )
    {
        printf ( "Nespravny vstup.\n" );
        return 1;
    }
    printf ( "Pozadavky:\n" );

    while ( ! feof ( stdin ) )
    {
        if ( ! readSym ( &sym ) )
        {
            printf ( "Nespravny vstup.\n" );
            free ( zaznamy );
            return 1;
        }
        if ( sym == '+' )
        {
            zaznamy = (PRODUCT *) realloc ( zaznamy, ( celkem + 2 ) * sizeof ( PRODUCT ) );
            if ( ! readPlus ( zaznamy, &celkem ) ) {
                printf ( "Nespravny vstup.\n" );
                free ( zaznamy );
                return 0;
            }
        }
        else
        {
            readHash ( zaznamy, celkem, N, sym );
        }
    }
    free ( zaznamy );
    return 0;
}

