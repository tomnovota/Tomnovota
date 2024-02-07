#include "Helpers.h"

const char * QUIT_COMMAND = "quit";
const char * PRINT_COMMAND = "print";
const char * PRINTF_COMMAND = "printf";
const char * DELETE_COMMAND = "delete";

const char * START_MESSAGE = "Edit table:";
const char * ENTER_DIALOG = "Do you want to load existing table?";
const char * EXIT_DIALOG = "Do you want to save table?";
const char * FILE_NAME_CALL = "Enter file name: ";
const char * ANSWER_YES = "yes";
const char * ANSWER_NO = "no";
const char * EDIT_TABLE_WARNING = "Editing table failure";
const char * WRONG_ANSWER_WARNING = "Wrong answer. Options are: ";
const char * LOAD_TABLE_WARNING = "Loading table failed";
const char * OPEN_FILE_WARNING = "Opening file failed";

const size_t CELL_GAP_SIZE = 3;
const char COL_MIN = 'A';
const char COL_MAX = 'Z';
const char COL_RANGE = COL_MAX - COL_MIN + 1;
const char ROW_MIN = '0';
const char ROW_MAX = '9';

const char * OPERATOR_PLUS = "+";
const char * OPERATOR_MINUS = "-";
const char * OPERATOR_MULTI = "*";
const char * OPERATOR_DIV = "/";
const char * OPERATOR_CONCAT = "CONCAT";
const char * OPERATOR_REPEAT = "REPEAT";
const char * FUNCTION_ABS = "ABS";
const char * FUNCTION_LOWER = "LOWER";
const char * FUNCTION_UPPER = "UPPER";
const char * LEFT_PAR = "(";
const char * RIGHT_PAR = ")";
const char TEXT_SYMBOL = '"';
const char * ERROR_VALUE = "#ERROR";

std::vector< std::string > PARAM_NAMES = { "INIT_SIZE", "HORIZONTAL_LINE", "VERTICAL_LINE", "IN_STREAM", "OUT_STREAM" };

CValue::CValue ( int src )
{
    m_Value = src;
    m_Empty = false;
}

CValue::CValue ( std::string src )
{
    m_Value = src;
    m_Empty = false;
}

CValue::CValue ( bool error )
{
    m_Error = true;
    m_Empty = false;
}

std::string CValue::GetValue ( void ) const
{
    try
    {
        return std::get< std::string > ( m_Value );
    }
    catch ( const std::bad_variant_access& ex )
    {
        int tmp = std::get< int > ( m_Value );
        return std::to_string ( tmp );
    }
}

std::string CValue::GetString ( void ) const
{
    try
    {
        return std::get< std::string > ( m_Value );
    }
    catch ( const std::bad_variant_access& ex )
    {
        throw std::invalid_argument ( "Non-numeric operand" );
    }
}

int CValue::GetNumber ( void ) const
{
    try
    {
        return std::get< int > ( m_Value );
    }
    catch ( const std::bad_variant_access& ex )
    {
        throw std::invalid_argument ( "Non-text operand" );
    }
}

bool CValue::Empty ( void ) const
{
    return m_Empty;
}

bool CValue::Error ( void ) const
{
    return m_Error;
}

bool TPosition::operator < ( const TPosition & pos ) const
{
    return m_Col < pos . m_Col
        ? true
        : m_Col > pos . m_Col
        ? false
        : m_Row < pos . m_Row;
}

std::string CNameCounter::NextRow ( size_t & row )
{
    return std::to_string ( ++ row );
}

std::string CNameCounter::NextCol ( std::string & col )
{
    for ( auto i = col . rbegin(); i != col . rend(); i ++ )
    {
        if ( * i != COL_MAX )
        {
            ( * i ) ++;
            return col;
        }
        * i = COL_MIN;
    }
    return col += COL_MIN;
}

int config_parameter_fail ( void )
{
    std::cout << "Expected only 1 config file!" << std::endl;
    return 0;
}

int config_fail ( void )
{
    std::cout << "Incorrect config file" << std::endl;
    return 0;
}

bool ProcessParam ( std::string param, std::string value, size_t & init_size, std::string & horizontal_line, std::string & vertical_line, std::istream *& in_stream, std::ostream *& out_stream, std::ifstream & ifs, std::ofstream & ofs )
{
    if ( param == PARAM_NAMES [0] )
    {
        if ( value . empty() )
        {
            init_size = 26;
            return true;
        }
        try
        {
            size_t size = std::stoull ( value );
            if ( size > 0 )
                return ( init_size = size );
    
            std::cout << "Invalid initial size" << std::endl;
        }
        catch ( const std::exception & ex )
        {
            std::cout << "Invalid initial size" << std::endl;
        }
        return false;
    }
    if ( param == PARAM_NAMES [1] )
    {
        if ( value . size() > 1 )
        {
            std::cout << "Invalid separator" << std::endl;
            return false;
        }
        horizontal_line = value . empty() ? "-" : value;
        return true;
    }
    if ( param == PARAM_NAMES [2] )
    {
        if ( value . size() > 1 )
        {
            std::cout << "Invalid separator" << std::endl;
            return false;
        }
        vertical_line = value . empty() ? "|" : value;
        return true;
    }
    if ( param == PARAM_NAMES [3] )
    {
        if ( value . empty() )
            return ( in_stream = & std::cin );

        ifs . open ( value );
        if ( ! ifs . is_open() )
            return false;

        in_stream = & ifs;
        return true;
    }
    if ( param == PARAM_NAMES [4] )
    {
        if ( value . empty() )
            return ( out_stream = & std::cout );

        ofs . open ( value );
        if ( ! ofs . is_open() )
            return false;

        out_stream = & ofs;
        return true;
    }
    return false;
}

bool LoadConfig ( size_t & init_size, std::string & horizontal_line, std::string & vertical_line, std::istream *& in_stream, std::ostream *& out_stream, std::ifstream & ifs, std::ofstream & ofs, const std::string & config_file )
{
    std::ifstream config ( config_file );
    if ( ! config . is_open() )
        return false;

    std::string line;
    for ( auto param_name : PARAM_NAMES ) // read parameters
    {
        if ( ! std::getline ( config, line ) )
            return false;

        std::stringstream ss ( line );
        std::string param, value;
        if ( ! ( ss >> param ) )
            return false;

        ss >> value;
        if ( param_name != param
            || ss >> value
            || ! ProcessParam ( param, value, init_size, horizontal_line, vertical_line, in_stream, out_stream, ifs, ofs ) )
            return false;
    }
    return true;
}