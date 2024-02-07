#include "CCell.h"

void CCell::Edit ( TCellProperties props )
{
    m_Formula = props . m_Formula;
    m_Expression = std::move ( props . m_Expression );
    m_Dependancies = props . m_Dependencies;
}

void CCell::Delete ( void )
{
    m_Formula . clear();
    m_Expression . reset();
    m_Dependancies . clear();
    m_Value = CValue();
}

void CCell::Evaluate ( void )
{
    if ( ! Empty() )
        m_Value = m_Expression -> Evaluate();
}

CValue CCell::Value ( void ) const
{
    return m_Value;
}

std::string CCell::GetFormula ( void ) const
{
    return m_Formula;
}

std::string CCell::GetValue ( void ) const
{
    return m_Value . Empty() ? "" : ( m_Value . Error() ? ERROR_VALUE : m_Value . GetValue() );
}

bool CCell::Empty ( void ) const
{
    return m_Formula . empty();
}