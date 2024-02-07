#include <stdio.h>
#include <stdlib.h>

/** count: stores information how many intervals sum to @sum
  * combi: factorial of count devided with 2
  */
typedef struct {
    int sum;
    int count;
    int combi;
} pair;

/** sum: sums all elements from start to num in sequence
  */
typedef struct {
    int num;
    int sum;
} element;

/** @param arr1 Is an array we want to free
  * @param arr2 Is an array we want to free
  * @return 0 as an end of the program when error occured
  */
int error ( element * arr2, pair * arr1 )
{
    free ( arr1 );
    free ( arr2 );
    printf ( "Nespravny vstup.\n" );
    return 0;
}

/** This function adds and element to the array seq and calculate sum of all elements before
  * @param seq Is pointer to an array with sequence of numbers
  * @param seq_size Is pointer to value with length of the array seq
  * @param seq_elem_count Is pointer to value with currently used length of the array seq
  * @param num Is value we want to add to the array seq
  */
void addSeq ( element * seq [], int * seq_size, int * seq_elem_count, int num )
{
    if ( ! *seq_size )
    {
        *seq_size = 50;
        *seq = (element *) malloc ( (*seq_size) * sizeof (**seq) );
    }        
    if ( *seq_elem_count == *seq_size )
    {
        *seq_size *= 2;
        *seq = (element *) realloc ( *seq, (*seq_size) * sizeof (**seq) );
    }
    (*seq)[*seq_elem_count].num = num;
    (*seq)[*seq_elem_count].sum = *seq_elem_count ? num + (*seq)[(*seq_elem_count)-1].sum : num;
}

/** This function stores particular sum of interval into the array sums
  * @param sums Is pointer to the array where the information about sums of intervals and their quantity is stored
  * @param size Is pointer to value with capacity of the array sums
  * @param elem_count Is pointer to value what length of the array sums is currently used
  * @param sum Is the particular value of sum that is stored in array sums
  */
void addSum ( pair * sums [], int * size, int * elem_count, int sum )
{
    if ( ! *size )
    {
        *size = 50;
        *sums = (pair *) malloc ( (*size) * sizeof (**sums) );
    }
    if ( *elem_count == *size )
    {
        *size *= 2;
        *sums = (pair *) realloc ( *sums, (*size) * sizeof (**sums) );
    }
    for ( int i = 0; i < *elem_count; i++ )
    {
        if ( (*sums)[i].sum == sum )
        {
            (*sums)[i].count++;
            (*sums)[i].combi *= (*sums)[i].count;
            return;
        }
    }
    (*sums)[*elem_count].sum = sum;
    (*sums)[*elem_count].count = 1;
    (*sums)[*elem_count].combi = 1;
    (*elem_count)++;
}

/** This function calculates different sums of intervals in sequence 
  *    for given position in an array seq and stores them in an array sums
  * @param sums Is pointer to the array where the information about sums of intervals and their quantity is stored
  * @param size Is pointer to value with capacity of the array sums
  * @param elem_count Is pointer to value what length of the array sums is currently used
  * @param seq Is the array with sequence of numbers
  * @param seq_size Is used length of array seq
  */
void getSums ( pair * sums [], int * size, int * elem_count, element seq [], int seq_size )
{
    if ( seq_size < 1 )
        return;
    int tmp_sum = seq[seq_size].sum;
    addSum ( sums, size, elem_count, tmp_sum );
    for ( int i = 0; i < seq_size - 1; i++ )
    {
        tmp_sum -= seq[i].num;
        addSum ( sums, size, elem_count, tmp_sum );
    }
}

int main ( void )
{
    int num, seq_size = 0, seq_elem_count = 0;
    int sums_size = 0, sums_elem_count = 0;
    int pair_count = 0;
    element * seq = NULL;
    pair * sums = NULL;

    printf( "Posloupnost:\n" );
    while ( scanf ( " %d", &num ) == 1 )
    {
        addSeq ( &seq, &seq_size, &seq_elem_count, num );
        if ( seq_elem_count == 2000 )
            return error( seq, sums );
        seq_elem_count++;
    }

    for ( int i = 0; i < seq_elem_count; i++ )
        getSums ( &sums, &sums_size, &sums_elem_count, seq, i );

    if ( ! feof ( stdin ) || ! seq_elem_count )
        return error( seq, sums );
    
    // calculates combination number: (quantity of particular sum, 2)
    for ( int i = 0; i < sums_elem_count; i++ )
        pair_count += sums[i].count > 1 ? (sums[i].count == 2 ? 1 : (sums[i].combi / 2 / (sums[i].count - 2))) : 0;
    
    printf( "Pocet dvojic: %d\n", pair_count );
    free ( seq );
    free ( sums );
    return 0;
}