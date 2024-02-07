#ifndef CSHAPEVECTOR_H

#include "CShape.h"
#include <vector>
#include <memory>

#define CSHAPEVECTOR_H


class CShapeVector
{
  public:
    CShapeVector ( void )
    {
        m_Arr . 
    }

    CShapeVector& push_back ( const CShape& el )
    {
        m_Arr . push_back ( std::unique_ptr< CShape > ( el . clone() ) );
        return *this;
    }

  private:
    std::vector< std::unique_ptr< CShape > > m_Arr ;

};


#endif // CSHAPEVECTOR_H