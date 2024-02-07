#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#endif /* __PROGTEST__ */

char getKeyChar ( const char * key, int * index, int size )
{
    char tmp = key[*index];
    if ( ++(*index) == size )
        *index = 0;
    return tmp;
}

int getLength ( const char * arr )
{
    int i = 0;
    while ( arr[i++] );
    return i - 1;
}

void appendRes ( char ** res [], int * size, int count )
{
    if ( count == *size )
    {
        *size *= 2;
        *res = ( char ** ) realloc ( *res, *size * sizeof(**res) );
    }
}

void appendWord ( char * word [], int * size, int length )
{
    if ( length == *size )
    {
        *size *= 2;
        *word = ( char * ) realloc ( *word, *size * sizeof(**word) );
    }
}

void initWord ( char * word [], int * size, int * length )
{
    *length = 0;
    *size = 100;
    *word = ( char * ) malloc ( *size * sizeof(**word) );
}

char ** deleteArr ( char * arr [], int size )
{
    for ( int i = 0; i <= size; i++ )
        free ( arr[i] );
    free ( arr );
    return NULL;
}

char ** decrypt ( const char * key, const char * str )

{
    if ( key == NULL || str == NULL )
        return NULL;
    int key_size = getLength ( key );

    if ( key_size < 1 )
        return NULL;

    // init result
    int res_size = 100;
    int word_count = 0;
    char ** result = ( char ** ) malloc ( res_size * sizeof ( *result ) );

    // init first word
    int word_length = 0;
    int word_size = 100;
    result[0] = ( char * ) malloc ( word_size * sizeof(*result) );

    // init variables for cyklus
    int key_index = 0;
    int str_index = 0;
    char tmp_key = getKeyChar ( key, &key_index, key_size );
    char tmp_res = tmp_key ^ str[str_index];
    int space_count = 0;
    if ( isspace ( tmp_res ) )
    {
        str_index++;
        tmp_key = getKeyChar ( key, &key_index, key_size );
        tmp_res = tmp_key ^ str[str_index];
        space_count++;
    }
    while ( tmp_res != '\0' )
    {
        appendWord ( &(result[word_count]), &word_size, word_length );
        if ( isspace ( tmp_res ) )
        {
            if ( space_count )
                return deleteArr ( result, word_count );
            result[word_count][word_length] = '\0';
            word_count++;
            appendRes ( &result, &res_size, word_count );
            initWord ( &(result[word_count]), &word_size, &word_length );
            space_count++;
        } else
        {
            result[word_count][word_length] = tmp_res;
            word_length++;
            space_count = 0;
        }
        str_index++;
        tmp_key = getKeyChar ( key, &key_index, key_size );
        tmp_res = tmp_key ^ str[str_index];
    }
    if ( word_length == 0 )
    {
        free ( result[word_count] );
        result[word_count] = NULL;
        return result;
    }
    result[word_count][word_length] = '\0';
    word_count++;
    appendRes ( &result, &res_size, word_count );
    result[word_count] = NULL;
    return result;
}

#ifndef __PROGTEST__
void destroyLogs (char ** memblock) {
    if (!memblock)
        return;
    for (size_t i = 0; memblock[i] != NULL; ++i)
        free(memblock[i]);
    free(memblock);
}

int main (int argc, char * argv []) {

    char ** result;
    const char str0[] = {
        0x6c, 0x72, 0x41, 0x52, 0x1d, 0x12, 0x18, 0x00, 
        0x09, 0x0e, 0x3c
    };
    result = decrypt("<3progtest", str0);
    assert ( ! strcmp ( result[0], "PA1" ) );
    assert ( ! strcmp ( result[1], "rulezz" ) );
    assert ( result[2] == NULL );
    destroyLogs(result);

    const char str1[] = {
        0x1b, 0x45, 0x0d, 0x03, 0x7f, 0x08, 0x41, 0x3e, 
        0x61, 0x52, 0x26, 0x44, 0x09, 0x65, 0x1f, 0x0b, 
        0x00, 0x13, 0x41
    };
    result = decrypt("I love PA1", str1);
    assert ( ! strcmp ( result[0], "Real" ) );
    assert ( ! strcmp ( result[1], "man" ) );
    assert ( ! strcmp ( result[2], "code" ) );
    assert ( ! strcmp ( result[3], "in" ) );
    assert ( ! strcmp ( result[4], "C" ) );
    assert ( result[5] == NULL );
    destroyLogs(result);

    const char str2[] = {
        0x19, 0x06, 0x1d, 0x7f, 0x1b, 0x55, 0x24, 0x75, 
        0x15, 0x65, 0x09, 0x02, 0x28, 0x1c, 0x06, 0x54, 
        0x0e, 0x4f, 0x25, 0x17, 0x10, 0x46, 0x6f
    };
    result = decrypt("Monty Python", str2);
    assert ( ! strcmp ( result[0], "Tis" ) );
    assert ( ! strcmp ( result[1], "but" ) );
    assert ( ! strcmp ( result[2], "a" ) );
    assert ( ! strcmp ( result[3], "flesh" ) );
    assert ( ! strcmp ( result[4], "wound." ) );
    assert ( result[5] == NULL );
    destroyLogs(result);

    const char str3[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x53, 
        0x49, 0x11, 0x10, 0x4e, 0x10, 0x4f, 0x09, 0x1a, 
        0x00, 0x55, 0x73
    };
    result = decrypt(" Romanes eunt domus ", str3);
    assert ( ! strcmp ( result[0], "Romani" ) );
    assert ( ! strcmp ( result[1], "ite" ) );
    assert ( ! strcmp ( result[2], "domum" ) );
    assert ( result[3] == NULL );
    destroyLogs(result);

    const char str4[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x53, 
        0x00, 0x0c, 0x01, 0x0b, 0x54, 0x44, 0x0b, 0x02, 
        0x18, 0x18, 0x53, 0x20
    };
    result = decrypt(" Romanes eunt domus ", str4);
    assert ( result == NULL );
    destroyLogs(result);

    const char str5[] = {
        0x70, 0x59, 0x39, 0x07, 0x01, 0x1a, 0x29, 0x79
    };
    result = decrypt("Python", str5);
    assert ( result == NULL );
    destroyLogs(result);

    assert ( decrypt ( NULL, "1234" ) == NULL );
    return 0;
}
#endif /* __PROGTEST__ */

