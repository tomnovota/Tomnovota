#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int prest (int year) {
    if ( (year % 4 == 0 && year % 100 != 0)
        || (year % 400 == 0 && year % 4000 != 0) )
        return 1;
    return 0;
}

int valday (int day, int month, int year) {
    if (month == 2) {
        if (prest (year) == 1) {
            if (day < 30)
                return 1;
            else return 0;
        };
        if (day < 29)
            return 1;
        else
            return 0;
    };
    if ( (month < 8 && month % 2 == 1)
                    || (month > 7 && month % 2 == 0) ) {
            if (day < 32)
                return 1;
            else
                return 0;
    };
    if (day < 31)
        return 1;
    else
        return 0;
}

int dateToIndex(int day, int month, int year, int *idx) {
    //PODMÍNKY
    if (year < 2000
        || month < 1
            || month > 12
                || day < 0
                    || valday (day, month, year) != 1)
        return 0;

    if (month == 1) {
        *idx = day;
        return 1;
    }
    if (month == 2) {
        *idx = 31 + day;
        return 1;
    }
    int lunor = 59;
    if (prest (year) == 1)
        lunor = 60;
    if (month == 3)
        *idx = lunor + day;
    if (month == 4)
        *idx = lunor + 31 + day;
    if (month == 5)
        *idx = lunor + 61 + day;
    if (month == 6)
        *idx = lunor + 92 + day;
    if (month == 7)
        *idx = lunor + 122 + day;
    if (month == 8)
        *idx = lunor + 153 + day;
    if (month == 9)
        *idx = lunor + 184 + day;
    if (month == 10)
        *idx = lunor + 214 + day;
    if (month == 11)
        *idx = lunor + 245 + day;
    if (month == 12)
        *idx = lunor + 275 + day;
    return 1;
}

#ifndef __PROGTEST__
int main (int argc, char * argv []) {
    int idx;
    assert(dateToIndex(29,  2, 2001, &idx) == 0);
    assert(dateToIndex( 1,  1, 1999, &idx) == 0);
    assert(dateToIndex(29,  2, 4000, &idx) == 0);

    assert(dateToIndex( 1,  1, 2000, &idx) == 1 && idx == 1);
    assert(dateToIndex( 1,  2, 2000, &idx) == 1 && idx == 32);
    assert(dateToIndex(29,  2, 2000, &idx) == 1 && idx == 60);

    assert(dateToIndex( 1, 12, 2000, &idx) == 1 && idx == 336);
    assert(dateToIndex(31, 12, 2000, &idx) == 1 && idx == 366);

    assert(dateToIndex( 6,  7, 3600, &idx) == 1 && idx == 188);
    assert(dateToIndex(29,  2, 3600, &idx) == 1 && idx == 60);

    return 0;
}
#endif /* __PROGTEST__ */


