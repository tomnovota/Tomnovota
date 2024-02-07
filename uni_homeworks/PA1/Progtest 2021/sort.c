#include <stdio.h>
#include <stdlib.h>

typedef struct T_ELEM
{
    int num;
    char sym;
} T_Elem;

void swapInt( T_Elem * arr, int i )
{
    int tmp = arr[i].num;
    arr[i].num = arr[i-1].num;
    arr[i-1].num = tmp;
    
}

void swapSym( T_Elem * arr, int i )
{
    char tmp = arr[i].sym;
    arr[i].sym = arr[i-1].sym;
    arr[i-1].sym = tmp;
    
}

void bubble_sort ( T_Elem * arr, int size )
{
    int change;
    do
    {
        change = 0;
        for ( int i = 1; i < size; i++ )
        {
            if ( arr[i-1].num > arr[i].num )
            {
                swapInt( arr, i );
                change = 1;
            }
            if ( arr[i-1].sym > arr[i].sym )
            {
                swapSym( arr, i );
                change = 1;
            }
        }
        size--;
    } while ( change );
}

void coctail_sort ( T_Elem * arr, int size )
{
    int change;
    int start = 1;
    do
    {
        change = 0;
        for ( int i = start; i < size; i++ )
        {
            if ( arr[i-1].num > arr[i].num )
            {
                swapInt( arr, i );
                change = 1;
            }
            if ( arr[i-1].sym > arr[i].sym )
            {
                swapSym( arr, i );
                change = 1;
            }
        }
        for ( int i = size - 2; i >= start; i-- )
        {
            if ( arr[i-1].num > arr[i].num )
            {
                swapInt( arr, i );
                change = 1;
            }
            if ( arr[i-1].sym > arr[i].sym )
            {
                swapSym( arr, i );
                change = 1;
            }
        }
        start++;
        size--;
    } while ( change );
}

void swapInt2( T_Elem * x1, T_Elem * x2 )
{
    int tmp = x1->num;
    x1->num = x2->num;
    x2->num = tmp;   
}

void swapSym2( T_Elem * x1, T_Elem * x2 )
{
    char tmp = x1->sym;
    x1->sym = x2->sym;
    x2->sym = tmp;   
}

void select_sort ( T_Elem * arr, int size )
{
    for ( int i = 0; i < size - 1; i++ )
    {
        int iminNum = i;
        int iminSym = i;
        for ( int j = i + 1; j < size; j++ )
        {
            if ( arr[j].num < arr[iminNum].num )
            {
                iminNum = j;
            }
            if ( arr[j].sym < arr[iminSym].sym )
            {
                iminSym = j;
            }
        }
        if ( arr[iminNum].num < arr[i].num )
            swapInt2( &arr[iminNum], &arr[i] );
        if ( arr[iminSym].sym < arr[i].sym )
            swapSym2( &arr[iminSym], &arr[i] );
    }
}

void insertNum( T_Elem * arr, int k, T_Elem x )
{
    int n = k - 1;
    for ( ; n >= 0 && arr[n].num > x.num; n-- )
    {
        arr[n+1].num = arr[n].num;
    }
    arr[n+1].num = x.num;
}

void insertSym( T_Elem * arr, int k, T_Elem x )
{
    int n = k - 1;
    for ( ; n >= 0 && arr[n].sym > x.sym; n-- )
    {
        arr[n+1].sym = arr[n].sym;
    }
    arr[n+1].sym = x.sym;
}

void insert_sort ( T_Elem * arr, int size )
{
    for ( int k = 1; k < size; k++ )
    {
        insertNum( arr, k, arr[k] );
        insertSym( arr, k, arr[k] );
    }
}

int compareNum ( const void * a, const void * b )
{
    const T_Elem * x = ( const T_Elem * ) a;
    const T_Elem * y = ( const T_Elem * ) b;
    return ( x->num > y->num ) - ( x->num < y->num );
}

int compareSym ( const void * a, const void * b )
{
    const T_Elem * x = ( const T_Elem * ) a;
    const T_Elem * y = ( const T_Elem * ) b;
    return ( x->sym > y->sym ) - ( x->sym < y->sym );
}

void merge_sort ( T_Elem * arr, int size )
{
}

void arrInit ( T_Elem * arr, int size )
{    
    char alphabet [] = "xadbclefghizjrkmtnopsuvwy";
    for ( int i = 0; i < size; i++ )
    {
        arr[i].num = rand() % 100;
        arr[i].sym = alphabet[i];
    }
}

void arrPrint ( T_Elem * arr, int size )
{
    printf( "Ciselna rada: " );
    for ( int i = 0; i < size; i++ )
    {
        printf( "%d ", arr[i].num );
    }
    printf( "\n" );
    printf( "Znakova rada: " );
    for ( int i = 0; i < size; i++ )
    {
        printf( "%c ", arr[i].sym );
    }
    printf( "\n" );
}

void arrCopy( T_Elem * arr, T_Elem * arr_copy, int size )
{
    for ( int i = 0; i < size; i++ )
    {
        arr_copy[i].num = arr[i].num;
        arr_copy[i].sym = arr[i].sym;
    }
}

int arrSame ( T_Elem * arr1, T_Elem * arr2, int size )
{
    int same = 1;
    for ( int i = 0; i < size; i++ )
    {
        if ( arr1[i].num != arr2[i].num || arr1[i].sym != arr2[i].sym )
            {
                same = 0;
                printf( "Nejsou stejne!\n" );
                return same;
            }
    }
    printf( "Jsou stejne.\n" );
    return same;
}

int main ( void )
{
    int size = 20;
    T_Elem * arr = ( T_Elem * ) malloc ( size * sizeof( *arr ) );
    T_Elem * arr_copy = ( T_Elem * ) malloc ( size * sizeof( *arr_copy ) );
    T_Elem * arr_copy2 = ( T_Elem * ) malloc ( size * sizeof( *arr_copy2 ) );
    arrInit( arr, size );
    arrPrint ( arr, size );

    arrCopy( arr, arr_copy, size );
    bubble_sort( arr_copy, size );

    printf( "%s: ", "Coctail: " );
    arrCopy( arr, arr_copy2, size );
    coctail_sort( arr_copy2, size );
    arrSame( arr_copy, arr_copy2, size );

    printf( "%s: ", "Select: " );
    arrCopy( arr, arr_copy2, size );
    select_sort( arr_copy2, size );
    arrSame( arr_copy, arr_copy2, size );

    printf( "%s: ", "Insert: " );
    arrCopy( arr, arr_copy2, size );
    insert_sort( arr_copy2, size );
    arrSame( arr_copy, arr_copy2, size );

    printf( "%s: ", "Merge: " );
    arrCopy( arr, arr_copy2, size );
    merge_sort( arr_copy2, size );
    arrSame( arr_copy, arr_copy2, size );

    printf( "%s: \n", "Quick" );
    arrCopy( arr, arr_copy2, size );
    qsort( arr_copy2, size, sizeof(T_Elem), compareNum );
    arrPrint( arr_copy2, size );
    arrCopy( arr, arr_copy2, size );
    qsort( arr_copy2, size, sizeof(T_Elem), compareSym );
    arrPrint( arr_copy2, size );

    int findNum = 92;
    T_Elem * pos = (T_Elem *) bsearch( &findNum, arr_copy, size, sizeof(*arr_copy), compareNum );
    if ( pos-arr_copy < 0 )
        printf( "Prvek %d nenalezen.\n", findNum );
    else
        printf( "Prvek %d nalezen na pozici %ld.\n", findNum, pos-arr_copy );

    free( arr );
    free( arr_copy );
    free( arr_copy2 );
    return 0;
}