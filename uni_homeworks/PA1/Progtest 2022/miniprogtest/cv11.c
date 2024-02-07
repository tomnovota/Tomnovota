#include <stdio.h>
#include <stdlib.h>

int error ()
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

int printRec ( int cipher_max, int same_max, int cipher_count, long long num, long long append_num, int append_num_count )
{
    int count = 0;
    if ( cipher_count == cipher_max
        || same_max == append_num_count )
        return 0; // trivial

    num *= 10;
    num += append_num;
    printf ( "%lld\n", num ); // print updated number
    count++;
    count += printRec ( cipher_max, same_max, cipher_count + 1, num, append_num, append_num_count + 1 );
    count += printRec ( cipher_max, same_max, cipher_count + 1, num, ! append_num, 0 );
    return count;
}

int main ( void )
{
    int count = 1, cipher_max, same_max;

    printf ( "Pocet cifer:\n" );
    if ( scanf ( "%d", &cipher_max ) != 1
        || cipher_max <= 0 || cipher_max > 64 )
        return error();

    printf ( "Max. stejnych po sobe:\n" );
    if ( scanf ( "%d", &same_max ) != 1
        || same_max <= 0 )
        return error();

    printf ( "1\n" );
    count += printRec ( cipher_max, same_max, 1, 1, 0, 0 );
    count += printRec ( cipher_max, same_max, 1, 1, 1, 1 );
    printf ( "Celkem: %d\n", count );
    return 0;
}