#ifndef CVICENI_8_CTRIANGLE_H
#define CVICENI_8_CTRIANGLE_H

#include "CShape.h"


class CTriangle : public CShape {
    double x, y, z;
public:
    double getCircumference() const;
    double getContent() const;
    virtual CShape* clone() const
    {
        return new CTriangle ( *this );
    }

    [[nodiscard]] double getX() const { return x; }
    void setX(double X) { x = X; }
    [[nodiscard]] double getY() const { return y; }
    void setY(double Y) { y = Y; }
    [[nodiscard]] double getZ() const { return z; }
    void setZ(double Z) { z = Z; }
    CTriangle(double x, double y, double z) : x(x), y(y), z(z) {}
};


#endif //CVICENI_8_CTRIANGLE_H
