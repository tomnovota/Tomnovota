#include "CControl.h"

// helper
void PrepareInput ( std::stringstream & ss, std::string & line, std::string & command );

CControl::CControl ( size_t init_size, std::string horizontal_line, std::string vertical_line, std::istream & in_stream, std::ostream & out_stream )
 : m_Table ( init_size )
 , m_Loader ( m_Table, horizontal_line, vertical_line )
 , m_Parser ( m_Table )
 , m_Print ( m_Table, horizontal_line, vertical_line )
 , m_PrintF ( m_Table, horizontal_line, vertical_line )
 , m_In_Stream ( in_stream )
 , m_Out_Stream ( out_stream )
 {}

void CControl::Start ( void )
{
    while ( ! EnterDialog() );

    if ( ! m_In_Stream ) // closed instream
        return;

    m_Out_Stream << START_MESSAGE << std::endl;
    while ( true )
    {
        try
        {
            if ( FormTable() )
                break; 
            return; // input ended
        }
        catch ( const std::exception & ex )
        {
            m_Out_Stream << EDIT_TABLE_WARNING << " (" << ex . what() << ")" << std::endl;
        }
    }
    while ( ! ExitDialog() );
}

bool CControl::FormTable ( void )
{
    std::string line, command;
    std::stringstream content;
    while ( m_Out_Stream << ">> " && std::getline ( m_In_Stream, line ) ) // reads line from input
    {
        if ( & m_In_Stream != & std::cin ) // cosmetic printing of current command from different input than cin
            std::cout << line << std::endl;

        PrepareInput ( content, line, command );

        if ( command == QUIT_COMMAND )
            return true; // right way to end program

        else if ( command == PRINT_COMMAND )
        {
            m_Table . Evaluate();
            m_Print ( m_Out_Stream, m_Parser . ParseRange ( content ) );
        }
        else if ( command == PRINTF_COMMAND )
        {
            m_PrintF ( m_Out_Stream, m_Parser . ParseRange ( content ) );
        }
        else if ( command == DELETE_COMMAND )
        {
            m_Table . Delete ( m_Parser . ParseRange ( content ) );
        }
        else
        {
            TPosition cell_pos = m_Parser . ParseCellName ( command );
            m_Table . Edit ( cell_pos, m_Parser . ParseExpression ( content ) );
        }
    }
    return false; // wrong way to end program
}

        /* ======= DIALOG ======= */

bool CControl::EnterDialog ( void )
{
    std::string answer = Dialog ( ENTER_DIALOG );
    if ( answer == ANSWER_YES )
    {
        try
        {
            m_Loader . Load ( Dialog ( FILE_NAME_CALL ) );
        }
        catch ( const std::exception & ex )
        {
            m_Out_Stream << LOAD_TABLE_WARNING << " (" << ex . what() << ")" << std::endl;
            return false;
        }
    }
    else if ( m_In_Stream && answer != ANSWER_NO )
    {
        m_Out_Stream << WRONG_ANSWER_WARNING << ANSWER_YES << " / " << ANSWER_NO << std::endl;
        return false;
    }
    return true;
}

bool CControl::ExitDialog ( void )
{
    std::string answer = Dialog ( EXIT_DIALOG );
    if ( m_In_Stream && answer == ANSWER_YES )
    {
        std::ofstream file ( Dialog ( FILE_NAME_CALL ) );
        if ( ! file . is_open() )
        {
            m_Out_Stream << OPEN_FILE_WARNING << std::endl;
            return false;
        }
        m_PrintF ( file, TRange ( m_Table . Size() ) ); // saves whole Table to given file
    }
    else if ( m_In_Stream && answer != ANSWER_NO )
    {
        m_Out_Stream << WRONG_ANSWER_WARNING << ANSWER_YES << " / " << ANSWER_NO << std::endl;
        return false;
    }
    return true;
}

        /* ======= HELPERS ======= */

void PrepareInput ( std::stringstream & ss, std::string & line, std::string & command )
{
        ss . clear(); // reset stream
        ss . str ( line ); // insert remaining into stringstream
        ss >> command >> std::ws; // load command
        ss . clear(); // reset stream
        ss . str ( std::string ( line . begin() + ss . tellg (), line . end() ) ); // insert remain of command into stream
}

std::string CControl::Dialog ( const char * message )
{
    std::string answer;
    m_Out_Stream << message << std::endl;
    m_Out_Stream << "> ";
    std::getline ( m_In_Stream, answer );
    if ( & m_In_Stream != & std::cin ) // cosmetic printing of current command from different input than cin
        std::cout << answer << std::endl;
    
    return answer;
}
