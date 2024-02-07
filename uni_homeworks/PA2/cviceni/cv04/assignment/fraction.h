#ifndef FRACTION_H
#define FRACTION_H

#include <iostream>
#include <cassert>

class Fraction {
public:
  // TODO: TASK 1 - Implement the following methods:
  Fraction();
  Fraction(int numerator, int denominator);
  friend std::ostream& operator<<(std::ostream& os, const Fraction& fraction);

  // TODO: TASK 2 - Ensure the fraction is always simplified
  // Hint: Use the provided gcd(int, int) function in the constructor

  // TODO: TASK 3 - Implement the following methods:
  Fraction& operator+=(const Fraction& other);
  Fraction operator-() const;
  Fraction operator+(const Fraction& other) const; // Hint: Do not copy code from op+=, rather call operator+=
  Fraction operator-(const Fraction& other) const; // Hint: Consider using unary minus
  Fraction operator*(const Fraction& other) const;
  Fraction operator/(const Fraction& other) const;

  // TODO: TASK 4 - Implement the following methods:
  Fraction(int number);
  bool operator<(const Fraction& other) const;
  bool operator==(const Fraction& other) const; // Hint: use op<
  bool operator<=(const Fraction& other) const; // Hint: use op< and op==
  bool operator>(const Fraction& other) const;  // Hint: use op<
  bool operator!=(const Fraction& other) const; // Hint: use op==
  bool operator>=(const Fraction& other) const; // Hint: use op<

  // TODO: TASK 5 - Implement the following method:
  friend std::istream& operator>>(std::istream& is, Fraction& fraction);

  // TODO: TASK 6 - Implement the following method:
  explicit operator double() const;

  // TODO: TASK 7 - Make the test work (but don't "hardcode" operator+/operator== for int and Fraction)

  // TODO: TASK 8 - Implement the following methods:
  Fraction& operator++();
  Fraction operator++(int);

private:
    // Variables
    int num;
    int den;

    // Methods
    void basicForm ( int & n, int & d ) const;
    void basicForm ();
};

#endif
