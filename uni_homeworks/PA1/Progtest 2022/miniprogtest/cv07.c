#include <stdio.h>
#include <stdlib.h>

#define MAX_COUNT 26

typedef struct {
    char name;
    int votes;
    double preference;
    int hornik_count;
} party;

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int inArr ( party arr[MAX_COUNT], char name, int size )
{
    for ( int i = 0; i < size; i++ )
    {
        if ( arr[i].name == name )
            return 1;
    }
    return 0;
}

int compare_party ( const void * a, const void * b )
{
    const party * x = ( const party * ) a;
    const party * y = ( const party * ) b;
    return ( x->name > y->name ) - ( x->name < y->name );
}

int samePref ( party arr[MAX_COUNT], int size )
{
    int count = 1;
    int i = 0;
    while ( i + 1 < size && arr[i].preference == arr[i+1].preference )
    {
        count++;
        i++;
    }
    return count;
}

void printArr ( party arr[MAX_COUNT], int size )
{
    for ( int i = 0; i < size; i++ )
    {
        printf("%c: v=%d, p=%g, h=%d\n", arr[i].name, arr[i].votes, arr[i].preference, arr[i].hornik_count);
    }
    printf("===============\n");
}

int main ( void )
{
    int party_count, tmp_votes, hornik_count;
    char tmp_name;
    party arr [MAX_COUNT];
    printf("Pocet stran:\n");
    if ( scanf( "%d", &party_count ) != 1
        || party_count < 2 || party_count > MAX_COUNT )
        return error();

    printf("Strany a pocet hlasu:\n");
    for ( int i = 0; i < party_count; i++ )
    {
        if ( scanf ( " %c %d", &tmp_name, &tmp_votes ) != 2
            || tmp_name < 'A'
            || tmp_name > ( 'A' + party_count - 1 )
            || inArr( arr, tmp_name, i )
            || tmp_votes < 1 )
            return error();

        int j = 0;
        while ( j < i && arr[j].votes > tmp_votes )
            j++;
        int k = i;
        while ( j < k )
        {
            arr[k] = arr[k-1];
            k--;
        }
        arr[j].name = tmp_name;
        arr[j].votes = tmp_votes;
        arr[j].preference = (double) tmp_votes / 1.42;
        arr[j].hornik_count = 0;
    }
    //printArr( arr, party_count );

    printf("Pocet horniku:\n");
    if ( scanf ( "%d", &hornik_count ) != 1
        || hornik_count < 0 )
        return error();
        
    for ( int i = 0; i < hornik_count; i++ )
    {
        if ( hornik_count - i < samePref( arr, party_count ))
        {
            printf("i = %d, hornik = %d\n", i, hornik_count);
            printArr( arr, party_count );
            printf("Nelze rozdelit.\n");
            return 0;
        }
        arr[0].hornik_count++;
        arr[0].preference = arr[0].votes / ( 1 + arr[0].hornik_count );
        party tmp;
        int j = 0;
        while ( j + 1 < party_count && arr[j].preference < arr[j+1].preference )
        {
            tmp = arr[j+1];
            arr[j+1] = arr[j];
            arr[j] = tmp;
            j++;
        }        
        //printArr( arr, party_count );
    }
        
    qsort( arr, party_count, sizeof ( party ), compare_party );

    printf("Pridelene pocty:\n");
    for ( int i = 0; i < party_count; i++ )
    {
        printf( "%c: %d\n", arr[i].name, arr[i].hornik_count );
    }
    return 0;
}