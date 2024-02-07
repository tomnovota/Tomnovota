#ifndef CVICENI_8_CCIRCLE_H
#define CVICENI_8_CCIRCLE_H

#include "CShape.h"


class CCircle : public CShape {
    double x;
public:
    virtual double getCircumference() const;
    virtual double getContent() const;
    virtual CShape* clone() const
    {
        return new CCircle ( *this );
    }

    [[nodiscard]] double getX() const { return x; }
    void setX(double X) { x = X; }
    explicit CCircle(double x) : x(x) {}
};


#endif //CVICENI_8_CCIRCLE_H
