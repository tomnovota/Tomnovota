#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */

typedef struct {
    char * arr;
    int capacity;
    int len;
} STRING;

int validReplace ( const char * (*replace)[2], int * count )
{
    int i = 0;
    int j;
    char * tmp;
    while ( replace[i][0] != NULL )
    {
        tmp = strdup ( replace[i][0] );
        j = i + 1;
        while ( replace[j][0] != NULL )
        {
            if ( strstr ( tmp, replace[j][0] ) == tmp || strstr ( replace[j][0], tmp ) == replace[j][0] )
            {
                free ( tmp );
                return 0;
            }
            j++;
        }
        free ( tmp );
        i++;
    }
    * count = i;
    return 1;
}

void initResult ( STRING * result )
{
    result->capacity = 300;
    result->len = 0;
    result->arr = ( char * ) malloc ( result->capacity * sizeof ( *(result->arr) ) );
}

void appendArray ( STRING * result, int len )
{
    if ( result->capacity <= result->len + 2 * len )
    {
        result->capacity *= 2;
        result->arr = ( char * ) realloc ( result->arr, result->capacity * sizeof ( char * ) );
    }
}

int findWord ( const char * (*replace)[2], const char * next, const char ** to_replace, int * len )
{
    for ( int i = 0; replace[i][0]; i++ )
    {
        if ( strstr ( next, replace[i][0] ) == next )
        {
            * to_replace = replace[i][1];
            * len = strlen ( replace[i][1] );
            return strlen ( replace[i][0] );
        }
    }
    * to_replace = next;
    * len = 1;
    return 1;
}

char * newSpeak ( const char * text, const char * (*replace)[2] )
{
    int replace_count;
    if ( ! validReplace ( replace, &replace_count ) || text == NULL )
        return NULL;
    STRING result;
    initResult ( &result );
    const char * next_word = text;
    const char * to_replace = NULL;
    int replace_len = 0;
    while ( next_word[0] != '\0' )
    {
        next_word += findWord ( replace, next_word, &to_replace, &replace_len ); // moves to next part of text to be changed
        printf ( "REPLACE = %s, LEN = %d\n", to_replace, replace_len );
        appendArray( &result, replace_len);
        strncpy(&(result.arr[result.len]), to_replace, replace_len);
        result.len += replace_len; // updates length of finished transformation
    }
    result.arr[result.len] = '\0';
    printf( "RESULT:%s\n", result.arr );
    return result.arr;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  char * res;

  const char * d1 [][2] =
  {
    { "murderer", "termination specialist" },
    { "failure", "non-traditional success" },
    { "specialist", "person with certified level of knowledge" },
    { "dumb", "cerebrally challenged" },
    { "teacher", "voluntary knowledge conveyor" },
    { "evil", "nicenest deprived" },
    { "incorrect answer", "alternative answer" },
    { "student", "client" },
    { NULL, NULL }
  };

  const char * d2 [][2] =
  {
    { "fail", "suboptimal result" },
    { "failure", "non-traditional success" },
    { NULL, NULL }
  };

  res = newSpeak ( "Everybody is happy.", d1 );
  assert ( ! strcmp ( res, "Everybody is happy." ) );
  free ( res );

  res = newSpeak ( "He was dumb, his failure was expected.", d1 );
  assert ( ! strcmp ( res, "He was cerebrally challenged, his non-traditional success was expected." ) );
  free ( res );

  res = newSpeak ( "The student answered an incorrect answer.", d1 );
  assert ( ! strcmp ( res, "The client answered an alternative answer." ) );
  free ( res );

  res = newSpeak ( "He was dumb, his failure was expected.", d1 );
  assert ( ! strcmp ( res, "He was cerebrally challenged, his non-traditional success was expected." ) );
  free ( res );

  res = newSpeak ( "The evil teacher became a murderer.", d1 );
  assert ( ! strcmp ( res, "The nicenest deprived voluntary knowledge conveyor became a termination specialist." ) );
  free ( res );

  res = newSpeak ( "Devil's advocate.", d1 );
  assert ( ! strcmp ( res, "Dnicenest deprived's advocate." ) );
  free ( res );

  res = newSpeak ( "Hello.", d2 );
  assert ( ! res );

    const char * tbl [][2] =
    {
    { "son", "offspring with both X and Y chromosomes" },
    { "prisoner", "client of the correctional system" },
    { NULL, NULL }
    };
  res = newSpeak ( "son of a prisoner", tbl );
  free ( res );

    const char * tbl2 [][2] =
    {
    { "answer", "conventional answer" },
    { "wrong answer", "alternative answer" },
    { NULL, NULL }
    };

  res = newSpeak ( "correct answer and wrong answer are opposites", tbl2 );
  free ( res );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
