#include "CTable.h"

CTable::CTable ( size_t init_size )
{
    Expand ( init_size );
}

void CTable::Edit ( TPosition pos, TCellProperties props )
{
    size_t max_pos = std::max ( pos . m_Row, pos . m_Col ) + 1;
    if ( max_pos > Size() ) // edited cell out of rendered range
        Expand ( max_pos );

    m_Arr [ pos . m_Row ] [ pos . m_Col ] -> Edit ( std::move ( props ) );
    if ( DetectCycle ( pos ) )
    {
        m_Arr [ pos . m_Row ] [ pos . m_Col ] -> Delete(); // cycle banned - erasing record
        throw std::logic_error ( "Cycle forbidden" );
    }
}

void CTable::Delete ( TRange range ) const
{
    // deletes only rendered part of table
    for ( size_t row = range . m_From . m_Row; row <= std::min ( range . m_To . m_Row, Size() - 1 ); row ++ ) 
        for ( size_t col = range . m_From . m_Col; col <= std::min ( range . m_To . m_Col, Size() - 1 ); col ++ )
            m_Arr [ row ] [ col ] -> Delete();
}

std::shared_ptr< CCell > CTable::At ( TPosition pos ) const
{
    if ( std::max ( pos . m_Row, pos . m_Col ) >= Size() )
        throw std::logic_error ( "Accessing cell out of table range" );

    return m_Arr [ pos . m_Row ] [ pos . m_Col ];
}

size_t CTable::Size ( void ) const
{
    return m_Arr . size();
}

void CTable::Expand ( size_t new_size )
{
    m_Arr . resize ( new_size );
    for ( auto & row : m_Arr )
    {
        row . resize ( new_size );
        for ( auto & cell : row )
        {
            if ( cell . get() == nullptr ) // adding cells in not pre-existing positions
                cell = std::make_shared< CCell > ();
        }
    }
}

void CTable::Evaluate ( void ) const
{
    auto nodes = ConstructNodes();
    for ( size_t row = 0; row < Size(); row ++ )
        for ( size_t col = 0; col < Size(); col ++ )
            DFS_scan ( TPosition ( row, col ), nodes, true );
}

bool CTable::DetectCycle ( TPosition cell ) const
{
    auto nodes = ConstructNodes();
    if ( ! DFS_scan ( cell, nodes, false ) )
        return true; // cycle detected

    return false; // cycle not detected
}

std::vector< std::vector< TNode > > CTable::ConstructNodes ( void ) const
{
    std::vector< std::vector< TNode > > nodes;
    nodes . resize ( Size() );
    for ( size_t row = 0; row < Size(); row ++ )
        for ( size_t col = 0; col < Size(); col ++ )
            nodes [ row ] . push_back ( m_Arr [ row ] [ col ] -> m_Dependancies );

    return nodes;
}

bool CTable::DFS_scan ( TPosition cell, std::vector< std::vector< TNode > > & nodes, bool eval ) const
{
    TNode & current_node = nodes [ cell . m_Row ] [ cell . m_Col ];
    current_node . SetOpen();
    for ( const auto & pos : ( current_node . m_Dependancies ) )
    {
        if ( nodes [ pos . m_Row ] [ pos . m_Col ] . Fresh()
            && ! DFS_scan ( pos, nodes, eval ) )
            return false; // propagate cycle detection back
        
        else if ( nodes [ pos . m_Row ] [ pos . m_Col ] . Open() )
            return false; // detected cycle
    }
    current_node . SetClose();
    if ( eval )
        At ( cell ) -> Evaluate(); // all dependencies closed and evaluated

    return true;
}