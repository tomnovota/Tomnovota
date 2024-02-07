#include "COperand.h"

    /* ======= OPERAND CELL ======= */

COperandCell::COperandCell ( std::shared_ptr< CCell > cell )
{
    m_Cell = cell;
}

bool COperandCell::ValueReady ( void ) const
{
    return false;    
}

CValue COperandCell::Evaluate ( void ) const
{
    return m_Cell -> Value();
}

    /* ======= OPERAND VALUE ======= */

COperandValue::COperandValue ( int src )
{
    m_Value = src;
}

COperandValue::COperandValue ( std::string src )
{
    m_Value = src;
}

bool COperandValue::ValueReady ( void ) const
{
    return true;
}

CValue COperandValue::Evaluate ( void ) const
{
    return m_Value;
}

