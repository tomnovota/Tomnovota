#include <stdio.h>
#include <math.h>

typedef struct T_Time
{
    int hr;
    int min;
    int sec;
    int ms;
} T_TIME;

int nacteni ( T_TIME * time1, T_TIME * time2 )
{
    T_TIME t1, t2;

    printf ( "Zadejte cas t1:\n" );
    if ( scanf ( " %d : %d : %d , %d", &t1.hr, &t1.min, &t1.sec, &t1.ms ) != 4
        || t1.hr < 0 || t1.hr > 23
        || t1.min < 0 || t1.min > 59
        || t1.sec < 0 || t1.sec > 59
        || t1.ms < 0 || t1.ms > 999 )
    {
        printf ( "Nespravny vstup.\n" );
        return 0;
    }
    time1->hr = t1.hr;
    time1->min = t1.min;
    time1->sec = t1.sec;
    time1->ms = t1.ms;
    printf ( "Zadejte cas t2:\n" );
    if ( scanf ( " %d : %d : %d , %d", &t2.hr, &t2.min, &t2.sec, &t2.ms ) != 4
        || t2.hr < 0 || t2.hr > 23
        || t2.min < 0 || t2.min > 59
        || t2.sec < 0 || t2.sec > 59
        || t2.ms < 0 || t2.ms > 999 )
    {
        printf ( "Nespravny vstup.\n" );
        return 0;
    }
    time2->hr = t2.hr;
    time2->min = t2.min;
    time2->sec = t2.sec;
    time2->ms = t2.ms;
    return 1;
}

void deduct ( int num1, int num2, int * doba, int * num11, int constant )
{
    int vysl = num2 - num1;
    if ( vysl < 0 )
    {
        * num11 += 1;
        * doba = constant + vysl;
    }
    else
        * doba = vysl;
}

int vypocet ( T_TIME time1, T_TIME time2 )
{
    T_TIME doba;
    int kontrola = 0;

    deduct ( time1.ms, time2.ms, &doba.ms, &time1.sec, 1000 );
    deduct ( time1.sec, time2.sec, &doba.sec, &time1.min, 60 );
    deduct ( time1.min, time2.min, &doba.min, &time1.hr, 60 );
    deduct ( time1.hr, time2.hr, &doba.hr, &kontrola, 24 );

    if ( kontrola != 0 )
    {
        printf ( "Nespravny vstup.\n" );
        return 0;
    }

    printf ( "Doba: " );
    if ( doba.hr < 10 )
        printf ( " " );
    printf ( "%d:%02d:%02d,%03d\n", doba.hr, doba.min, doba.sec, doba.ms );
    return 1;
}

int main ( void )
{
    T_TIME time1, time2;
    if ( nacteni ( &time1, &time2 ) != 1 )
        return 1;
    if ( vypocet ( time1, time2 ) != 1 )
        return 1;
    return 0;
}

