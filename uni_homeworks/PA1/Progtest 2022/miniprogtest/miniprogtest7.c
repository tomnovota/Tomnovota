#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */

int getPos ( int len, int rotate, int x )
{
    x += rotate;
    if ( x >= len )
        x %= len;
    if ( x < 0 )
        x += len;
    return x;
}

void rotateArray ( int array [], int arrayLen, int rotateBy )
{
    rotateBy = rotateBy % arrayLen;
    if ( ! rotateBy )
        return;

    int prev = 0;
    int next = getPos ( arrayLen, rotateBy, prev );
    int tmp;
    int index = prev;
    int tmp_prev = array[prev];
    for ( int i = 0; i < arrayLen; i++ )
    {
        tmp = array [next];
        array [next] = tmp_prev;
        tmp_prev = tmp;
        prev = next;
        if ( index == next )
        {
            prev ++;
            tmp_prev = array[prev];
            index = prev;
        }
        next = getPos ( arrayLen, rotateBy, prev );
    }
}

#ifndef __PROGTEST__
int identicalArrays ( const int a[], const int b[], int cnt )
{
    for ( int i = 0; i < cnt; i++ )
        if ( a[i] != b[i] )
            return 0;
    return 1;
}
int main (int argc, char * argv [])
{
  int in0 []  = { 1, 2, 3, 4, 5 };
  int out0 [] = { 5, 1, 2, 3, 4 };

  int in1 []  = { 1, 2, 3, 4, 5 };
  int out1 [] = { 2, 3, 4, 5, 1 };

  int in2 []  = { 1, 2, 3, 4, 5, 6 };
  int out2 [] = { 4, 5, 6, 1, 2, 3 };

  int in3 []  = { 1, 2, 3, 4, 5, 6, 7 };
  int out3 [] = { 3, 4, 5, 6, 7, 1, 2 };

  int in4 []  = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  int out4 [] = { 2, 3, 4, 5, 6, 7, 8, 9, 1 };

  rotateArray ( in0, 5, 1 );
  assert ( identicalArrays ( in0, out0, 5 ) );
  rotateArray ( in3, 7, 12 );
  assert ( identicalArrays ( in3, out3, 7 ) );
  rotateArray ( in1, 5, -1 );
  assert ( identicalArrays ( in1, out1, 5 ) );
  rotateArray ( in2, 6, -3 );
  assert ( identicalArrays ( in2, out2, 6 ) );
  rotateArray ( in4, 9, -100 );
  assert ( identicalArrays ( in4, out4, 9 ) );
  return 0;
}
#endif /* __PROGTEST__ */

