#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINS = -10.000
#define MAXS = 10.000

int error()
{
    printf("Nespravny vstup.\n");
    return 0;
}

int main ( int argc, const char * argv [] )
{
    srand( time( NULL ));
    int start = 0; // start = 10^3 * 0.000
    int end = rand();

    int a = scanf("%d", &start);
    while ( a )
    {
        
    }
    return 0;
}