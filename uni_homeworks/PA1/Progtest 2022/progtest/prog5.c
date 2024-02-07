#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MY_DBL 1e-12

typedef struct {
    double pos1;
    double pos2;
    char name [200];
} plane;

typedef struct {
    char plane1 [200];
    char plane2 [200];
} group;

int error ( plane input[], group result[] )
{
    free ( input );
    free ( result );
    printf ("Nespravny vstup.\n");
    return 0;
}

double getDistance ( plane plane1, plane plane2 )
{
    double n1 = ( plane1.pos1 - plane2.pos1 );
    double n2 = ( plane1.pos2 - plane2.pos2 );
    return n1 * n1 + n2 * n2;
}

group getGroup ( char name1 [200], char name2 [200] )
{
    group tmp;
    sscanf ( name1, "%s", tmp.plane1 );
    sscanf ( name2, "%s", tmp.plane2 );
    return tmp;
}

int precise ( double x, double y )
{
    if ( fabs ( x - y ) <= MY_DBL * ( fabs ( x + y ) ) )
        return 1;
    return 0;
}

void addInput ( int * input_count, int * input_size, plane * input[], plane tmp )
{
    (*input_count)++;
    if ( *input_count == *input_size )
    {
        *input_size *= 2;
        *input = ( plane * ) realloc ( *input, *input_size * sizeof ( **input ) );
    }
    (*input)[*input_count] = tmp;
}

void firstResult ( int * result_count, double * min_distance, plane tmp, group result[], plane input[] )
{
    *result_count = 0;
    *min_distance = getDistance ( tmp, input[0] );
    group tmp_group = getGroup ( tmp.name, input[0].name );
    result[0] = tmp_group;
    return;
}

void findResult ( plane tmp, double * min_distance, int * result_count, int * result_size, plane input[], group * result[], int index )
{
    double tmp_distance;
    group tmp_group;
    if ( tmp.pos1 - input[index].pos1 - *min_distance > MY_DBL || tmp.pos2 - input[index].pos2 - *min_distance > MY_DBL )
        return;
    tmp_distance = getDistance ( tmp, input[index] );
    if ( precise ( tmp_distance, *min_distance ) )
    {
        (*result_count)++;
        if ( *result_count == *result_size )
        {
            *result_size *= 2;
            *result = ( group * ) realloc ( *result, *result_size * sizeof ( **result ) );
        }
        tmp_group = getGroup ( tmp.name, input[index].name );
        (*result)[*result_count] = tmp_group;

    }
    else if ( tmp_distance < *min_distance )
    {
        *result_count = 0;
        *min_distance = tmp_distance;
        tmp_group = getGroup ( tmp.name, input[index].name );
        (*result)[0] = tmp_group;
    }
}

int main ( void )
{
    int input_size = 100;
    int result_size = 100;
    plane * input = ( plane * ) malloc ( input_size * sizeof ( *input ) );
    group * result = ( group * ) malloc ( result_size * sizeof ( *result ) );
    int input_count = -1;
    int result_count = -1;
    double min_distance = 0;
    plane tmp;
    printf ( "Pozice letadel:\n" );
    while ( scanf ( "%lf,%lf: %199s ", &tmp.pos1, &tmp.pos2, tmp.name ) == 3 )
    {
        addInput ( &input_count, &input_size, &input, tmp );
        if ( input_count == 0 )
            continue;
        if ( input_count == 1 )
        {
            firstResult ( &result_count, &min_distance, tmp, result, input );
            continue;
        }
        for ( int i = 0; i < input_count; i++ )
            findResult ( tmp, &min_distance, &result_count, &result_size, input, &result, i );
    }
    if ( ! feof(stdin) || input_count < 1 )
        return error( input, result );
    printf ( "Vzdalenost nejblizsich letadel: %lf\n", sqrt ( min_distance ) );
    printf ( "Nalezenych dvojic: %d\n", result_count + 1 );
    for ( int i = 0; i <= result_count; i++ )
        printf ( "%s - %s\n", result[i].plane2, result[i].plane1 );
    free ( input );
    free ( result );
    return 0;
}