#include <cassert>
#include <iostream>
#include <sstream>

#include "fraction.h"

#if TEST >= 1
bool expect(const Fraction& frac, const std::string& expected)
{
  std::ostringstream oss;
  oss << frac;
  auto actual = oss.str();

  if (actual != expected)
  {
    std::cout << "Actual  : " << actual << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    return false;
  }

  return true;
}
#endif

int main()
{
  std::cout << " -- Tutorial 4 test set START --" << std::endl;

#if TEST >= 1 && TEST <= 8
  std::cout << " -- Flag TEST=" << TEST << " enabled --" << std::endl;
#else
  std::cout << " -- No test flag enabled. Set a test flag at line 4 of the "
    "Makefile to TEST=[1-8]. --"
    << std::endl;
#endif

#if TEST >= 1
  // no-args constructor
  assert(expect(Fraction(), "0/1"));

  // args constructor
  assert(expect(Fraction(1, 3), "1/3"));
  assert(expect(Fraction(23, 4), "23/4"));
  assert(expect(Fraction(-2, 3), "-2/3"));
#endif

#if TEST >= 2
  assert(expect(Fraction(-3, 5), "-3/5"));
  assert(expect(Fraction(-6, -7), "6/7"));
  assert(expect(Fraction(2, 4), "1/2"));
  assert(expect(Fraction(4, 6), "2/3"));
#endif

#if TEST >= 3
  // operator +=
  {
    Fraction f1(1, 1);
    Fraction f2(2, 1);
    Fraction f3(3, 1);
    f1 += f2 += f3;

    assert(expect(f3, "3/1"));
    assert(expect(f2, "5/1"));
    assert(expect(f1, "6/1"));
  }

  // operator - (unary)
  assert(expect(-Fraction(2, 3), "-2/3"));
  assert(expect(-Fraction(-2, 3), "2/3"));

  // operator +
  assert(expect(Fraction(0, 1) + Fraction(1, 3), "1/3"));
  assert(expect(Fraction(1, 3) + Fraction(1, 3), "2/3"));
  assert(expect(Fraction(1, 3) + Fraction(23, 4), "73/12"));
  assert(expect(Fraction(1, 4) + Fraction(1, 4), "1/2"));
  assert(expect(Fraction(1, 5) + Fraction(-1, 5), "0/1"));

  // operator -
  assert(expect(Fraction(2, 3) - Fraction(1, 3), "1/3"));
  assert(expect(Fraction(1, 3) - Fraction(1, 4), "1/12"));
  assert(expect(Fraction(1, 5) - Fraction(0, 1), "1/5"));

  // operator *
  assert(expect(Fraction(1, 2) * Fraction(1, 6), "1/12"));
  assert(expect(Fraction(2, 3) * Fraction(4, -9), "-8/27"));
  assert(expect(Fraction(-2, 3) * Fraction(4, 9), "-8/27"));
  assert(expect(Fraction(-1, 3) * Fraction(1, -3), "1/9"));

  // operator /
  assert(expect(Fraction(1, 2) / Fraction(2, 3), "3/4"));
  assert(expect(Fraction(1, 2) / Fraction(-2, 3), "-3/4"));
  assert(expect(Fraction(-1, 2) / Fraction(2, 3), "-3/4"));
  assert(expect(Fraction(-1, 2) / Fraction(-2, 3), "3/4"));
  assert(expect(Fraction(2, 3) / Fraction(3, 9), "2/1"));
#endif

#if TEST >= 4
  // implicit cast
  assert(Fraction(4, 1) == 4);
  // operator <
  assert(Fraction(1, 2) < Fraction(3, 4));
  assert(!(Fraction(3, 4) < Fraction(1, 2)));
  assert(!(Fraction(1, 2) < Fraction(1, 2)));
  assert(Fraction(1, 3) < Fraction(1, 2));
  // operator <=
  assert(Fraction(1, 2) <= Fraction(3, 4));
  assert(!(Fraction(3, 4) <= Fraction(1, 2)));
  assert(Fraction(1, 2) <= Fraction(1, 2));
  // operator == and !=
  assert(!(Fraction(1, 2) == Fraction(3, 4)));
  assert(Fraction(1, 2) == Fraction(1, 2));
  assert(Fraction(1, 2) != Fraction(3, 4));
  assert(Fraction(3, 4) != Fraction(1, 2));
  // operator >=
  assert(!(Fraction(1, 2) >= Fraction(3, 4)));
  assert(Fraction(3, 4) >= Fraction(1, 2));
  assert(Fraction(1, 2) >= Fraction(1, 2));
  // operator >
  assert(!(Fraction(1, 2) > Fraction(3, 4)));
  assert(Fraction(3, 4) > Fraction(1, 2));
  assert(!(Fraction(1, 2) > Fraction(1, 2)));
#endif

#if TEST >= 5
  std::stringstream ss;

  // Test valid input
  ss.str("2/3");
  Fraction f1;
  ss >> f1;
  assert(f1 == Fraction(2, 3));

  // Test invalid input
  ss.clear();  // Clear the fail bit from the previous test
  ss.str("invalid");
  Fraction f2;
  ss >> f2;
  assert(ss.fail());  // Check that the fail bit is set

  // Test input with whitespace
  ss.clear();
  ss.str(" 4 / 5 ");
  Fraction f3;
  ss >> f3;
  assert(f3 == Fraction(4, 5));

  // Test normalize input
  ss.clear();
  ss.str("-2/-4");
  Fraction f4;
  ss >> f4;
  assert(expect(f4, "1/2"));

  // Test do not overwrite object on invalid inputs
  ss.clear();
  ss.str("-2/pls");
  ss >> f4;
  assert(ss.fail());
  assert(f4 == Fraction(1, 2));
#endif

#if TEST >= 6
  // operator double()
  assert(static_cast<double>(Fraction(1, 2)) == 0.5);
  assert(static_cast<double>(Fraction(3, 4)) == 0.75);
  assert(static_cast<double>(Fraction(2, 1)) == 2.0);
#endif

#if TEST >= 7
  assert(4 == Fraction(4, 1));
  assert(2 + Fraction(1) == 3);
#endif

#if TEST >= 8
  {
    Fraction f(1, 2);
    f++;
    assert(expect(f, "3/2"));

    ++f;
    assert(expect(f, "5/2"));

    f = Fraction(1, 2);
    assert(expect(f++ + Fraction(2, 1), "5/2"));
    assert(expect(f, "3/2"));

    f = Fraction(1, 2);
    assert(expect(++f + Fraction(2, 1), "7/2"));
    assert(expect(f, "3/2"));
  }
#endif

  std::cout << " -- Tutorial 4 test set END --" << std::endl;
  return 0;
}
