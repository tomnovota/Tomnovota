#include <stdio.h>
#include <math.h>

#define DBL 1e-13

typedef struct {
    double x;
    double y;
} vector;

int error( void )
{
    printf("Nespravny vstup.\n");
    return 0;
}

double size ( vector u )
{
    return ( u.x*u.x + u.y*u.y );
}

int nacteni ( vector * v )
{
    double x, y;
    if ( scanf( " %lf %lf", &x, &y ) != 2 )
        return 0;
    v->x = x;
    v->y = y;
    return 1;
}

int splyvani( vector u, vector v )
{
    if ( (u.x == 0 && u.y == 0) 
        || (v.x == 0 && v.y == 0) 
        || (u.x == v.x && u.y == v.y) )
        return 1;
    return 0;
}

vector getVec ( vector r, vector s )
{
    vector tmp;
    tmp.x = s.x - r.x;
    tmp.y = s.y - r.y;
    return tmp;
}

int precise ( double x, double y )
{
    if ( fabs(x - y) <= (DBL * fabs(x + y)) )
        return 1;
    return 0;
}

int nuly ( vector u, vector v, char * c )
{
    if ( precise( v.x, 0 ) )
    {
        if ( ! precise( u.x, v.x ) )
            return 0;
        if ( u.y * v.y < 0 )
        {
            *c = 'A';
            return 1;
        }
        *c = size(u) > size(v) ? 'C' : 'B';
        return 1;
    }
    if ( precise( v.y, 0 ) )
    {
        if ( ! precise( u.y, v.y ) )
            return 0;
        if ( u.x * v.x < 0 )
        {
            *c = 'A';
            return 1;
        }
        *c = size(u) > size(v) ? 'C' : 'B';
        return 1;
    }
    return 1;
}

int oneLine ( vector u, vector v, char * c )
{
    if ( precise( v.x, 0 ) || precise( v.y, 0 ) )
    {
        if ( nuly( u, v, c ) )
            return 1;
        return 0;    
    }
    double d1 = u.x / v.x;
    double d2 = u.y / v.y;
    if ( ! precise( d1, d2 ) )
        return 0;
    if ( u.x * v.x < 0 && u.y * v.y < 0 )
        *c = 'A';
    else if ( size(u) < size(v) )
        *c = 'B';
    else
        *c = 'C';
    return 1;
}

int main ( void )
{
    vector a, b, c, r, s;
    char bod = 'N';
    printf( "Bod A:\n" );
    if ( ! nacteni( &a ) )
        return error();
    printf( "Bod B:\n" );
    if ( ! nacteni( &b ) )
        return error();
    printf( "Bod C:\n" );
    if ( ! nacteni( &c ) )
        return error();
    r = getVec ( a, b );
    s = getVec ( a, c );
    if ( splyvani( r, s ) )
    {
        printf( "Nektere body splyvaji.\n" );
        return 0;
    }
    if ( ! oneLine ( r, s, &bod ) )
    {
        printf( "Body nelezi na jedne primce.\n" );
        return 0; 
    }
    printf( "Body lezi na jedne primce.\nProstredni je bod %c.\n", bod );
    return 0;
}