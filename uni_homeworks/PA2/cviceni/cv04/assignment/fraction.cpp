#include "fraction.h"

namespace {
  /**
   * Find the Greatest Common Divisor using the Euclidean algorithm (BI-DML)
   */
  int gcd(int a, int b)
  {
    if (a == 0)
    {
      return b;
    }
    if (b == 0)
    {
      return a;
    }
    int r = a % b;
    return gcd(b, r);
  }
}

  // TODO: TASK 1 - Implement the following methods:
Fraction::Fraction ()
: num(0), den(1)
{

}

Fraction::Fraction ( int numerator, int denominator )
{
    assert ( denominator );
    basicForm ( numerator, denominator );
    num = numerator;
    den = denominator;
}

std::ostream& operator<< ( std::ostream& os, const Fraction& fraction )
{
    ( os << fraction . num << "/" << fraction . den ) . flush();
    return os;
}

  // TODO: TASK 2 - Ensure the fraction is always simplified
  // Hint: Use the provided gcd(int, int) function in the constructor
void Fraction::basicForm ( int & n, int & d ) const
{
    int GCD = gcd ( n, d );
    n /= GCD;
    d /= GCD;
    if ( d < 0 )
    {
        d = - d;
        n = - n;
    }
}

void Fraction::basicForm ()
{
    int GCD = gcd ( num, den );
    num /= GCD;
    den /= GCD;
    if ( den < 0 )
    {
        den = - den;
        num = - num;
    }
}

  // TODO: TASK 3 - Implement the following methods:
Fraction& Fraction::operator+= ( const Fraction& other )
{
    int GCD = gcd ( den, other . den );
    num = num * ( other . den / GCD ) + other . num * ( den / GCD );
    den = den * ( other . den / GCD );
    basicForm ();
    return *this;
}

Fraction Fraction::operator- () const
{
    return Fraction ( - num, den );
}

Fraction Fraction::operator+ ( const Fraction& other ) const
{
    Fraction tmp ( num, den );
    return tmp += other;
}

// Hint: Do not copy code from op+=, rather call operator+=
Fraction Fraction::operator- ( const Fraction& other ) const
{
    Fraction tmp ( num, den );
    return tmp += ( - other );
}

// Hint: Consider using unary minus
Fraction Fraction::operator* ( const Fraction& other ) const
{
    return Fraction ( num * other . num, den * other . den );
}

Fraction Fraction::operator/ ( const Fraction& other ) const
{
    return Fraction ( num * other . den, den * other . num );
}


// TODO: TASK 4 - Implement the following methods:
Fraction::Fraction ( int number )
: num(number), den(1)
{

}

bool Fraction::operator< ( const Fraction& other ) const
{
    int GCD = gcd ( den, other . den );
    int num1 = num * ( other . den / GCD );
    int num2 = other . num * ( den / GCD );
    return num1 < num2;
}

bool Fraction::operator== ( const Fraction& other ) const
{
    return ! ( *this < other || other < *this );
}
// Hint: use op<
bool Fraction::operator<= ( const Fraction& other ) const
{
    return *this < other || *this == other;
}
// Hint: use op< and op==
bool Fraction::operator> ( const Fraction& other ) const
{
    return ! ( *this <= other );
}
// Hint: use op<
bool Fraction::operator!= ( const Fraction& other ) const
{
    return ! ( *this == other );
}
 // Hint: use op==
bool Fraction::operator>= ( const Fraction& other ) const
{
    return ! ( *this < other );
}
 // Hint: use op<

  // TODO: TASK 5 - Implement the following method:
std::istream& operator>> ( std::istream& is, Fraction& fraction )
{
    int num;
    int den;
    char c;
    is >> num >> c >> den;
    assert ( c == '/' && is . good() );
    fraction . num = num;
    fraction . den = den;
    return is;
}


// TODO: TASK 6 - Implement the following method:
Fraction::operator double() const
{
    return num / den;
}

// TODO: TASK 7 - Make the test work (but don't "hardcode" operator+/operator== for int and Fraction)

// TODO: TASK 8 - Implement the following methods:
Fraction& Fraction::operator++ ()
{
    num += den;
    return *this;
}

Fraction Fraction::operator++ ( int )
{
    int tmp = num;
    num += den;
    return Fraction ( tmp, den );
}
