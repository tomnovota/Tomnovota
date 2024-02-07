#ifndef COPERATOR_H
#define COPERATOR_H

#include <vector>
#include <deque>
#include <string>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <algorithm>

#include "CExpression.h"

/**
 * @class COperator
 * @brief Abstract class representing operator in expression tree.
*/
class COperator : public CExpression
{
  public:
    COperator ( void ) = default;
    virtual ~COperator ( void ) = default;

    /**
     * @brief Abstract method assigning operands to operator and removing them from operand container.
     * @param[in,out] operands Container with operands.
    */
    virtual void SetOperands ( std::deque< std::unique_ptr< CExpression > > & operands ) = 0;

    /**
     * @brief Abstract method getting operand count for operand.
     * @return Operand count.
    */
    virtual size_t OperandCount ( void ) const = 0;

    /**
     * @brief Abstract method representing precedence of operator.
     * @return Precedence of operator in an expression.
    */
    virtual int Precedence ( void ) const = 0;
};

/**
 * @class CUnaryOperator
 * @brief Abstract class representing type of operator with 1 operand.
*/
class CUnaryOperator : public COperator
{
  public:
    CUnaryOperator ( void ) = default;
    virtual ~CUnaryOperator ( void ) = default;

    /**
     * @brief Method assigning 1 operand from operand container into operator.
     * @param[in,out] operands Container with operands.
    */
    void SetOperands ( std::deque< std::unique_ptr< CExpression > > & operands ) override;

    /**
     * @brief Method getting operand count for operand.
     * @return Operand count 1.
    */
    size_t OperandCount ( void ) const override;

  protected:
    /** Operand of operator. */
    std::unique_ptr< CExpression > m_Operand; 
};

/**
 * @class CBinaryOperator
 * @brief Abstract class representing special type of operator with 2 operand.
*/
class CBinaryOperator : public COperator
{
  public:
    CBinaryOperator ( void ) = default;
    virtual ~CBinaryOperator ( void ) = default;

    /**
     * @brief Method assigning 2 operands from operand container into operator. 
     * @param[in,out] operands Container with operands.
    */
    void SetOperands ( std::deque< std::unique_ptr< CExpression > > & operands ) override;

    /**
     * @brief Method getting operand count for operand.
     * @return Operand count 2.
    */
    size_t OperandCount ( void ) const override;

  protected:
    /** First operand. */
    std::unique_ptr< CExpression > m_Operand1; 

    /** Second operand. */
    std::unique_ptr< CExpression > m_Operand2; 
};

/**
 * @class CPar
 * @brief Class representing left parenthesis in expression.
*/
class CPar : public CUnaryOperator
{
  public:
    CPar ( void ) = default;
    ~CPar ( void ) = default;

    /**
     * @brief Method returning value of expression inside parenthesis brackets.
     * @return value of expression inside brackets.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return minimal precedence 0.
    */
    int Precedence ( void ) const override;
};

/**
 * @class CFunctionAbs
 * @brief Class representing absolute value function in expression.
*/
class CFunctionAbs : public CUnaryOperator
{
  public:
    CFunctionAbs ( void ) = default;
    ~CFunctionAbs ( void ) = default;

    /**
     * @brief Method applying absolute value to assigned number expression.
     * @return Positive value of expression.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return maximal precedence 3.
    */
    int Precedence ( void ) const override;
};

/**
 * @class CFunctionLower
 * @brief Class representing lowercase function in expression.
*/
class CFunctionLower : public CUnaryOperator
{
  public:
    CFunctionLower ( void ) = default;
    ~CFunctionLower ( void ) = default;

    /**
     * @brief Method applying lowercase to alphabetic characters in assigned expression.
     * @return Lowercase text.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return maximal precedence 3.
    */
    int Precedence ( void ) const override;
};

/**
 * @class CFunctionUpper
 * @brief Class representing uppercase function in expression.
*/
class CFunctionUpper : public CUnaryOperator
{
  public:
    CFunctionUpper ( void ) = default;
    ~CFunctionUpper ( void ) = default;

    /**
     * @brief Method applying uppercase to alphabetic characters in assigned expression.
     * @return Uppercase text.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return maximal precedence 3.
    */
    int Precedence ( void ) const override;
};

/**
 * @class COperatorPlus
 * @brief Class representing addition in expression.
*/
class COperatorPlus : public CBinaryOperator
{
  public:
    COperatorPlus ( void ) = default;
    ~COperatorPlus ( void ) = default;

    /**
     * @brief Adds assigned number expressions.
     * @return Result of addition.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return precedence 2.
    */
    int Precedence ( void ) const override;
};

/**
 * @class COperatorMinus
 * @brief Class representing subtraction in expression.
*/
class COperatorMinus : public CBinaryOperator
{
  public:
    COperatorMinus ( void ) = default;
    ~COperatorMinus ( void ) = default;

    /**
     * @brief Subtract assigned number expressions.
     * @return Result of subtraction.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return precedence 2.
    */
    int Precedence ( void ) const override;
};

/**
 * @class COperatorMulti
 * @brief Class representing multiplication in expression.
*/
class COperatorMulti : public CBinaryOperator
{
  public:
    COperatorMulti ( void ) = default;
    ~COperatorMulti ( void ) = default;

    /**
     * @brief Multiply assigned number expressions.
     * @return Result of multiplication.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return maximal precedence 3.
    */
    int Precedence ( void ) const override;
};

/**
 * @class COperatorDiv
 * @brief Class division multiplication in expression.
*/
class COperatorDiv : public CBinaryOperator
{
  public:
    COperatorDiv ( void ) = default;
    ~COperatorDiv ( void ) = default;

    /**
     * @brief Divide assigned number expressions.
     * @return Result of division.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return maximal precedence 3.
    */
    int Precedence ( void ) const override;
};

/**
 * @class COperatorConcat
 * @brief Class representing text concatenation in expression.
*/
class COperatorConcat : public CBinaryOperator
{
  public:
    COperatorConcat ( void ) = default;
    ~COperatorConcat ( void ) = default;

    /**
     * @brief Concatenate assigned text expressions.
     * @return Result of concatenation.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return precedence 2.
    */
    int Precedence ( void ) const override;
};

/**
 * @class COperatorRepeat
 * @brief Class representing text repetition in expression.
*/
class COperatorRepeat : public CBinaryOperator
{
  public:
    COperatorRepeat ( void ) = default;
    ~COperatorRepeat ( void ) = default;

    /**
     * @brief Repeats assigned text expression multiple times.
     * @return Result of repetition of first operand second's operand times.
     */
    CValue Evaluate ( void ) const override;

    /**
     * @brief Getting precedence of operator.
     * @return precedence 2.
    */
    int Precedence ( void ) const override;
};

#endif // COPERATOR_H

