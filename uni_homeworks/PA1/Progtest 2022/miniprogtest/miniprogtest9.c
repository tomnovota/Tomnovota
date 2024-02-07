#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int error ()
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

int onlyWhite ( char * string )
{
    int i = 0;
    while ( string[i] )
    {
        if ( ! isspace ( string[i] ) )
            return 0;
        i ++;
    }
    return 1;
}

int checkInput ( char * string, int * len )
{
    if ( * len < 2
        || onlyWhite ( string ) )
    {
        free ( string );
        return 0;
    }
    (* len) --;
    if ( string [* len] != '\n' )
    {
        free ( string );
        return 0;
    }
    string [* len] = '\0'; // omit newline
    return 1;
}

void printAnswer ( int x )
{
    switch ( x )
    {
        case 0:
            printf ( "Retezec neni palindrom.\n" ); break;
        case 1:
            printf ( "Retezec je palindrom (case-insensitive).\n" ); break;
        case 2:
            printf ( "Retezec je palindrom (case-sensitive).\n" ); break;
    }
}

int getLen ( char * string, int len )
{
    int count = 0;
    for ( int i = 0; i < len; i++ )
    {
        if ( isspace ( string[i] ) )
            continue;
        count ++;
    }
    return count / 2;
}

int getAnswer ( char * string, int len )
{
    int start = 0,
        end = len - 1,
        steps = getLen ( string, len ),
        result = 2;

    for ( int i = 0; i < steps; i++ )
    {
        while ( isspace ( string[start + i] ) )
            start++;
        while ( isspace ( string[end - i] ) )
            end--;
        if ( string[start + i] == string[end - i] )
            continue;
        if ( tolower ( string[start + i] ) == tolower ( string[end - i] ) )
        {
            result = 1;
            continue;
        }
        return 0;
    }
    return result;
}

int main ( void )
{
//  -- Initialization --
    char * string = NULL;
    size_t n = 0;
    int items;

//  -- Program --
    printf ( "Zadejte retezec:\n" );
    while ( ( items = getline ( & string, & n, stdin ) ) != -1 )
    {
        if ( ! checkInput ( string, & items ) )
            return error ();
        
        printAnswer ( getAnswer ( string, items ) );
        
        // clean string
        free ( string );
        string = NULL;
    }
    free ( string );
    if ( ! feof ( stdin ) )
        return error ();
    return 0;
}