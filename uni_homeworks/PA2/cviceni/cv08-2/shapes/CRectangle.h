#ifndef CVICENI_8_CRECTANGLE_H
#define CVICENI_8_CRECTANGLE_H

#include "CShape.h"


class CRectangle : public CShape {
    double x, y;
public:
    double getCircumference() const;
    double getContent() const;

    virtual CShape* clone() const
    {
        return new CRectangle ( *this );
    }

    [[nodiscard]] double getX() const { return x; }
    void setX(double X) { x = X; }
    [[nodiscard]] double getY() const { return y; }
    void setY(double Y) { y = Y; }
    CRectangle(double x, double y) : x(x), y(y) {}

    bool operator==(const CRectangle &rhs) const { return getX() == rhs.getX() && getY() == rhs.getY(); }
    bool operator!=(const CRectangle &rhs) const { return !(rhs == *this); }
};


#endif //CVICENI_8_CRECTANGLE_H
