#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#include <math.h>
#endif /* __PROGTEST__ */

double twoWayDistance ( int fuelTotal, int maxLoad )
{
  double i = 1.0, dist = 0.0;
  while( fuelTotal > 0 )
  {
    dist += ( (double) ( fuelTotal > maxLoad ? maxLoad : fuelTotal ) ) / i++;
    fuelTotal -= maxLoad;
  }
  double res = dist / 2.0;
  printf( "Výsledek je: %f\n", res );
  return res;
}

#ifndef __PROGTEST__
int smallDiff ( double a, double b )
{
  printf( "fabs( a - b ) = %g\n ( fabs(a + b) * 10e-14 ) = %g\n", fabs( a - b ), ( fabs(a + b) * 10e-14 ) );
  if ( a == b
    || fabs( a - b ) <= ( fabs(a + b) * 10e-14 ) ) return 1;  
  return 0;
}

int main ( int argc, char * argv [] )
{
  twoWayDistance ( 20000, 1000 );
  assert ( smallDiff ( twoWayDistance ( 800, 1000 ), 400.000000 ) );
  assert ( smallDiff ( twoWayDistance ( 2000, 1000 ), 750.000000 ) );
  assert ( smallDiff ( twoWayDistance ( 20000, 1000 ), 1798.869829 ) );
  assert ( smallDiff ( twoWayDistance ( 200000, 1000 ), 2939.015474 ) );
  assert ( smallDiff ( twoWayDistance ( 2000000, 1000 ), 4089.184052 ) );
  assert ( smallDiff ( twoWayDistance ( 20000, 100 ), 293.901547 ) );
  assert ( smallDiff ( twoWayDistance ( 20000, 10000 ), 7500.000000 ) );
  assert ( smallDiff ( twoWayDistance ( 4365, 1000 ), 1078.166667 ) );
  return 0;
}
#endif /* __PROGTEST__ */
