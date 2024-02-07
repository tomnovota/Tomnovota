#include <stdio.h>
#include <math.h>

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int getSize( int * a )
{
    if ( scanf(" %d", a) != 1
        || *a < 1 )
        return 0;
    return 1;
}

int tooClose ( int a, int b, int c, int r )
{
    return 0;
}

int getInput( int * a, int * b, int * c, int r )
{
    if ( scanf(" %d %d %d", a, b, c) != 3
        || *a < 0 || *b < 0 || *c < 0
        || *a > r || *b > r || *c > r 
        || tooClose(*a, *b, *c, r) )
        return 0;
    return 1;
}

void getRes ( int a, int b, int c, int d, int e, int f, char * res )
{
    res[0] = fabs( a - d ) + fabs( b - e ) + fabs( c - f );
}

int main( void )
{
    int r, a, b, c, d, e, f;
    char res [2];
    printf("Rozmer mistnosti:\n");
    if ( ! getSize(&r) )
        return error();
    printf("Bod #1:\n");
    if ( ! getInput(&a, &b, &c, r) )
        return error();
    printf("Bod #2:\n");
    if ( ! getInput(&d, &e, &f, r) )
        return error();
    getRes(a, b, c, d, e, f, res);
    printf("Delka potrubi: %d\n", res[0]);
    //printf("Delka hadice: %g\n", res[1]);

    return 0;
}