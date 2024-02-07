#include <stdio.h>

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int input( double * u )
{
    double x;
    if ( scanf(" %lf", &x) != 1
        || x < 0 )
        return 0;
    *u = x;
    return 1;
}

double myRound ( double x )
{
    return ((int)( x * 100 )) / (double)100;
}

void newBalance( double * balance, double u, int days, int deposit )
{
    double urok;
    for ( int i = 0; i < days; i++ )
    {
        urok = *balance * u / 100;
        *balance = myRound(*balance + urok);
    }
    *balance += deposit;
}

int trans( double * balance, double ku, double du )
{
    int day, deposit, pre_day = -1, days;
    double curr_u;
    while ( scanf( " %d, %d", &day, &deposit ) == 2
        && day > pre_day )
    {
        pre_day = pre_day == -1 ? 0 : pre_day;
        curr_u = *balance < 0 ? du : ku;
        days = day - pre_day;
        pre_day = day;
        newBalance( balance, curr_u, days, deposit );
        if ( ! deposit )
            return 1;
    }
    if ( ! feof(stdin) )
        return 0;
    return 1;
}

int main( void )
{
    double ku, du, balance = 0;
    printf("Zadejte kreditni urok [%%]:\n");
    if ( ! input( &ku ) )
        return error();
    printf("Zadejte debetni urok [%%]:\n");
    if ( ! input( &du ) )
        return error();
    printf("Zadejte transakce:\n");
    if ( ! trans( &balance, ku, du ) )
        return error();
    printf("Zustatek: %.2f\n", balance);
    return 0;
}