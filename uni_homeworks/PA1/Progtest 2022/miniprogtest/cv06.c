#ifndef __PROGTEST__
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdint.h>
#endif /* __PROGTEST__ */

long long getMod ( long long x )
{
    return x - (x / 3) * 3;
} 

long long avg3 ( long long a, long long b, long long c )
{
  long long mod1, mod2, mod3;
  mod1 = getMod(a);
  mod2 = getMod(b);
  mod3 = getMod(c);
  long long mod_all = (mod1 + mod2 + mod3)/3;
  a /= 3;
  b /= 3;
  c /= 3;

  return a + b + c + mod_all;
}

#ifndef __PROGTEST__
int main (int argc, char * argv [])
{ 
  printf( "%lld\n", getMod(9223372036854775800) );
  assert ( avg3 ( 9223372036854775800, 9223372036854775800, -8 ) == 6148914691236517197 );
  
  return 0;
}
#endif /* __PROGTEST__ */

