#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct TResult
{
  struct TResult * m_Next;
  int              m_ID;
  char           * m_Name;
} TRESULT;
#endif /* __PROGTEST__ */

typedef struct TDbElem
{
    int id;
    char * name;
    int rodic1;
    int rodic2;
} TDBELEM;

typedef struct TDatabase
{
    TDBELEM * arr;
    int capacity;
    int len;
} TDATABASE;

void printDB ( TDATABASE * db )
{
    for ( int i = 1; i < db -> len; i ++ )
    {
        printf ( "[%d] %s: r1: %d, r2: %d\n", db -> arr [i] . id, db -> arr [i] . name, db -> arr [i] . rodic1, db -> arr [i] . rodic2 );
    }
}

void printRes ( TRESULT * src )
{
    printf ( "RES ID's:\n" );
    while ( src )
    {
        printf ( "-> %d ", src -> m_ID );
        src = src -> m_Next;
    }
    printf ( "\n==========================\n" );
}

void      freeResult       ( TRESULT         * res )
{
    TRESULT * tmp;
    while ( res )
    {
        tmp = res -> m_Next;
        free ( res -> m_Name );
        free ( res );
        res = tmp;
    }
}

void      initAll          ( TDATABASE       * db )
{
    db -> capacity = 100;
    db -> len = 1;
    db -> arr = ( TDBELEM * ) malloc ( db -> capacity * sizeof ( TDBELEM ) );
    for ( int i = 0; i < db -> capacity; i ++ )
    {
        db -> arr [i] . id = 0;
    }
}

void      doneAll          ( TDATABASE       * db )
{
    for ( int i = 1; i < db -> len; i ++ )
    {
        free ( db -> arr [i] . name );
    }
    free ( db -> arr );
}

void appendDB ( TDATABASE * db )
{
    if ( db -> len == db -> capacity )
    {
        db -> capacity *= 2;
        db -> arr = ( TDBELEM * ) realloc ( db -> arr, db -> capacity * sizeof ( TDBELEM ) );
        for ( int i = db -> capacity / 2; i < db -> capacity; i ++ )
        {
            db -> arr [i] . id = 0;
        }
    }
}

int idFound ( TDATABASE * db, int id, int * pos )
{
    for ( int i = 1; i < db -> len; i ++ )
    {
        if ( db -> arr [i] . id == id )
        {
            * pos = i;
            return 1;
        }
    }
    * pos = 0;
    return 0;
}

int       addPerson        ( TDATABASE       * db,
                             int               id,
                             const char      * name,
                             int               id1,
                             int               id2 )
{
    int pos;
    if ( ( id1 == id2 && id1 != 0 )
        || ! id
        || idFound ( db, id, & pos )
        || ( id1 && ! idFound ( db, id1, & pos ) )
        || ( id2 && ! idFound ( db, id2, & pos ) ) )
        return 0;
    appendDB ( db );
    db -> arr [db -> len] . id = id;

    idFound ( db, id1, & pos );
    db -> arr [db -> len] . rodic1 = pos;
    idFound ( db, id2, & pos );
    db -> arr [db -> len] . rodic2 = pos;
    db -> arr [db -> len] . name = strdup ( name );
    ++ db -> len;
    return 1;
}

TRESULT * getEnd ( TRESULT * src )
{
    while ( src -> m_Next )
        src = src -> m_Next;
    return src;
}

int inResult ( TRESULT * src, int id )
{
    while ( src )
    {
        if ( src -> m_ID == id )
            return 1;
        src = src -> m_Next;
    }
    return 0;
}

TRESULT * ancRec ( TDATABASE * db, int pos )
{
    TRESULT * left = NULL;
    TRESULT * right = NULL;
    TRESULT * current = ( TRESULT * ) malloc ( sizeof ( * current ) );
    TRESULT * ptr = NULL;
    TRESULT * prev = NULL;
    TRESULT * tmp = NULL;
    
    current -> m_ID = db -> arr [pos] . id;
    current -> m_Name = strdup ( db -> arr [pos] . name );

    if ( db -> arr [pos] . rodic1 )
        left = ancRec ( db, db -> arr [pos] . rodic1 );

    if ( db -> arr [pos] . rodic2 )
        right = ancRec ( db, db -> arr [pos] . rodic2 );

    if ( left && right )
    {
        ptr = left;
        prev = NULL;
        while ( ptr && ! inResult ( right, ptr -> m_ID ) )
        {
            prev = ptr;
            ptr = ptr -> m_Next;
        }
        while ( ptr )
        {
            tmp = ptr -> m_Next;
            free ( ptr -> m_Name );
            free ( ptr );
            ptr = tmp;
        }
        if ( prev )
            prev -> m_Next = NULL;
        else
            left = NULL;
    }

    if ( left != NULL )
    {
        TRESULT * lEnd = getEnd ( left );
        lEnd -> m_Next = right;
        current -> m_Next = left;
    } else
        current -> m_Next = right;
    return current;
}

TRESULT * ancestors        ( TDATABASE       * db,
                             int               id )
{
    int pos;
    TRESULT * src = NULL;
    TRESULT * tmp = NULL;
    if ( ! idFound ( db, id, & pos ) )
        return NULL;

    src = ancRec ( db, pos );
    tmp = src ->m_Next;
    free ( src -> m_Name );
    free ( src );
    return tmp;
}

TRESULT * commonAncestors  ( TDATABASE       * db,
                             int               id1,
                             int               id2 )
{
    int pos1, pos2;
    TRESULT * src1 = NULL;
    TRESULT * src2 = NULL;
    if ( ! idFound ( db, id1, & pos1 ) || ! idFound ( db, id1, & pos2 ) )
        return NULL;

    src1 = ancestors ( db, id1 );
    src2 = ancestors ( db, id2 );
    TRESULT * ptr = src1;
    TRESULT * prev = NULL;
    TRESULT * next = NULL;
    while ( ptr )
    {
        if ( ! inResult ( src2, ptr -> m_ID ) )
        {
            next = ptr -> m_Next;
            if ( prev )
                prev -> m_Next = next;
            else
                src1 = next;
            free ( ptr -> m_Name );
            free ( ptr );
            ptr = next;
        } else {
            prev = ptr;
            ptr = ptr -> m_Next;
        }
    }
    freeResult ( src2 );
    return src1;
}

#ifndef __PROGTEST__
int main                   ( int               argc,
                             char            * argv [] )
{
  char      name[100];
  TDATABASE a, b;
  TRESULT * l;

  initAll ( &a );
  assert ( addPerson ( &a, 1, "John", 0, 0 ) == 1 );
  strncpy ( name, "Jane", sizeof ( name ) );
  assert ( addPerson ( &a, 2, name, 0, 0 ) == 1 );
  assert ( addPerson ( &a, 3, "Caroline", 0, 0 ) == 1 );
  assert ( addPerson ( &a, 4, "Peter", 0, 0 ) == 1 );
  assert ( addPerson ( &a, 5, "George", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 6, "Martin", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 7, "John", 3, 4 ) == 1 );
  assert ( addPerson ( &a, 8, "Sandra", 3, 4 ) == 1 );
  assert ( addPerson ( &a, 9, "Eve", 1, 2 ) == 1 );
  assert ( addPerson ( &a, 10, "Douglas", 1, 4 ) == 1 );
  strncpy ( name, "Phillipe", sizeof ( name ) );
  assert ( addPerson ( &a, 11, name, 6, 8 ) == 1 );
  strncpy ( name, "Maria", sizeof ( name ) );
  assert ( addPerson ( &a, 12, name, 5, 8 ) == 1 );
  l = ancestors ( &a, 11 );
  assert ( l );
  assert ( l -> m_ID == 6 );
  assert ( ! strcmp ( l -> m_Name, "Martin" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "John" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  assert ( ancestors ( &a, 3 ) == NULL );
  assert ( ancestors ( &a, 13 ) == NULL );
  l = commonAncestors ( &a, 11, 12 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );

  l = commonAncestors ( &a, 10, 9 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next == NULL );
  freeResult ( l );
  assert ( commonAncestors ( &a, 7, 6 ) == NULL );
  l = commonAncestors ( &a, 7, 10 );
  assert ( l );
  assert ( l -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Name, "Peter" ) );
  assert ( l -> m_Next == NULL );
  freeResult ( l );
  assert ( addPerson ( &a, 13, "Quido", 12, 11 ) == 1 );
  l = ancestors ( &a, 13 );
  assert ( l );
  assert ( l -> m_ID == 12 );
  assert ( ! strcmp ( l -> m_Name, "Maria" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 5 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "George" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 11 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Phillipe" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 6 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Martin" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "John" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  l = commonAncestors ( &a, 9, 12 );
  assert ( l );
  assert ( l -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Name, "John" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next == NULL );
  freeResult ( l );
  assert ( addPerson ( &a, 1, "Francois", 0, 0 ) == 0 );
  initAll ( &b );
  assert ( addPerson ( &b, 10000, "John", 0, 0 ) == 1 );
  assert ( addPerson ( &b, 10000, "Peter", 0, 0 ) == 0 );
  assert ( addPerson ( &b, 20000, "Jane", 10000, 0 ) == 1 );
  assert ( addPerson ( &b, 30000, "Maria", 10000, 10000 ) == 0 );
  assert ( addPerson ( &b, 40000, "Joe", 10000, 30000 ) == 0 );
  assert ( addPerson ( &b, 50000, "Carol", 50000, 20000 ) == 0 );
  assert ( addPerson ( &b, 12, "Maria", 20000, 10000 ) == 1 );
  l = ancestors ( &a, 12 );
  assert ( l );
  assert ( l -> m_ID == 5 );
  assert ( ! strcmp ( l -> m_Name, "George" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 1 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "John" ) );
  assert ( l -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_ID == 2 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Name, "Jane" ) );
  assert ( l -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_ID == 8 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "Sandra" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 3 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Caroline" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_ID == 4 );
  assert ( ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "Peter" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeResult ( l );
  l = ancestors ( &b, 12 );
  assert ( l );
  assert ( l -> m_ID == 20000 );
  assert ( ! strcmp ( l -> m_Name, "Jane" ) );
  assert ( l -> m_Next );
  assert ( l -> m_Next -> m_ID == 10000 );
  assert ( ! strcmp ( l -> m_Next -> m_Name, "John" ) );
  assert ( l -> m_Next -> m_Next == NULL );
  freeResult ( l );
  assert ( ancestors ( &a, 20000 ) == NULL );
  assert ( commonAncestors ( &b, 7, 10 ) == NULL );
  doneAll ( &b );

  doneAll ( &a );

  return 0;
}
#endif /* __PROGTEST__ */
