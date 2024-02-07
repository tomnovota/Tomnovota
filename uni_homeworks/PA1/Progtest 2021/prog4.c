#include <stdio.h>

#define ID_MAX 100000
#define LOG_MAX 1000000

int pokolikate [ ID_MAX ] [ 2 ];
int vstupy [ LOG_MAX + 1 ] [ 3 ];

int dotaz ( int celkemLogu )
{
    int from, to;
    int uniq = 0;
    if ( scanf ( "%d %d ", &from, &to ) != 2
            || from < 0
                || to >= ( celkemLogu - 1 )
                    || from > to )
        return 0;

    from += 1;
    to += 1;

    for ( int i = from; i <= to; i++ )
    {
        if ( vstupy [ i ] [ 2 ] < from )
            uniq += 1;
    }

    printf ("> %d / %d\n", uniq, ( to - from + 1 ));
    return 1;
}

int navsteva ( int celkemLogu )
{
    int id;
    if ( scanf ( "%d ", &id ) != 1
            || id < 0
                || id >= ID_MAX
                    || celkemLogu > LOG_MAX )
        return 0;

    pokolikate [ id ] [ 0 ] += 1;
    int kolikata = pokolikate [ id ] [ 0 ];

    vstupy [ celkemLogu ] [ 0 ] = id;
    vstupy [ celkemLogu ] [ 1 ] = kolikata;

    int kdePosledni = pokolikate [ id ] [ 1 ];
    vstupy [ celkemLogu ] [ 2 ] = kdePosledni;

    pokolikate [ id ] [ 1 ] = celkemLogu;

    if ( kolikata == 1 )
    {
        printf ( "> prvni navsteva\n" );
        return 1;
    }
    printf ( "> navsteva #%d\n", kolikata );
    return 1;
}

int nacteni ()
{
    char znak;
    int celkemLogu = 1;

    while ( scanf ( "%c ", &znak ) == 1
                && ( znak == '+' || znak == '?' ) )
    {
        if ( znak == '+' )
        {
            if ( navsteva ( celkemLogu ) != 1 )
                return 0;
            celkemLogu += 1;
        }
        else if ( dotaz ( celkemLogu ) != 1 )
            return 0;
    }
    if ( ! feof ( stdin ) )
        return 0;
    return 1;
}

int main ( void )
{
    printf( "Pozadavky:\n" );

    if ( nacteni () != 1 )
    {
        printf ( "Nespravny vstup.\n" );
        return 1;
    }
    return 0;
}

