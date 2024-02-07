#ifndef __PROGTEST__
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#endif /* __PROGTEST__ */

long long avg3 ( long long a, long long b, long long c )
{
  if (
     ( ( a > 0 ) && ( b > 0 ) && ( (a + b <= 0) ) )
  || ( ( a > 0 ) && ( c > 0 ) && ( (a + c <= 0) ) ) 
  || ( ( b > 0 ) && ( c > 0 ) && ( (b + c <= 0) ) ) 
  )
    return ( (a/3) + (b/3) + (c/3) );
  return ( (a + b + c) / 3 );
}

#ifndef __PROGTEST__
int main (int argc, char * argv [])
{
  assert ( avg3 ( 1, 2, 3 ) == 2 );
  assert ( avg3 ( -100, 100, 30 ) == 10 );
  assert ( avg3 ( 1, 2, 2 ) == 1 );
  assert ( avg3 ( -1, -2, -2 ) == -1 );
  assert ( avg3 ( LLONG_MAX, LLONG_MAX, LLONG_MAX ) == LLONG_MAX );
  return 0;
}
#endif /* __PROGTEST__ */

