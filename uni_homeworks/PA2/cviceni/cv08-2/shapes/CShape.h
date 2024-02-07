#ifndef CSHAPE_H

#define CSHAPE_H 1

class CShape
{
  public:
    virtual double getCircumference() = delete;
    virtual double getContent() = delete;
    virtual CShape* clone() = delete;
};


#endif // CSHAPE_H