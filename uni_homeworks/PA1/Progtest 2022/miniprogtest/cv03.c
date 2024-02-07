#include <stdio.h>

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int nacteni ( int * h, int * m, int * s, int * ms )
{
    if ( scanf(" %d : %d : %d , %d", h, m, s, ms) != 4
        || *h < 0 || *h > 23
        || *m < 0 || *m > 59
        || *s < 0 || *s > 59
        || *ms < 0 || *ms > 999 )
        return 0;
    return 1;
}

long long convert ( int h, int m, int s, int ms )
{
    return ms + s*1000 + m*60000 + h*3600000;
}

void getRes ( long long res, int * h, int * m, int * s, int * ms )
{
    *h = res / 3600000 ;
    res %= 3600000;
    *m = res / 60000;
    res %= 60000;
    *s = res / 1000;
    res %= 1000;
    *ms = res;
}

int main (void)
{
    int h1, m1, s1, ms1;
    int h2, m2, s2, ms2;
    long long t1, t2;
    printf("Zadejte cas t1:\n");
    if ( ! nacteni( &h1, &m1, &s1, &ms1 ) )
        return error();
    printf("Zadejte cas t2:\n");
    if ( ! nacteni( &h2, &m2, &s2, &ms2 ) )
        return error();
    t1 = convert ( h1, m1, s1, ms1 );
    t2 = convert ( h2, m2, s2, ms2 );
    if ( t1 > t2 )
        return error();
    getRes( t2 - t1, &h1, &m1, &s1, &ms1 );
    printf("Doba: %2d:%02d:%02d,%03d\n", h1, m1, s1, ms1 );
    return 0;
}