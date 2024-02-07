#include <stdio.h>
#include <float.h>
#include <math.h>

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int precise ( double x, double y )
{
    if ( fabs(x - y) <= (DBL_EPSILON * fabs(x + y)) )
        return 1;
    return 0;
}

double special( double x )
{
    if ( x > 0 )
    {
        if ( precise( x, ceil(x) ) )
            return ceil(x);
        return floor(x);
    }
    if ( precise( x, floor(x) ) )
        return floor(x);
    return ceil(x);
}

int notIn(char c, double a, double b, double * x)
{
    switch (c)
    {
        case '+':
            *x = a + b;
            return 0;
        case '-':
            *x = a - b;
            return 0;
        case '*':
            *x = a * b;
            return 0;
        case '/':
            *x = special(a / b);
            return 0;
        default:
            return 1;        
    }
}

int input( double * x, double * y )
{
    double a, b, c;
    char o, p;
    if ( scanf( " %lf %c %lf %c %lf", &a, &o, &b, &p, &c ) != 5
        || ( o == '/' && b == 0)
        || notIn(o, a, b, x) || p != '=' )
        return 0;
    * y = c;
    return 1;
}

int main (void)
{
    double a, b;
    printf( "Zadejte rovnici:\n" );
    if ( ! input(&a, &b) )
        return error();
    if ( precise( a, b ) )
    {
        printf( "Rovnice je spravne.\n" );
        return 0;
    }
    printf( "%g != %g\n", a, b );
    return 0;
}