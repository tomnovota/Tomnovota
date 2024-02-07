#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TItem
{
    struct TItem * m_Next;
    char * m_Name;
    char m_Secret[24];
} TITEM;

#endif /* __PROGTEST__ */

void printList ( TITEM * l )
{
    while ( l )
    {
        printf ( "%s\n", l->m_Name );
        l = l->m_Next;
    }

}

void freeList ( TITEM * src )
{
    TITEM * tmp;
    while ( src )
    {
        tmp = src->m_Next;
        if ( src->m_Name )
            free ( src->m_Name );
        free ( src );
        src = tmp;
    }
}

TITEM * newItem ( const char * name, TITEM * next )
{
    TITEM * item = ( TITEM * ) malloc ( sizeof ( *item ) );
    
    if ( name != NULL )
    {
        int size = strlen ( name ) + 1;
        item->m_Name = ( char * ) malloc ( size * sizeof ( char ) );
        strcpy ( item->m_Name, name );
    } else {
        item->m_Name = NULL;
    }

    for ( int i = 0; i < 24; i++ )
        item->m_Secret[i] = '\0';
    item->m_Next = next;
    return item;
}

void splitList ( TITEM ** left, TITEM ** right, TITEM * origin )
{
    TITEM * l = origin;
    TITEM * r = origin->m_Next;

    while ( r != NULL )
    {
        r = r->m_Next; // r goes 2x faster
        if ( r != NULL )
        {
            l = l->m_Next;
            r = r->m_Next;
        }
    }
    *left = origin;
    *right = l->m_Next; // right starts at half
    l->m_Next = NULL; // end of left half
}

TITEM * merge ( TITEM * left, TITEM * right, int asc )
{
    TITEM * sorted = NULL; // head element of sorted list

    if ( left == NULL ) // left side empty
        return right;
    if ( right == NULL ) // right side empty
        return left;
    int cmp;

    if ( left->m_Name != NULL && right->m_Name != NULL )
    {
        cmp = strcmp ( left->m_Name, right->m_Name );
    } else { // if some string is NULL pointer
        sorted = right->m_Name == NULL ? left : right;
        sorted->m_Next = right->m_Name == NULL ? merge ( left->m_Next, right, asc ) : merge ( left, right->m_Next, asc );
        return sorted;
    }

    if ( (cmp <= 0 && asc == 1) || (cmp >= 0 && asc == 0) ) // asc: left <= right, desc: left >= right
    {
        sorted = left;
        sorted->m_Next = merge ( left->m_Next, right, asc );
    } else {                                           // asc: right > left
        sorted = right;
        sorted->m_Next = merge ( left, right->m_Next, asc );
    }
    return sorted;
}

// recursive function to sort array by merge sort
TITEM * sortList ( TITEM * l,  int ascending )
{
    if ( l == NULL ) // no list
        return l;
    if ( l->m_Next == NULL ) // one element
        return l;
    
    TITEM * left;
    TITEM * right;
    splitList ( &left, &right, l ); // gets left side and right side to be sorted
    // returns 2 sorted halfs
    left = sortList ( left, ascending );
    right = sortList ( right, ascending );
    TITEM * my = merge ( left, right, ascending );
    return my;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  TITEM * l;
  char tmp[50];

    l = newItem ( NULL, NULL );
    l = newItem ( NULL, l );
    sortList (l, 1);
    freeList (l);
    return 0;

  assert ( sizeof ( TITEM ) == sizeof ( TITEM * ) + sizeof ( char * ) + 24 * sizeof ( char ) );
  l = NULL;
  l = newItem ( "PA1", l );
  l = newItem ( "PA2", l );
  l = newItem ( "UOS", l );
  l = newItem ( "LIN", l );
  l = newItem ( "AG1", l );

  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 1 );
  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = newItem ( "programming and algorithmic I", l );
  l = newItem ( "AAG", l );
  assert ( l
           && ! strcmp ( l -> m_Name, "AAG" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 0 );
  assert ( l
           && ! strcmp ( l -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AAG" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeList ( l );
  l = NULL;
  strncpy ( tmp, "PA1", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "PA2", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "UOS", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "LIN", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "AG1", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 1 );
  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  strncpy ( tmp, "programming and algorithmic I", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "AAG", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  assert ( l
           && ! strcmp ( l -> m_Name, "AAG" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 0 );
  assert ( l
           && ! strcmp ( l -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AAG" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeList ( l );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
