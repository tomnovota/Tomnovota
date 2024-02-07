#include "CRectangle.h"

double CRectangle::getCircumference() const
{
    return 2 * x + 2 * y;
}

double CRectangle::getContent() const
{
    return x * y;
}