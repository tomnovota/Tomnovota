#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

        // zjisti zda je druhe datum pozdejsi datum, 0=ano 1=ne
int validInterval (long int a, long int b) {
    if (b - a >= 0)
        return 0;
    return 1;
}
        // konverze vsech udaju na jedno cislo
long int convertTime (int y, int m, int d, int h, int i) {
    return (((y*10e2 + m)*10e2 + d)*10e2 + h)*10e2 + i;
}
        // zjisti zda je rok prestupny 1=ano 0=ne
int leap (int y) {
    if ( (y % 4 == 0 && y % 100 != 0)
            || (y % 400 == 0 && y % 4000 != 0) )
        return 1;
    return 0;
}
        // zjisti zda pocet dni je validni vuci mesici a roku 1=ano 0=ne
int validDay (int y, int m, int d) {
    if (m == 2) {
        if (leap (y) == 1) {
            if (d < 30)
                return 1;
            return 0;
        };
        if (d < 29)
            return 1;
        return 0;
    };
    if ( (m < 8 && m % 2 == 1)
                    || (m > 7 && m % 2 == 0) ) {
            if (d < 32)
                return 1;
            return 0;
    };
    if (d < 31)
        return 1;
    return 0;
}
        // zjisti zda datum splnuje podminky, vrací 1=neplatné 0=validní
int validDate (int y, int m, int d, int h, int i) {
    if (y < 1600
        || m < 1 || m > 12
            || d < 1 || validDay (y, m, d) == 0
                || h < 0 || h > 23
                    || i < 0 || i > 59)
        return 1;
    return 0;
}
        // extrahuje cislice z dvouciferneho cisla
int accessNumber (int num, int pos) {
    int digit2 = num/10;
    if (pos == 1)
        return num - digit2*10;
    return digit2;
}
        // vyhodnoti energii v jednom kroku
int countSmall (int num1, int num2) {
    if ( num1 == 0 )
        return 4;
    if ( num1 == 1 )
        return 5;
    if ( num1 == 2 && num2 == 3 )
        return 2;
    if ( num1 == 2 && num2 == 0 )
        return 3;
    if ( num1 == 3 )
        return 3;
    if ( num1 == 4 )
        return 3;
    if ( num1 == 5 && num2 == 6)
        return 1;
    if ( num1 == 5 && num2 == 0)
        return 3;
    if ( num1 == 6 )
        return 5;
    if ( num1 == 7 )
        return 4;
    if ( num1 == 8 )
        return 1;
    if ( num1 == 9 )
        return 2;
    else {
        return 0;
    }
}
        // pocita kolik energie je potreba na samostatne minuty + sekundy
long long int countMinute (int i1, int i2) {
    long long int energie = 0;
    int tmp = i1;
    while (i1 != i2) {
        energie += 200; // energie sekund za 1 minutu
        int next = i1 + 1;
        if (i1 == 59) {
            energie += 5; // 5->0 + 9->0
            i1 = 0;
        }
        else if ( accessNumber (i1, 2) == accessNumber (next, 2) ) {
            energie += countSmall ( accessNumber (i1, 1), accessNumber(next, 1) );
            i1 = next;
        }
        else {
            energie += countSmall ( accessNumber (i1, 2), accessNumber(next, 2) ) + 2; // 9->0
            i1 = next;
        };
    };
    if (tmp > i2)
        energie -= 12200; // pokud se v minutach meni hodina tak se odecte cela jedna hodina, ktera se pocita navic
    return energie;
}
        // pocita kolik energie je potreba na samostatne hodiny (s jejich sekundami a minutami)
long long int countHour (int h1, int h2) {
    long long int energie = 0;
    int tmp = h1;
    while (h1 != h2) {
        energie += 12200; // energie minut a sekund za 1 hodinu
        int next = h1 + 1;
        if (h1 == 23) {
            energie += 6; // 2->0 + 3->0
            h1 = 0;
        }
        else if ( accessNumber (h1, 2) == accessNumber (next, 2) ) {
            energie += countSmall ( accessNumber (h1, 1), accessNumber(next, 1) );
            h1 = next;
        }
        else {
            energie += countSmall ( accessNumber (h1, 2), accessNumber(next, 2) ) + 2; // 9->0
            h1 = next;
        };
    };
    if (tmp > h2)
        energie -= 292886; // pokud se v minutach meni hodina tak se odecte cela jedna hodina, ktera se pocita navic
    return energie;
}
        // pocita pocet dni v daném roce
int daysIn (int y, int m, int d) {
    if (m == 1)
        return d;
    if (m == 2)
        return 31 + d;
    int lunor = 59;
    if (leap (y) == 1)
        lunor = 60;
    if (m == 3)
        return lunor + d;
    if (m == 4)
        return lunor + 31 + d;
    if (m == 5)
        return lunor + 61 + d;
    if (m == 6)
        return lunor + 92 + d;
    if (m == 7)
        return lunor + 122 + d;
    if (m == 8)
        return lunor + 153 + d;
    if (m == 9)
        return lunor + 184 + d;
    if (m == 10)
        return lunor + 214 + d;
    if (m == 11)
        return lunor + 245 + d;
    if (m == 12)
        return lunor + 275 + d;
    return 0;
}
        // pocita kolik energie je potreba na samostatne dny
long long int countDay (int y1, int m1, int d1, int y2, int m2, int d2) {
    int firstYear = 0;
    int lastYear = 0;
    if (y1 < y2) {
        firstYear = 365 - daysIn (y1, m1, d1);
        if (leap (y1) == 1)
            firstYear += 1;
        lastYear = daysIn (y2, m2, d2);
    }
    else
        firstYear = daysIn (y2, m2, d2) - daysIn(y1, m1, d1);
    int tmp = y1 + 1;
    int betweenYear = 0;
    while (tmp < y2) {
        betweenYear += daysIn (tmp, 12, 31);
        tmp += 1;
    };
    return ((long long int)firstYear + lastYear + betweenYear)*292886;
}
        // HLAVNÍ FCE
int energyConsumption ( int y1, int m1, int d1, int h1, int i1,
                        int y2, int m2, int d2, int h2, int i2, long long int * consumption ) {
    long int date1 = convertTime ( y1, m1, d1, h1, i1);
    long int date2 = convertTime (y2, m2, d2, h2, i2);

    if (validDate (y1, m1, d1, h1, i1)
            || validDate (y2, m2, d2, h2, i2)
                || validInterval (date1, date2) )
        return 0;
    * consumption = countMinute (i1, i2) + countHour (h1, h2) + countDay (y1, m1, d1, y2, m2, d2);
    return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int consumption;

  assert ( accessNumber (25,1) == 5 );
  assert ( accessNumber (25,2) == 2 );

  assert ( countSmall (5,6) == 1 );
  assert ( countSmall (6,7) == 5 );
  assert ( countSmall (2,0) == 3 );

  assert ( countMinute (5,7) == 406 );
  assert ( countMinute (2,9) == 1419 );
  assert ( countMinute (25,37) == 2438 );
  assert ( countMinute (58,1) == 610-12200 );

  assert ( countHour (2,4) == 5 + 24400 );
  assert ( countHour (8,10) == 7 + 24400 );
  assert ( countHour (21,3) == 24 + 6*12200 - 292886);

  assert ( daysIn (2020, 1, 20) == 20 );
  assert ( daysIn (2020, 10, 27) == 301 );

  assert ( countDay (2021, 10, 1, 2021, 10, 2) == 1*292886);

  assert ( validInterval (202010510502L, 202010510602L) == 0 );
  assert ( validInterval (202010510502L, 202010510402L) == 1 );
  assert ( validInterval (202010510502L, 201910510602L) == 1 );
  assert ( validInterval (202010510502L, 202008510602L) == 1 );

  assert ( energyConsumption ( 2021, 11,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2021, 10, 32, 12,  0,
                               2021, 11, 10, 12,  0, &consumption ) == 0 );
  assert ( energyConsumption ( 2100,  2, 29, 12,  0,
                               2100,  2, 29, 12,  0, &consumption ) == 0 );

  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  1, 18, 45, &consumption ) == 1
           && consumption == 67116LL );
  assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                               2021, 10,  2, 11, 20, &consumption ) == 1
           && consumption == 269497LL );
  assert ( energyConsumption ( 2021,  1,  1, 13, 15,
                               2021, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2024,  1,  1, 13, 15,
                               2024, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81398919LL );
  assert ( energyConsumption ( 1900,  1,  1, 13, 15,
                               1900, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );
  assert ( energyConsumption ( 2021, 10,  1,  0,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 146443LL );
  assert ( energyConsumption ( 2021, 10,  1,  0, 15,
                               2021, 10,  1,  0, 25, &consumption ) == 1
           && consumption == 2035LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  0, &consumption ) == 1
           && consumption == 0LL );
  assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                               2021, 10,  1, 12,  1, &consumption ) == 1
           && consumption == 204LL );
  assert ( energyConsumption ( 2400,  2, 29, 12,  0,
                               2400,  2, 29, 12,  0, &consumption ) == 1
           && consumption == 0LL );
  assert ( energyConsumption ( 2116, 11, 12, 12, 9, 2126, 11, 4, 13, 34, &consumption ) ==1
           && consumption == 1067293873LL);
  return 0;
}
#endif /* __PROGTEST__ */

