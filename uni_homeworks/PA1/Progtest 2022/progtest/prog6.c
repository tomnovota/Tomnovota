#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char ** product; // array of product in particular regal
    int product_size;
    int product_len;

    char ** list; // array of list matching with product in particular regal
    int list_size;
    int list_len;
} T_Regal;

typedef struct {
    T_Regal * regal; // array of regals, last regal for not-matching elements on list
    int size;
    int length;
} T_Shop;

int error()
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

// delete arrays of list after proccessing particular list of list
char deleteList ( T_Shop shop )
{ 
    for ( int i = 0; i <= shop.length; i++ )
    {
        for ( int j = 0; j < shop.regal[i].list_len; j++ )
        {
            free ( shop.regal[i].list[j] ); // free all particular list
        }
        free ( shop.regal[i].list ); // free pointer for list in particular regal
    }
    return 0;
}

// delete all regals after finishing program
char deleteShop ( T_Shop shop )
{
    for ( int i = 0; i < shop.length; i++ )
    {
        for ( int j = 0; j < shop.regal[i].product_len; j++ )
        {
            free ( shop.regal[i].product[j] ); // free all particular product
        }
        free ( shop.regal[i].product ); // free pointer for product in particular regal
    }
    free ( shop.regal ); // free pointer for regals
    return 0;
}

// check array of regals and eventually append
void appendShop ( T_Shop * shop )
{
    if (shop->length == shop->size )
    {
        shop->size *= 2;
        shop->regal = ( T_Regal * ) realloc ( shop->regal, shop->size * sizeof ( T_Regal ) );
    }
}

// create array for product in particular regal
void initProduct ( T_Shop * shop )
{
    shop->regal[shop->length].product_size = 100;
    shop->regal[shop->length].product_len = 0;
    shop->regal[shop->length].product = ( char ** ) malloc ( 100 * sizeof ( char * ) );
}

// check if array of product in particular regal is enough, eventually append
void appendProduct ( T_Shop * shop )
{
    if ( shop->regal[shop->length].product_len == shop->regal[shop->length].product_size )
    {
        shop->regal[shop->length].product_size *= 2;
        int size = shop->regal[shop->length].product_size;
        shop->regal[shop->length].product = ( char ** ) realloc ( shop->regal[shop->length].product, size * sizeof ( char * ) );
    }
}

// read line from input and add it in product array, eventually find out the end of product
char addProduct ( T_Shop * shop )
{
    size_t size = 0;
    int items = getline ( &(shop->regal[shop->length].product[shop->regal[shop->length].product_len]), &size, stdin );
    if ( items == -1 ) // end-of-file
    {
        free ( shop->regal[shop->length].product[shop->regal[shop->length].product_len] );
        return 0;
    }
    if ( items == 1 ) // only nextline -> end of product
    {
        free ( shop->regal[shop->length].product[shop->regal[shop->length].product_len] );
        return 2;
    }
    (shop->regal[shop->length].product_len)++;
    return 1;
}

// read all product and add it to matching regals
char readRegal ( T_Shop * shop )
{   
    int num;
    char next_line;
    if ( scanf ( "#%d%c", &num, &next_line ) != 2 )
        return 0;

    while ( num == shop->length )
    {
        appendShop ( shop );
        initProduct ( shop );
        while ( scanf ( "#%d%c", &num, &next_line ) != 2 )
        {
            appendProduct ( shop );
            char situation = addProduct ( shop );
            if ( ! situation )
            {
                (shop->length)++;                            
                return 0;     
            }
            if ( situation == 2 )
            {
                (shop->length)++;            
                appendShop ( shop );
                return 1;     
            }
        }
        (shop->length)++;
    }
    return 0;
}

// initialize arrays of list in all regals
void initList ( T_Shop * shop )
{
    for ( int i = 0; i <= shop->length; i++ )
    {
        shop->regal[i].list_size = 100;
        shop->regal[i].list_len = 0;
        shop->regal[i].list = ( char ** ) malloc ( 100 * sizeof ( char * ) );
    }
}

// check if particular regal have enough place for list
void appendList ( T_Shop * shop, int regal_num )
{
    if ( shop->regal[regal_num].list_len == shop->regal[regal_num].list_size )
    {
        shop->regal[regal_num].list_size *= 2;
        int size = shop->regal[regal_num].list_size;
        shop->regal[regal_num].list = ( char ** ) realloc ( shop->regal[regal_num].list, size * sizeof ( char * ) );
    }
}

// find out if string2 is substring in string1
char compare ( char * string1, char * string2 )
{
    int index;
    int same = 1;
    int string1_len = strlen ( string1 ) - 1; // without newline
    int string2_len = strlen ( string2 );

    if ( string1_len < string2_len )
        return 0;
    for ( int i = 0; i <= (string1_len - string2_len); i++ )
    {
        index = 0;
        same = 1;
        if ( tolower (string1[i]) == tolower ( string2[index] ) ) 
        {
            for ( int j = i + 1; j < i + string2_len; j++ )
            {
                index++;
                if ( tolower (string1[j]) != tolower (string2[index]) )
                {
                    same = 0;
                    break;
                }
            }
            if ( same && string1_len == string2_len)
                return 1;
            if ( same )
                return 2;
        }
    }
    return 0;
}

// insert name of list/product in array list
void insertList ( T_Shop * shop, int regal_num, char * str )
{
    int size = strlen ( str ) + 1;
    shop->regal[regal_num].list[shop->regal[regal_num].list_len] = ( char * ) malloc ( size * sizeof ( char ) );
    strncpy ( shop->regal[regal_num].list[shop->regal[regal_num].list_len], str, size );
    (shop->regal[regal_num].list_len)++;
}

// find matching regal for elemnt on list and insert this name in array list and after that insert name of matching product
// if not foung matching product, insert in last regal where is no product
void addList ( T_Shop * shop, char * str )
{
    int default_regal = shop->length;
    int default_pos = -1;
    for ( int i = 0; i < shop->length; i++ )
    {
        for ( int j = 0; j < shop->regal[i].product_len; j++ )
        {
            char situation = compare ( shop->regal[i].product[j], str );
            if ( situation == 2 && default_regal == shop->length ) // found first only matching product
            {
                default_regal = i;
                default_pos = j;
            }
            if ( situation == 1 ) // found perfect matching product
            {
                appendList ( shop, i );
                insertList ( shop, i, str ); // insert element on list
                insertList ( shop, i, shop->regal[i].product[j] ); // insert name of product
                return;
            }
        }
    }
    appendList ( shop, default_regal );
    insertList ( shop, default_regal, str ); // insert altogether not found product and not perfectly matching product
    if ( default_pos != -1 )
        insertList ( shop, default_regal, shop->regal[default_regal].product[default_pos] ); // for not perfectly matching insert name of product
}

// read all elements on particular list and insert them in correct position
void readList ( T_Shop * shop )
{
    char * tmp = NULL;
    size_t size = 0;
    int items;
    while ( ( items = getline ( &tmp, &size, stdin ) ) != 1 ) // until not empty line on input
    {
        if ( items == -1 ) // end-of-file
        {
            free ( tmp );
            return;
        }
        if ( ! feof ( stdin ) )
            tmp[strlen ( tmp ) - 1] = '\0'; // delete newline from element on list
        addList ( shop, tmp );
        free ( tmp );
        tmp = NULL;
        size = 0;
    }
    free ( tmp ); // prázdná řádka
}

// print result output
void printResult ( T_Shop shop )
{
    int index = 0;
    for ( int i = 0; i < shop.length; i++ )
    {
        for ( int j = 0; j < shop.regal[i].list_len; j += 2 )
        {
            printf ( " %d. %s -> #%d %s", index, shop.regal[i].list[j], i, shop.regal[i].list[j+1] );
            index++;
        }
    }
    int i = shop.length;
    for ( int j = 0; j < shop.regal[i].list_len; j ++ )
        {
            printf ( " %d. %s -> N/A\n", index, shop.regal[i].list[j] );
            index++;
        }
}

int main ( void )
{
    T_Shop shop;
    shop.size = 100;
    shop.length = 0;
    shop.regal = ( T_Regal * ) malloc ( shop.size * sizeof ( *(shop.regal) ) );

    if ( ! readRegal ( &shop ) )
    {
        deleteShop ( shop );
        return error();
    }
    while ( ! feof ( stdin ) )
    {
        initList ( &shop );
        readList ( &shop );
        printf ( "Optimalizovany seznam:\n" );
        printResult ( shop );
        deleteList ( shop );
    }
    deleteShop ( shop );
    return 0;
}