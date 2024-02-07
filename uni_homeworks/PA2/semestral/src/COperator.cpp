#include "COperator.h"

size_t CUnaryOperator::OperandCount ( void ) const
{
    return 1;
}

size_t CBinaryOperator::OperandCount ( void ) const
{
    return 2;
}

void CUnaryOperator::SetOperands ( std::deque< std::unique_ptr< CExpression > > & operands )
{
    if ( operands . size() < OperandCount() )
        throw std::invalid_argument ( "Wrong operand count (expected: " + std::to_string ( OperandCount() ) + ")" );

    m_Operand = std::move ( operands . back() );
    operands . pop_back();
}

void CBinaryOperator::SetOperands ( std::deque< std::unique_ptr< CExpression > > & operands )
{
    if ( operands . size() < OperandCount() )
        throw std::invalid_argument ( "Wrong operand count (expected: " + std::to_string ( OperandCount() ) + ")" );

    m_Operand2 = std::move ( operands . back() );
    operands . pop_back();

    m_Operand1 = std::move ( operands . back() );
    operands . pop_back();
}

CValue CPar::Evaluate ( void ) const
{
    return m_Operand -> Evaluate();
}

CValue CFunctionAbs::Evaluate ( void ) const
{
    int res;
    try
    {
        CValue val = m_Operand -> Evaluate();
        if ( val . Error() || val . Empty() )
            return val;

        res = val . GetNumber();
    }
    catch ( std::exception & ex )
    {
        return CValue ( true );
    }
    return res >= 0 ? res : - res;
}

CValue CFunctionLower::Evaluate ( void ) const
{
    std::string res;
    try
    {
        CValue val = m_Operand -> Evaluate();
        if ( val . Error() || val . Empty() )
            return val;

        res = val . GetString();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true );
    }
    std::transform ( res . begin(), res . end(), res . begin(), 
                        [] ( unsigned char c ) { return std::tolower ( c ); } );
    return res;
}

CValue CFunctionUpper::Evaluate ( void ) const
{
    std::string res;
    try
    {
        CValue val = m_Operand -> Evaluate();
        if ( val . Error() || val . Empty() )
            return val;

        res = val . GetString();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true );
    }
    std::transform ( res . begin(), res . end(), res . begin(),
                        [] ( unsigned char c ) { return std::toupper ( c ); } );
    return res;
}

/* Number addition. */
CValue COperatorPlus::Evaluate ( void ) const
{
    int res;
    try
    {
        CValue val1 = m_Operand1 -> Evaluate();
        CValue val2 = m_Operand2 -> Evaluate();
        if ( val1 . Error() || val1 . Empty() )
            return val1;

        if ( val2 . Error() || val2 . Empty() )
            return val2;

        res = val1 . GetNumber() + val2 . GetNumber();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true );
    }
    return res;
}

/* Number subtraction. */
CValue COperatorMinus::Evaluate ( void ) const
{
    int res;
    try
    {
        CValue val1 = m_Operand1 -> Evaluate();
        CValue val2 = m_Operand2 -> Evaluate();
        if ( val1 . Error() || val1 . Empty() )
            return val1;

        if ( val2 . Error() || val2 . Empty() )
            return val2;

        res = val1 . GetNumber() - val2 . GetNumber();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true );
    }
    return res;
}

/* Number multiplication. */
CValue COperatorMulti::Evaluate ( void ) const
{
    int res;
    try
    {
        CValue val1 = m_Operand1 -> Evaluate();
        CValue val2 = m_Operand2 -> Evaluate();
        if ( val1 . Error() || val1 . Empty() )
            return val1;

        if ( val2 . Error() || val2 . Empty() )
            return val2;

        res = val1 . GetNumber() * val2 . GetNumber();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true );
    }
    return res;
}

/* Number division. */
CValue COperatorDiv::Evaluate ( void ) const
{
    int num, denum;
    try
    {
        CValue val1 = m_Operand1 -> Evaluate();
        CValue val2 = m_Operand2 -> Evaluate();
        if ( val1 . Error() || val1 . Empty() )
            return val1;

        if ( val2 . Error() || val2 . Empty() )
            return val2;

        num = val1 . GetNumber();
        denum = val2 . GetNumber();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true ); // division by 0
    }
    return denum == 0 ? CValue ( true ) : num / denum;
}

/* Concatenate 2 texts into one. */
CValue COperatorConcat::Evaluate ( void ) const
{
    std::string res;
    try
    {
        CValue val1 = m_Operand1 -> Evaluate();
        CValue val2 = m_Operand2 -> Evaluate();
        if ( val1 . Error() || val1 . Empty() )
            return val1;

        if ( val2 . Error() || val2 . Empty() )
            return val2;

        res = val1 . GetString() + val2 . GetString();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true );
    }
    return res;
}

/* Repeats given text multiple times. */
CValue COperatorRepeat::Evaluate ( void ) const
{
    std::string text;
    int count;
    try
    {
        CValue val1 = m_Operand1 -> Evaluate();
        CValue val2 = m_Operand2 -> Evaluate();
        if ( val1 . Error() || val1 . Empty() )
            return val1;

        if ( val2 . Error() || val2 . Empty() )
            return val2;

        text = m_Operand1 -> Evaluate() . GetString();
        count = m_Operand2 -> Evaluate() . GetNumber();
    }
    catch ( const std::exception & ex )
    {
        return CValue ( true );
    }
    if ( count <= 0 )
        return CValue ( true );

    std::string res;
    for ( int i = 0; i < count; i ++ )
        res += text;

    return res;
}

int CPar::Precedence ( void ) const
{
    return 0; // par is always put into operator stack in shunting yard, and should be released only after right par
}

int CFunctionAbs::Precedence ( void ) const
{
    return 3;
}

int CFunctionLower::Precedence ( void ) const
{
    return 3;
}

int CFunctionUpper::Precedence ( void ) const
{
    return 3;
}

int COperatorPlus::Precedence ( void ) const
{
    return 2;
}

int COperatorMinus::Precedence ( void ) const
{
    return 2;
}

int COperatorMulti::Precedence ( void ) const
{
    return 3;
}

int COperatorDiv::Precedence ( void ) const
{
    return 3;
}

int COperatorConcat::Precedence ( void ) const
{
    return 2;
}

int COperatorRepeat::Precedence ( void ) const
{
    return 2;
}