#include "CCircle.h"

const double PI = 3.14;

double CCircle::getCircumference() const
{
    return 2 * PI * x;
}

double CCircle::getContent() const
{
    return PI * x * x;
}