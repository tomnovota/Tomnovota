#include "CLoadTable.h"

CLoadTable::CLoadTable ( CTable & table, std::string horizontal_line, std::string vertical_line )
 : m_Table ( table )
 , m_Parser ( m_Table )
 , m_Horizontal_Line ( horizontal_line )
 , m_Vertical_Line ( vertical_line )
 {}

void CLoadTable::Load ( const std::string & file_name )
{
    // reset loader
    m_File . clear();
    m_File . open ( file_name );
    if ( ! m_File . is_open() )
    {
        m_File . close();
        throw std::invalid_argument ( OPEN_FILE_WARNING );
    }

    if ( ! ReadLine() || ! CheckHeader() || ! CheckLineSep() )
    {
        m_File . close();
        throw std::invalid_argument ( "Incorrect header" );
    }

    for ( size_t row = 0; row < m_Table . Size(); row ++ )
    {
        if ( ! ReadLine() || ! LoadLine ( row ) || ! CheckLineSep() )
        {
            m_File . close();
            throw std::invalid_argument ( "Incorrect table format" );
        }
    }
}

bool CLoadTable::ReadLine ( void )
{
    std::string line;
    if ( ! std::getline ( m_File, line ) )
        return false;

    m_Line . clear();
    m_Line . str ( line );
    return true;
}

bool CLoadTable::CheckHeader ( void )
{
    m_Col_Widths . clear();
    m_Row_Len = 0;
    std::string separator, col_name, pattern_name;
    size_t col_width, table_size = 0;
    while ( m_Line >> separator && separator == m_Vertical_Line ) // read separator
    {
        col_width = static_cast< size_t > ( m_Line . tellg() ) - m_Row_Len; // current column width
        m_Col_Widths . push_back ( col_width );
        m_Row_Len += col_width; // overall size of len
        if ( ! ( m_Line >> col_name ) ) // line ends with separator
        {
            PrepareParameters ( table_size );
            return true;
        }
        if ( col_name != CNameCounter::NextCol ( pattern_name ) )
            return false; // wrong col name
        
        table_size ++; // overall size of table
    }
    return false; // wrong separator
}

void CLoadTable::PrepareParameters ( size_t table_size )
{
    m_Table . Expand ( table_size );
    m_Numbering_Width = m_Col_Widths . front(); // extract numbering width
    m_Col_Widths . pop_front();                 // and erase it from column widths
}

bool CLoadTable::CheckLineSep ( void )
{
    if ( ! ReadLine() )
        return false;

    char separator;
    size_t count = 0;
    while ( m_Line >> separator && separator == m_Horizontal_Line [0] )
        count ++;

    return m_Row_Len == count && ! m_Line;
}

bool CLoadTable::CheckRowNumber ( size_t row )
{
    std::string separator;
    size_t num = 0;
    return m_Line >> num
        && m_Line >> separator
        && num == row + 1
        && separator == m_Vertical_Line
        && static_cast< size_t > ( m_Line . tellg() ) == m_Numbering_Width;
}

bool CLoadTable::LoadLine ( size_t row )
{
    if ( ! CheckRowNumber ( row ) )
        return false;

    size_t curr_width, prev_width = m_Line . tellg();
    std::string expr, tmp;
    for ( size_t col = 0; col < m_Table . Size(); col ++ )
    {
        expr . clear();
        while ( m_Line >> tmp )
        {
            if ( tmp == m_Vertical_Line ) // end of cell
            {
                curr_width = static_cast< size_t > ( m_Line . tellg() ) - prev_width;
                if ( curr_width != m_Col_Widths [ col ] ) // not-matching column width
                    return false;

                prev_width = m_Line . tellg();
                if ( ! expr . empty() )
                {
                    std::stringstream ss ( expr );
                    m_Table . Edit ( TPosition ( row, col ), m_Parser . ParseExpression ( ss ) );
                }
                break; // parsing cell ended
            }
            expr += " " + tmp;
        }
    }
    return prev_width == m_Row_Len; // check if row len match
}
