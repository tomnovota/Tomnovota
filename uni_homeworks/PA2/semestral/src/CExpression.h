#ifndef CEXPRESSION_H
#define CEXPRESSION_H

#include "Helpers.h"

/**
 * @class CExpression
 * @brief Abstract class representing node in a tree representing expression in a cell.
*/
class CExpression
{
  public:
    CExpression ( void ) = default;
    virtual ~CExpression ( void ) = default;

    /**
     * @brief Abstract method evaluating current expression node.
     * @return current value of node.
    */
    virtual CValue Evaluate ( void ) const = 0;
};

#endif // CEXPRESSION_H
