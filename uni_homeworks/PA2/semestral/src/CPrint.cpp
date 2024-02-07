#include "CPrint.h"

CPrint::CPrint ( CTable & table, std::string horizontal_line, std::string vertical_line )
 : m_Table ( table )
 , m_Horizontal_Line ( horizontal_line )
 , m_Vertical_Line ( vertical_line )
 {}

CPrintFormula::CPrintFormula ( CTable & table, std::string horizontal_line, std::string vertical_line )
 : CPrint ( table, horizontal_line, vertical_line ) {}

CPrintValue::CPrintValue ( CTable & table, std::string horizontal_line, std::string vertical_line )
 : CPrint ( table, horizontal_line, vertical_line ) {}

std::string CPrintFormula::GetContent ( TPosition pos ) const
{
    return std::move ( m_Table . At ( pos ) -> GetFormula() );
}

std::string CPrintValue::GetContent ( TPosition pos ) const
{
    return std::move ( m_Table . At ( pos ) -> GetValue() );
}

void CPrint::operator () ( std::ostream & os, TRange range ) const
{
    size_t max_existing_row = std::min ( m_Table . Size() - 1, range . m_To . m_Row );
    size_t max_existing_col = std::min ( m_Table . Size() - 1, range . m_To . m_Col );
    size_t not_expanded_part_row = std::max ( m_Table . Size(), range . m_From . m_Row );
    size_t not_expanded_part_col = std::max ( m_Table . Size(), range . m_From . m_Col );

    std::vector< size_t > max_cols;
    size_t max_number = 1, row_len = 0;
    std::string col_name, content;

    for ( size_t i = 0; i < range . m_From . m_Col; i ++ ) // initial col value
        CNameCounter::NextCol ( col_name );
    size_t row_ctr = range . m_From . m_Row; // inicital row value

    CountMaxes ( range, max_cols, max_number, row_len );

    PrintContent ( os, "", max_number ); // left upper corner
    for ( size_t i = 0, col = range . m_From . m_Col; col <= range . m_To . m_Col; col ++, i ++ )
        PrintContent ( os, CNameCounter::NextCol ( col_name ), max_cols [ i ] ); // column labels
    PrintLine ( os, row_len );

    for ( size_t row = range . m_From . m_Row; row <= max_existing_row; row ++ ) // expanded part
    {
        size_t i = 0;
        PrintContent ( os, CNameCounter::NextRow ( row_ctr ), max_number ); // row label
        for ( size_t col = range . m_From . m_Col; col <= max_existing_col; col ++, i ++ ) // expanded part
            PrintContent ( os, GetContent ( TPosition ( row, col ) ), max_cols [ i ] ); // cell

        for ( size_t col = not_expanded_part_col; col <= range . m_To . m_Col; col ++, i ++ ) // not expanded part
            PrintContent ( os, "", max_cols [ i ] ); // cell empty

        PrintLine ( os, row_len ); // line finished
    }
    for ( size_t row = not_expanded_part_row; row <= range . m_To . m_Row; row ++ ) // not expanded part
    {
        PrintContent ( os, CNameCounter::NextRow ( row_ctr ), max_number ); // row label
        for ( size_t i = 0, col = range . m_From . m_Col; col <= range . m_To . m_Col; col ++, i ++ )
            PrintContent ( os, "", max_cols [ i ] ); // cell empty
        PrintLine ( os, row_len );
    }
}

void CPrint::CountMaxes ( TRange range, std::vector< size_t > & max_cols, size_t & max_number, size_t & row_len ) const
{
    size_t tmp, max;
    std::string col_name;
    for ( size_t i = 0; i < range . m_From . m_Col; i ++ ) // nastavení sloupce na počátení hodnotu
        CNameCounter::NextCol ( col_name );


    for ( size_t col = range . m_From . m_Col; col <= range . m_To . m_Col; col ++ )
    {
        max = CNameCounter::NextCol ( col_name ) . size();
        if ( col < m_Table . Size() ) // part of existing table
            for ( size_t row = range . m_From . m_Row; row <= std::min ( range . m_To . m_Row, m_Table . Size() - 1 ); row ++ )
                max = std::max ( max, GetContent ( TPosition ( row, col ) ) . size() );

        max_cols . push_back ( max );
        row_len += max + CELL_GAP_SIZE;
    }
    tmp = range . m_To . m_Row + 1; // row numbering is shifted by 1
    for ( ; tmp / 10 > 0; tmp /= 10, max_number ++ ); // computes max length of numbering
    row_len += max_number + CELL_GAP_SIZE - 1; // adding numbering length
}

void CPrint::PrintContent ( std::ostream & os, const std::string & content, size_t gap ) const
{
    os << content;
    PrintSpaces ( os, gap - content . size() );
    os << " " << m_Vertical_Line << " ";
}

void CPrint::PrintLine ( std::ostream & os, size_t row_len ) const
{
    os << std::endl;
    for ( size_t i = 0; i < row_len; i ++ )
        os << m_Horizontal_Line;

    os << std::endl;
}

void CPrint::PrintSpaces ( std::ostream & os, size_t n )
{
    for ( size_t i = 0; i < n; i ++ )
        os << " ";
}

