#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void limit ()
{

}

void options ( char * shortcut, char * stc, int num )
{

}

int formatInput ( char * stcTMP, char * stc, char * type, int * num )
{
    if ( int a = sscanf ( stcTMP, "%c %d \"", type, num ) != 2
        || ( * type != '#' && * type != '?' )
        || * num < 1 )
        return 1;
    char * pTMP = stcTMP;
    char * p = stc;
    while ( * pTMP )
    {
        if ( * pTMP != ' ' )
        {
            * p = tolower ( * pTMP );
            p++;
        }
        else if ( * ( p - 1 ) != '\0' )
        {
            * p = '\0';
            p++;
        }
        pTMP++;
    }
    free ( stcTMP );
    return 0;
}

int nacteniZkratky ( char * shortcut, long unsigned int * delkaZkratky )
{
    if ( getline ( & shortcut, delkaZkratky, stdin ) < 1 )
    {
        free ( shortcut );
        return 0;
    }
    char * p = shortcut;
    while ( * p )
    {
        if ( ( * p < 'A'
            || * p > 'Z' ) && * p == ' ' )
            {
                free ( shortcut );
                return 0;
            }
        * p = tolower ( * p );
        p++;
    }
    printf ( "Povedlo se nacist: %s", shortcut );
    return 1;
}

int main ( void )
{
    long unsigned int delkaZkratky = 0;
    char * shortcut = NULL;
    printf ( "Zkratka:\n" );
    if ( nacteniZkratky ( shortcut, &delkaZkratky ) == 0 )
    {
        printf ( "Nespravny vstup.\n" );
        return 1;
    }
    long unsigned int capacity;
    char * stcTMP = NULL;
    printf ( "Problemy:\n" );
    while ( getline ( &stcTMP, &capacity, stdin ) > 0 )
    {
        char * stc = ( char * ) malloc ( capacity );
        char type;
        int num;
        if ( capacity < 3
            //|| stcTMP [capacity - 1] != '"'
            || formatInput ( stcTMP, stc, & type, &num ) )
        {
            free ( shortcut );
            free ( stc );
            free ( stcTMP );
            printf ( "Nespravny vstup.\n" );
            return 1;
        }
        if ( type == '#' )
            options ( shortcut, stc, num );
        else
            limit ();
        free ( stc );
    }
    free ( shortcut );
    return 0;
}

