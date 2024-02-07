#ifndef COPERAND_H
#define COPERAND_H

#include <string>
#include <memory>

#include "CExpression.h"
#include "CCell.h"
#include "Helpers.h"

/**
 * @class COperand
 * @brief Abstract class representing operand in expression.
*/
class COperand : public CExpression
{
  public:
    COperand ( void ) = default;
    virtual ~COperand ( void ) = default;

    /**
     * @brief Abstract method returning whether has operand variable or static value.
     * @return whether value ready.
    */
    virtual bool ValueReady ( void ) const = 0;
};

/**
 * @class COperandCell
 * @brief Class representing cell reference in expression.
*/
class COperandCell : public COperand
{
  public:
    COperandCell ( void ) = default;
    COperandCell ( std::shared_ptr< CCell > cell );
    ~COperandCell ( void ) = default;

    /**
     * @brief Cell reference is dynamic value.
     * @return that current node is not ready.
    */
    bool ValueReady ( void ) const override;

    /**
     * @brief Method evaluating cell that this node is referencing to.
     * @return Current value of referenced cell.
    */
    CValue Evaluate ( void ) const override;

  private:
    /** Referenced cell. */
    std::shared_ptr< CCell > m_Cell; 
};

/**
 * @class COperandValue
 * @brief Class representing constant value in Expression.
*/
class COperandValue : public COperand
{
  public:
    COperandValue ( void ) = default;
    ~COperandValue ( void ) = default;

    /**
     * @brief Assigning integer value to this operand.
     * @param[in] src Integer value.
    */
    COperandValue ( int src );

    /**
     * @brief Assigning text value to this operand.
     * @param[in] src Text value.
    */
    COperandValue ( std::string src );

    /**
     * @brief Constant value is static value.
     * @return That this value is ready to be proccessed.
    */
    bool ValueReady ( void ) const override;

    /**
     * @brief Returning constant value that this node contains.
     * @return constant value that this node contains.
    */
    CValue Evaluate ( void ) const override;

  private:
    /** Constant value assigned to this node. */
    CValue m_Value; 
};

#endif // COPERAND_H
