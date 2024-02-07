#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */

/** fills vzorek [] with the string long as vzor [] and if not the same, moves pointer by 1 to the right if makes sense
  * returns [1] if vzor [] was found in string str2
  * returns [0] if all possible vzorek [] where compared with vzor [] and not the same
  */

int find ( char * vzor, const char * str2, int size )
{
    int lenght = strlen ( str2 );
    for ( int i = 0; i < ( lenght - size + 1 ); i++ )
    {
        if ( tolower(vzor[0]) == tolower(str2[i]) )
        {
            int j = 1;
            while ( j < size && tolower (vzor [j]) == tolower (str2[i+j]) )
                j++;
            if ( j == size )
                return 1;
        }
    }
    return 0;
}

/** loads current word into tmp [] and sends it to find ()
  * returns [1] if the current word was found in the second string
  * returns [0] if the current word was not found in the second string
  */

int words ( int * pozice, const char * str1, const char * str2 )
{
    int i = * pozice;
    int size = 0;
    int max_size = 4;
    char * tmp = (char * ) malloc ( max_size );

    while ( isalpha ( str1 [i] ) ) // in this cycle, tmp is filled with the current word
    {
        if ( size >= ( max_size - 1 ) )
        {
            max_size *= 2;
            tmp = ( char * ) realloc ( tmp, max_size );
        }
        tmp [size++] = str1 [i];
        i++;
    }
    tmp [size] = '\0';
    if ( ! find ( tmp, str2, size ) )
    {
        free ( tmp );
        return 0;
    }
    free ( tmp );
    * pozice = i;
    return 1;
}

/** returns [1] if all words from the first string given are found in the second string given
  * returns [0] if not
  */

int getwords ( const char * str1, const char * str2 )
{
    int i = 0;
    while ( 1 )
    {
        while ( ! isalpha ( str1[i] ) && str1 [i] != '\0' ) // skips white-spaces
                i++;
        if ( str1 [i] == '\0') // checks if the string ends
            return 1;
        if ( ! words ( &i, str1, str2 ) ) // str[i] is now a character for sure, words() will work with it
            return 0;
    }
}

int sameWords ( const char * a, const char * b )
{
    return ( getwords( a, b ) && getwords ( b, a ) );
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  assert ( sameWords ( "ahoj", "ahoj" ) == 1 );
  assert ( sameWords ( " He said 'hello!'", "'Hello.' he   said." ) == 1 );
  assert ( sameWords ( "Hello students.", "HELLO studEnts!" ) == 1 );
  assert ( sameWords ( "He said he would do it.", "IT said: 'He would do it.'" ) == 1 );
  assert ( sameWords ( "one two three", "one two five" ) == 0 );

  return 0;
}
#endif /* __PROGTEST__ */

