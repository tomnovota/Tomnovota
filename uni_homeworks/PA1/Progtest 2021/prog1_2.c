#include <stdio.h>
#include <math.h>
#include <float.h>

#define DBL_MOJE 2e-13

int error () 
{
    printf ( "Nespravny vstup.\n" );
    return 0;
}

double getVal ()
{
    double num;
    if ( scanf ( " %lf", &num ) != 1 || num <= 0 )
        return 0;
    return num;
}

int getO( double * obvod, double * obsah, char * druh )
{
    char ch;
    if ( scanf( " %c", &ch) != 1
        || ( ch != 'S' && ch != 'R' && ch != 'T' )
        ) return error();
    if ( ch == 'S' ) 
    {
        double a;
        * druh = ch;
        a = getVal();
        if ( !a )
            return error();
     
        * obvod = 4*a;
        * obsah = a*a;
    };
    if ( ch == 'R' ) 
    {
        double a, b;
        * druh = ch;
        a = getVal();
        if ( !a )
            return error();
        b = getVal();
        if ( !b )
            return error();

        if (a==b)
            return error();
        * obvod = 2*(a+b);
        * obsah = a*b;
    };
    if ( ch == 'T' )
    {
        double a, b, c, s;
        * druh = ch;
        a = getVal();
        if ( !a )
            return error();
        b = getVal();
        if ( !b )
            return error();
        c = getVal();
        if ( !c )
            return error();

        if ( a+b<=c || a+c<=b || b+c<=a
            || fabs(a+b-c) <= (DBL_MOJE*fabs(a+b+c) )
            || fabs(a+c-b) <= (DBL_MOJE*fabs(a+b+c) )
            || fabs(b+c-a) <= (DBL_MOJE*fabs(a+b+c) ) )
            return error();
        s = (a+b+c) /2;
        * obvod = a+b+c;
        * obsah = sqrt (s*(s-a)*(s-b)*(s-c));
    };
    return 1;
}

void obraz( char druh )
{
    if ( druh == 'S' ) printf ( "ctverec " );
    else if ( druh == 'R' ) printf ( "obdelnik " );
    else printf ( "trojuhelnik " );
}

int printRes ( char druh1, double obv1, double obs1, char druh2, double obv2, double obs2 )
{
    char cmp;
    if ( obv1 == obv2 
        || (fabs(obv1-obv2) <= ( DBL_MOJE* fabs(obv1+obv2) ) )
        ) 
        cmp = '=';
    else if ( obv1 > obv2 ) 
        cmp = '>';
    else 
        cmp = '<';
    printf( "Obvod: " );
    obraz( druh1 );
    printf( "#1 %c " , cmp ); 
    obraz( druh2 );
    printf( "#2\n" );
    
    if ( obs1 == obs2 
        || (fabs(obs1-obs2) <= ( DBL_MOJE* fabs(obs1+obs2) ) ) 
        ) 
        cmp = '=';
    else if ( obs1 > obs2 ) 
        cmp = '>';
    else 
        cmp = '<';
    printf( "Obsah: " );
    obraz( druh1 );
    printf( "#1 %c " , cmp ); 
    obraz( druh2 );
    printf( "#2\n" );
    return 0;
}

int main (void)
{
    double obr1_obvod, obr1_obsah, obr2_obvod, obr2_obsah;
    char druh1; char druh2;

    printf ( "Obrazec #1\n" );
    if ( ! getO( &obr1_obvod, &obr1_obsah, &druh1 ) ) return 0;
    
    printf ( "Obrazec #2\n" );
    if ( ! getO( &obr2_obvod, &obr2_obsah, &druh2 ) ) return 0;

    return printRes( druh1, obr1_obvod, obr1_obsah, druh2, obr2_obvod, obr2_obsah );
    return 0;
}

