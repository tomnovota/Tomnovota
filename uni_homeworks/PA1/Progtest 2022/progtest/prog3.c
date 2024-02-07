#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

typedef struct {
    long long int y;
    long long int m;
    long long int d;
    long long int h;
    long long int i;
} date;

typedef struct {
    long long int st;
    long long int nd;
} bell;

long long int isLeap ( long long int y )
{
    if ( ( (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0 && y % 4000 != 0) ) )
        return 1;
    return 0;
}

long long int group ( long long int m, long long int y )
{
    return m == 2 ? 28 + isLeap(y) : ( m < 8 ? (m % 2 == 0 ? 30 : 31) : (m % 2 == 0 ? 31 : 30) );
}

long long int getDate ( long long int y, long long int m, long long int d, long long int h, long long int i, date * dates )
{
    if( y < 1600 || m < 1 || m > 12 || d < 1 || h < 0 || h > 23 || i < 0 || i > 59 )
        return 0;
    if ( d > group( m, y ) )
        return 0;
    dates->y = y;
    dates->m = m;
    dates->d = d;
    dates->h = h;
    dates->i = i;
    return 1;
}

long long int date_order ( date d1, date d2 )
{
    if( d1.y > d2.y || ( d1.y == d2.y && (d1.m > d2.m || ( d1.m == d2.m && ( d1.d > d2.d || ( d1.d == d2.d && ( d1.h > d2.h || ( d1.h == d2.h && d1.i > d2.i ) ) ) ) ) ) ) )
        return 0;
    return 1;
}

unsigned char whatDay ( long long int d, long long int m, long long int y )
{
    if ( m < 3 )
    {
        m += 12;
        y = y - 1;
    }
    long long int h = ( d + ( 26 * (m + 1) / 10 ) + y + ( y / 4 ) + 6 * (y / 100) + (y / 400) + 6 * (y / 4000) ) % 7;
    return ( h + 5 ) % 7;
}

void HourBell ( long long int * h, long long int * min, bell * bell_count )
{
    bell_count->nd += *h == 0 ? 12 : *h;
    bell_count->st += 10;
    *h += 1;
}

bell getBellsUntilDay ( long long int h, long long int min )
{
    bell bell_count = {0, 0};
// upraveno do tvaru minut od kterých se počítá
    min += min % 15 != 0 ? (15 - (min % 15)) : 0;
// dorovnání na celé hodiny
    bell_count.nd += min == 0 ? (h == 0 ? 12 : (h > 12 ? (h - 12) : h)) : 0;
    while( min != 60 )
    {
        bell_count.st += min == 0 ? 4 : ( min == 45 ? 3 : ( min == 30 ? 2 : 1 ) );
        min += 15;
    }
    h += 1;
    min -= 60;
    while( h < 12 )
        HourBell( &h, &min, &bell_count );
    h -= 12;
    while( h < 12 )
        HourBell( &h, &min, &bell_count );
    return bell_count;
}

bell complement ( long long int h, long long int min )
{
    bell tmp = {0, 0};
    if ( min % 15 == 0 )
    {
        tmp.st = min == 0 ? 4 : ( min == 45 ? 3 : ( min == 30 ? 2 : 1 ) );
        tmp.nd = min == 0 ? (h == 0 ? 12 : (h > 12 ? (h - 12) : h)) : 0;
    }
    return tmp;
}

long long int howManyLeap ( long long int y )
{
    return (y / 4) - (y / 100) + (y / 400) - (y / 4000);
}

long long int howManyDays ( date d1, date d2 )
{
    long long int count = -1;
    if( d1.m == d2.m && d1.y == d2.y)
    {
        return count + d2.d - d1.d; // 3.5. - 5.5. = 1 den
    }
    count += group( d1.m++, d1.y ) - d1.d + d2.d;
    if( d1.y == d2.y )
    {
        while( d1.m < d2.m )
        {
            count += group( d1.m, d1.y );
            d1.m++;
        }
        return count;
    }
    while( d1.m < 13 )
    {
        count += group( d1.m, d1.y );
        d1.m++;
    }
    d1.m -= 12;
    d1.y++;
    if( d1.y < d2.y )
    {
        count += (d2.y - d1.y) * 365 + howManyLeap(d2.y) - howManyLeap(d1.y - 1) - isLeap(d2.y);
    }
    d1.y = d2.y;
    while( d1.m < d2.m )
    {
        count += group( d1.m, d1.y );
        d1.m++;
    }
    return count;
}

int bells ( int y1, int m1, int d1, int h1, int i1,
            int y2, int m2, int d2, int h2, int i2,
            long long int * b1, long long int * b2 )
{
    date date1;
    date date2;
// ošetření vsutpních datumů
    if( ! getDate( y1, m1, d1, h1, i1, &date1 ) || ! getDate( y2, m2, d2, h2, i2, &date2 )
        || ! date_order( date1, date2 ) )
        return 0;
        
// inicializace potřebných proměnných
    bell bell_count = { 0, 0 };
    bell one_day_count = { 240, 156 };

// spočítat den v týdnu prvního a posledního dne
    unsigned char day1, day2;
    day1 = whatDay( date1.d, date1.m, date1.y );
    day2 = whatDay( date2.d, date2.m, date2.y );
    //printf("day1 = %d, day2 = %d\n", day1, day2);

// spočítat počet odbití do konce prvního a posledního dne
    bell start = getBellsUntilDay( date1.h, date1.i );
    bell end = getBellsUntilDay( date2.h, date2.i );
    //printf("start.st = %lld, start.nd = %lld\n", start.st, start.nd);
    //printf("end.st = %lld, end.nd = %lld\n", end.st, end.nd);
    

// pokud jsou datumy ve stejný den, tak rovnou spočítat zvonění
    if( date1.y == date2.y && date1.m == date2.m && date1.d == date2.d )
    {
        if ( day1 == 6 ) // jestli je to neděle
        {
            *b1 = 0;
            *b2 = 0;            
            return 1;
        }
        bell complem = complement(date2.h, date2.i);
        //printf( "start:\n  b1 = %lld\n  b2 = %lld\n", start.st, start.nd );
        //printf( "end:\n  b1 = %lld\n  b2 = %lld\n", end.st, end.nd );
        //printf( "complement:\n  b1 = %lld\n  b2 = %lld\n", complem.st, complem.nd );
        bell_count.st = start.st - end.st + complem.st;
        bell_count.nd = start.nd - end.nd + complem.nd;
        *b1 = bell_count.st;
        *b2 = bell_count.nd;
        //printf( "bell_count:\n  b1 = %lld\n  b2 = %lld\n", bell_count.st, bell_count.nd );
        return 1;
    }

// přičíst počet odbití prvního dne
    if( day1 != 6 )
    {
        bell_count.st += start.st;
        bell_count.nd += start.nd;
    }
    
// přičíst počet odbití  poslední den
    if( day2 != 6 )
    {
        bell_count.st += one_day_count.st - end.st + complement(date2.h, date2.i).st;
        bell_count.nd += one_day_count.nd - end.nd + complement(date2.h, date2.i).nd;
    }

// spočítat kolik dní je v intervalu
    long long int day_count = howManyDays( date1, date2 );
    //printf("day_count = %d\n", day_count);
// odečíst (počet dní / 7 + "1") nedělí v intervalu
    day_count -= day_count / 7;
    //printf("day_count = %d\n", day_count);
    if ( day1 >= day2 && day1 != 6 && day2 != 6 )
        day_count--;
    //printf("day_count = %d\n", day_count);

// spočítat počet odbití v těchto dnech 
    bell_count.st += day_count * one_day_count.st;
    bell_count.nd += day_count * one_day_count.nd;
    
// výsledek
    *b1 = bell_count.st;
    *b2 = bell_count.nd;

    return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int b1, b2;
  date date0;

    bells ( 1999, 10,  1, 13, 15, 2010, 10,  1, 18, 45, &b1, &b2 );
    printf("out -> b1: %lld   b2: %lld\n\n", b1, b2);

    bells ( 2000, 1,  1, 13, 15, 2022, 1,  1, 18, 45, &b1, &b2 );
    printf("out -> b1: %lld   b2: %lld\n\n", b1, b2);

    bells ( 1840, 5,  3, 13, 15, 2022, 3,  9, 18, 45, &b1, &b2 );
    printf("out -> b1: %lld   b2: %lld\n\n", b1, b2);

    assert( getDate(2022, 10, 29, 25, 38, &date0) == 0 );
    assert( getDate(2022, 2, 29, 22, 38, &date0) == 0 );
    assert( getDate(2020, 2, 29, 22, 38, &date0) == 1 );
    assert( getDate(2021, 2, 29, 22, 38, &date0) == 0 );
    assert( getDate(2020, 10, 31, 22, 38, &date0) == 1 );
    assert( getDate(2020, 4, 31, 22, 38, &date0) == 0 );

    assert( whatDay(29, 10, 2022) == 5 );
    assert( whatDay(4, 10, 2001) == 3 );
    assert( whatDay(1, 1, 2020) == 2 );
    assert( whatDay(4, 10, 2020) == 6 );
    assert( whatDay( 29, 2, 2004 ) == 6 );
    assert( whatDay( 1, 10, 2022 ) == 5);
    assert( whatDay( 3, 10, 2022 ) == 0);


  assert ( bells ( 2022, 10,  1, 13, 15,
                   2022, 10,  1, 18, 45, &b1, &b2 ) == 1
           && b1 == 56
           && b2 == 20 );
  assert ( bells ( 2022, 10,  3, 13, 15,
                   2022, 10,  4, 11, 20, &b1, &b2 ) == 1
           && b1 == 221
           && b2 == 143 );
  assert ( bells ( 2022, 10,  1, 13, 15,
                   2022, 10,  2, 11, 20, &b1, &b2 ) == 1
           && b1 == 106
           && b2 == 65 );
  assert ( bells ( 2022, 10,  2, 13, 15,
                   2022, 10,  3, 11, 20, &b1, &b2 ) == 1
           && b1 == 115
           && b2 == 78 );
  assert ( bells ( 2022, 10,  1, 13, 15,
                   2022, 10,  3, 11, 20, &b1, &b2 ) == 1
           && b1 == 221
           && b2 == 143 );
  assert ( bells ( 2022,  1,  1, 13, 15,
                   2022, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 56861
           && b2 == 36959 );
  assert ( bells ( 2019,  1,  1, 13, 15,
                   2019, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 57101
           && b2 == 37115 );
  assert ( bells ( 2024,  1,  1, 13, 15,
                   2024, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 57341
           && b2 == 37271 );
  assert ( bells ( 1900,  1,  1, 13, 15,
                   1900, 10,  5, 11, 20, &b1, &b2 ) == 1
           && b1 == 57101
           && b2 == 37115 );
  assert ( bells ( 2022, 10,  1,  0,  0,
                   2022, 10,  1, 12,  0, &b1, &b2 ) == 1
           && b1 == 124
           && b2 == 90 );
  assert ( bells ( 2022, 10,  1,  0, 15,
                   2022, 10,  1,  0, 25, &b1, &b2 ) == 1
           && b1 == 1
           && b2 == 0 );
  assert ( bells ( 2022, 10,  1, 12,  0,
                   2022, 10,  1, 12,  0, &b1, &b2 ) == 1
           && b1 == 4
           && b2 == 12 );
  assert ( bells ( 2000,  2, 29, 12,  0,
                   2000,  2, 29, 12,  0, &b1, &b2 ) == 1
           && b1 == 4
           && b2 == 12 );
  assert ( bells ( 2004,  2, 29, 12,  0,
                   2004,  2, 29, 12,  0, &b1, &b2 ) == 1
           && b1 == 0
           && b2 == 0 );
  assert ( bells ( 2022, 11,  1, 12,  0,
                   2022, 10,  1, 12,  0, &b1, &b2 ) == 0
           && b1 == 0
           && b2 == 0 );
  assert ( bells ( 2022, 10, 32, 12,  0,
                   2022, 11, 10, 12,  0, &b1, &b2 ) == 0
           && b1 == 0
           && b2 == 0 );
  assert ( bells ( 2100,  2, 29, 12,  0,
                   2100,  2, 29, 12,  0, &b1, &b2 ) == 0
           && b1 == 0
           && b2 == 0 );
    assert( bells ( 1914, 7, 3, 15, 23, 
                    2033, 10, 21, 19, 51, 
                    &b1, &b2 ) == 1 
                    && b1 == 8964045 
                    && b2 == 5826622);             

    assert( bells ( 2078, 3, 11, 13, 0, 
                    2078, 3, 11, 13, 0, &b1, &b2 ) == 1 
                    && b1 == 4 
                    && b2 == 1);
      assert ( bells ( 2022, 10,  29, 0, 0,
                   2022, 11,  7, 0, 0, &b1, &b2 ) == 1
           && b1 == 7*240+4
           && b2 == 7*156+12 );


  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
