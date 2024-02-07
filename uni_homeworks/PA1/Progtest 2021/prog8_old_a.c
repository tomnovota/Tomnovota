#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TEmployee
{
  struct TEmployee         * m_Next;
  struct TEmployee         * m_Bak;
  char                     * m_Name;
} TEMPLOYEE;

#endif /* __PROGTEST__ */

void printEmployee ( TEMPLOYEE * src )
{
    for ( int i = 0; src; src = src -> m_Next, i ++ )
    {
        if ( src -> m_Bak != NULL )
            printf ( "[%d] %s -> %s\n", i, src -> m_Name, src -> m_Bak -> m_Name );
        else
            printf ( "[%d] %s -> ///\n", i, src -> m_Name );
    }

}

void               freeList     ( TEMPLOYEE       * src )
{
    TEMPLOYEE * tmp;
    while ( src )
    {
        tmp = src -> m_Next;
        free ( src -> m_Name );
        free ( src );
        src = tmp;
    }
}

TEMPLOYEE        * newEmployee  ( const char      * name,
                                  TEMPLOYEE       * next )
{
    TEMPLOYEE * newNode = ( TEMPLOYEE * ) malloc ( sizeof ( * newNode ) );
    newNode -> m_Next = next;
    newNode -> m_Bak = NULL;
    newNode -> m_Name = strdup ( name );
    return newNode;
}

int getLen ( TEMPLOYEE * src )
{
    int size = 0;
    while ( src )
    {
        src = src -> m_Next;
        size ++;
    }
    return size;
}

int getPos ( TEMPLOYEE * bak, TEMPLOYEE * src )
{
    int pos = 0;
    while ( src && src != bak )
    {
        src = src -> m_Next;
        pos ++;
    }
    return pos;
}

TEMPLOYEE * copyEmployee ( const char * name, TEMPLOYEE * next )
{
    TEMPLOYEE * newNode = ( TEMPLOYEE * ) malloc ( sizeof ( * newNode ) );
    newNode -> m_Next = next;
    newNode -> m_Bak = NULL; // temporarily
    newNode -> m_Name = strdup ( name );
    return newNode;
}

TEMPLOYEE * copyRec ( TEMPLOYEE * src, int pos, int map [], TEMPLOYEE * orig )
{
    if ( ! src )
        return NULL;
    if ( src -> m_Bak != NULL )
        map [pos] = getPos ( src -> m_Bak, orig );
    else
        map [pos] = -1;
    return copyEmployee ( src -> m_Name, copyRec ( src -> m_Next, ++ pos, map, orig ) );
}

TEMPLOYEE * getBak ( int pos, TEMPLOYEE * orig )
{
    for ( int i = 0; i < pos; i ++ )
    {
        orig = orig -> m_Next;
    }
    printf ( "BAK[%d] %s\n", pos, orig -> m_Name );
    return orig;
}

void fillBak ( TEMPLOYEE * src, int map [], int len, TEMPLOYEE * orig )
{
    for ( int i = 0; i < len; i ++ )
    {
        if ( map [i] != -1 )
        {
            src -> m_Bak = getBak ( map[i], orig );
        }
        src = src -> m_Next;
    }
}

TEMPLOYEE        * cloneList    ( TEMPLOYEE       * src )
{
    int len = getLen ( src );
    int * map = ( int * ) malloc ( len * sizeof ( * map ) );
    TEMPLOYEE * copy = copyRec ( src, 0, map, src );
    printEmployee ( copy );
    for ( int i = 0; i < len; i ++ )
        printf ( "map[%d] %d\n", i, map[i] );
    fillBak ( copy, map, len, copy );
    free ( map );
    return copy;
}

#ifndef __PROGTEST__
int                main         ( int               argc, 
                                  char            * argv [] )
{
  TEMPLOYEE * a, *b;
  char tmp[100];

  assert ( sizeof ( TEMPLOYEE ) == 3 * sizeof ( void * ) );
  a = NULL;
  a = newEmployee ( "Peter", a );
  a = newEmployee ( "John", a );
  a = newEmployee ( "Joe", a );
  a = newEmployee ( "Maria", a );
  a -> m_Bak = a -> m_Next;
  a -> m_Next -> m_Next -> m_Bak = a -> m_Next -> m_Next -> m_Next;
  a -> m_Next -> m_Next -> m_Next -> m_Bak = a -> m_Next;
  assert ( a
           && ! strcmp ( a -> m_Name, "Maria" )
           && a -> m_Bak == a -> m_Next );
  assert ( a -> m_Next
           && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
           && a -> m_Next -> m_Bak == NULL );
  assert ( a -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
           && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  b = cloneList ( a );
  strncpy ( tmp, "Moe", sizeof ( tmp ) );
  a = newEmployee ( tmp, a );
  strncpy ( tmp, "Victoria", sizeof ( tmp ) );
  a = newEmployee ( tmp, a );
  strncpy ( tmp, "Peter", sizeof ( tmp ) );
  a = newEmployee ( tmp, a );
  b -> m_Next -> m_Next -> m_Next -> m_Bak = b -> m_Next -> m_Next;
  assert ( a
           && ! strcmp ( a -> m_Name, "Peter" )
           && a -> m_Bak == NULL );
  assert ( a -> m_Next
           && ! strcmp ( a -> m_Next -> m_Name, "Victoria" )
           && a -> m_Next -> m_Bak == NULL );
  assert ( a -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "Moe" )
           && a -> m_Next -> m_Next -> m_Bak == NULL );
  assert ( a -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Maria" )
           && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Joe" )
           && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == NULL );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" )
           && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );

    printEmployee ( a );
    printEmployee ( b );
  assert ( b );
  assert ( ! strcmp ( b -> m_Name, "Maria" ) );
  
  assert ( b -> m_Bak == b -> m_Next );
  assert ( b
           && ! strcmp ( b -> m_Name, "Maria" )
           && b -> m_Bak == b -> m_Next );
  assert ( b -> m_Next
           && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
           && b -> m_Next -> m_Bak == NULL );
  assert ( b -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
           && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeList ( a );
  b -> m_Next -> m_Bak = b -> m_Next;
  a = cloneList ( b );
  assert ( a
           && ! strcmp ( a -> m_Name, "Maria" )
           && a -> m_Bak == a -> m_Next );
  assert ( a -> m_Next
           && ! strcmp ( a -> m_Next -> m_Name, "Joe" )
           && a -> m_Next -> m_Bak == a -> m_Next );
  assert ( a -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Name, "John" )
           && a -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next
           && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && a -> m_Next -> m_Next -> m_Next -> m_Bak == a -> m_Next -> m_Next );
  assert ( a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  assert ( b
           && ! strcmp ( b -> m_Name, "Maria" )
           && b -> m_Bak == b -> m_Next );
  assert ( b -> m_Next
           && ! strcmp ( b -> m_Next -> m_Name, "Joe" )
           && b -> m_Next -> m_Bak == b -> m_Next );
  assert ( b -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Name, "John" )
           && b -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next
           && ! strcmp ( b -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" )
           && b -> m_Next -> m_Next -> m_Next -> m_Bak == b -> m_Next -> m_Next );
  assert ( b -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeList ( b );
  freeList ( a );
  return 0;
}
#endif /* __PROGTEST__ */
