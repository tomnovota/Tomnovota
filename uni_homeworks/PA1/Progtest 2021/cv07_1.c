#include <stdio.h>
#include <stdlib.h>
#define MAX_POCET_STRAN 27
#define DH 1.42

int mandaty [27];

int nacteni1 (int * N) {
    int a;
    if (scanf ("%d", &a) != 1
        || a < 2
            || a > 26)
        return 0;
    * N = a;
    return 1;
}

int duplicita (char nazvyStran [MAX_POCET_STRAN], int kolik) {
    for (int i = 0; i < (kolik - 1); i++) {
        if ( nazvyStran [i] == nazvyStran [kolik] )
            return 1;
    };
    return 0;
}

int nacteni2 (char nazvyStran [MAX_POCET_STRAN], double poctyHlasu [MAX_POCET_STRAN], int N) {
    int a;
    for (int i = 0; i < N; i++) {
        if (scanf (" %c %d", &nazvyStran [i], &a) != 2
                || nazvyStran [i] < 'A'
                    || nazvyStran [i] > ('A' + (N - 1) )
                        || duplicita (nazvyStran, i)
                            || a < 1)
            return 0;
        else
            poctyHlasu [i] = (a / DH);
    };
    return 1;
}

int nacteni3 (int * hornici) {
    int a;
    if (scanf ("%d", &a) != 1
        || a < 1)
        return 0;
    * hornici = a;
    return 1;
}

void metoda ( double poctyHlasu [MAX_POCET_STRAN], int N, int mandaty [27], int hornici ) {
    for (int j = 0; j < hornici; j++) {
        int max = 0;
        for (int i = 1; i < (N - 1); i++) {
            if ( poctyHlasu [max] < poctyHlasu [i] )
                max = i;
        };
        mandaty [max] += 1;
        if ( mandaty [max] == 1 )
            poctyHlasu [max] = (poctyHlasu [max] * DH) / 2;
        else {
            poctyHlasu [max] = (poctyHlasu [max] * (mandaty [max] - 1) ) / mandaty [max];
            //printf ("MAX: %d\n", max);
        };
    };
}

int main (void) {

    printf ("Pocet stran:\n");
    int N;
    if (nacteni1 (&N) != 1) {
        printf ("Nespravny vstup.\n");
        return 1;
    };

    printf ("Strany a pocet hlasu:\n");
    char nazvyStran [MAX_POCET_STRAN];
    double poctyHlasu [MAX_POCET_STRAN];
    if ( nacteni2 ( nazvyStran, poctyHlasu, N) != 1 ) {
        printf ("Nespravny vstup.\n");
        return 1;
    };

    printf ("Pocet horniku:\n");
    int hornici;
    if (  nacteni3 (&hornici) != 1 ) {
        printf ("Nespravny vstup.\n");
        return 1;
    };

    metoda (poctyHlasu, N, mandaty, hornici);
    printf ("Pridelene pocty:\n");
    for ( int i = 0; i < N; i++ )
        printf ( "%c: %d\n", nazvyStran [i], mandaty [i] );

    return 0;
}

