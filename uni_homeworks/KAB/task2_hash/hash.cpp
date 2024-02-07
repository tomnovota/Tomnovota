#ifndef __PROGTEST__
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

#endif /* __PROGTEST__ */

const int ASCII_NUM = 48;
const int ASCII_ALPHA = 55;
const int BYTE_SIZE = 8;
const int LAST_BYTE_POS = 8 - 1;

int error ( char * text )
{
    free ( text );
    return 0;
}

bool readBit ( unsigned char byte, int pos )
{
    return byte & ( 1 << pos );
}

bool nullBits ( unsigned char bytes [], int cnt ) // LSB
{
    int seq = 0;
    unsigned char byte = bytes [seq ++];
    for ( int i = 0, pos = 0; i < cnt; i ++, pos ++ )
    {
        if ( i != 0 && i % 8 == 0 ) 
        {
            byte = bytes [seq ++];
            pos = 0;
        }
        if ( readBit ( byte, LAST_BYTE_POS - pos ) )
            return false;
    }
    return true;
}

char convertToHex ( unsigned char byte, int part )
{
    unsigned tmp = 0;
    for ( int j = 4 * part, val = 1; j < 4 + (4 * part); j ++, val *= 2 )
        tmp += readBit ( byte, j ) * val;
    return tmp < 10 ? tmp + ASCII_NUM : tmp + ASCII_ALPHA;
}

bool bytesToHex ( const unsigned char bytes [], char ** hex, const unsigned int len )
{
    unsigned char byte;
    *hex = ( char * ) malloc ( ( 2 * len + 1) * sizeof ( **hex ) );
    size_t pos = 0;
    for ( size_t i = 0; i < len; i ++ )
    {
        byte = bytes [i];
        (*hex) [pos ++] = convertToHex ( byte, 1 );
        (*hex) [pos ++] = convertToHex ( byte, 0 );
    }
    (*hex) [ 2 * len ] = '\0';
    return true;
}

int findHashEx ( int bits, char ** message, char ** hash, const char * hashFunction )
{
    if ( bits < 0 || bits > EVP_MAX_MD_SIZE )
        return 0;
    EVP_MD_CTX * ctx;  // struktura kontextu
    const EVP_MD * type; // typ pouzite hashovaci funkce
    unsigned char hash_bytes [ EVP_MAX_MD_SIZE ]; // char pole pro hash - 64 bytu (max pro sha 512)
    unsigned int length;  // vysledna delka hashe

    OpenSSL_add_all_digests(); // Inicializace OpenSSL hash funkci
    if ( ! ( type = EVP_get_digestbyname ( hashFunction ) ) ) // Zjisteni, jaka hashovaci funkce ma byt pouzita
        return 0;

    unsigned char message_bytes [ EVP_MAX_MD_SIZE ];
    int block_size = EVP_MD_block_size ( type );

    do { // Hash the text
        if ( RAND_bytes ( message_bytes, sizeof ( message_bytes ) ) != 1 ) // ziskani textu na zakodovani
            return 0;
        message_bytes [ sizeof ( message_bytes ) - 1 ] = '\0';

        if ( ( ctx = EVP_MD_CTX_new() ) == NULL // create context
            || ! EVP_DigestInit_ex ( ctx, type, NULL ) // context setup for our hash type
            )
            return 0;
        
        for ( size_t i = 0; i < sizeof ( message_bytes ); i += block_size )
        {
            if ( ! EVP_DigestUpdate ( ctx, message_bytes, sizeof ( message_bytes ) ) ) // feed the message in
                return 0;
        }
        if ( ! EVP_DigestFinal_ex ( ctx, hash_bytes, &length ) ) // get the hash
            return 0;

        EVP_MD_CTX_free ( ctx ); // destroy the context

    } while ( ! ( nullBits ( hash_bytes, bits ) ) );

    bytesToHex ( hash_bytes, hash, length );
    bytesToHex ( message_bytes, message, sizeof ( message_bytes ) );
    return 1;
}

int findHash ( int bits, char ** message, char ** hash )
{
    return findHashEx ( bits, message, hash, "sha512" );
}

#ifndef __PROGTEST__

int checkHash ( int bits, char * hexString )
{
    int pos = 0;

    for ( ; pos < bits / 4; pos ++ )
    {
        if ( hexString [ pos ] != '0' )
            return 0;
    }
    if ( bits % 4 == 0 )
        return 1;
    char tmp = 8;
    for ( int i = 1; i < bits % 4; i ++ )
        tmp /= 2;
    tmp += ASCII_NUM;
    if ( hexString [ pos ] >= tmp )
    {
        return 0;
    }
    return 1;
}

int main ( void ) {

    unsigned char x [] = {39};
    char * hex;
    assert ( bytesToHex ( x, &hex, 1 ) );
    assert ( ! strcmp ( hex, "27" ) );
    free ( hex );
    assert ( nullBits ( x, 1 ) );
    assert ( nullBits ( x, 2 ) );
    assert ( ! nullBits ( x, 3 ) );

    unsigned char y [] = {0, 1};
    assert ( nullBits ( y, 1 ) );
    assert ( nullBits ( y, 2 ) );
    assert ( nullBits ( y, 3 ) );
    assert ( nullBits ( y, 4 ) );
    assert ( nullBits ( y, 5 ) );
    assert ( nullBits ( y, 6 ) );
    assert ( nullBits ( y, 7 ) );
    assert ( nullBits ( y, 8 ) );
    assert ( nullBits ( y, 11 ) );
    assert ( nullBits ( y, 15 ) );
    assert ( ! nullBits ( y, 16 ) );
    assert ( ! nullBits ( y, 17 ) );
    assert ( ! nullBits ( y, 18 ) );


    unsigned char z [] = {1,200};
    assert ( bytesToHex ( z, &hex, 2 ) );
    assert ( ! strcmp ( hex, "01C8" ) );
    free ( hex );

    // PROGTEST
    char * message, * hash;
    assert(findHash(0, &message, &hash) == 1);
    assert(message && hash && checkHash(0, hash));

    printf ( "message = %s\n", message );
    printf ( "hash = %s\n", hash );

    free(message);
    free(hash);

    assert(findHash(1, &message, &hash) == 1);
    assert(message && hash && checkHash(1, hash));

    printf ( "message = %s\n", message );
    printf ( "hash = %s\n", hash );

    free(message);
    free(hash);

    assert(findHash(2, &message, &hash) == 1);
    assert(message && hash && checkHash(2, hash));

    printf ( "message = %s\n", message );
    printf ( "hash = %s\n", hash );

    free(message);
    free(hash);

    assert(findHash(3, &message, &hash) == 1);
    assert(message && hash && checkHash(3, hash));

    printf ( "message = %s\n", message );
    printf ( "hash = %s\n", hash );

    free(message);
    free(hash);

    assert(findHash(-1, &message, &hash) == 0);

    assert(findHash(20, &message, &hash) == 1);
    assert(message && hash && checkHash(20, hash));

    printf ( "message = %s\n", message );
    printf ( "hash = %s\n", hash );

    free(message);
    free(hash);

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

