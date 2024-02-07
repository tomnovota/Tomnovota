#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include <variant>
#include <stdexcept>

class CCell;
class CExpression;

/**
 * @class CValue
 * @brief Wrapper around variant representing constant value in table.
*/
struct CValue
{
    CValue ( void ) = default;

    /** Initialize value with given number. */
    CValue ( int src );

    /** Initialize value with given text. */
    CValue ( std::string src );

    /** Initialize value as error value. */
    CValue ( bool error );

    /**
     * @brief Getter of string representation of value.
     * @return String representation of value.
    */
    std::string GetValue ( void ) const;

    /**
     * @brief Returns current value if type is text.
     * @return Contained text value.
    */
    std::string GetString ( void ) const;

    /**
     * @brief Returns current value if type is number.
     * @return Contained number value.
    */
    int GetNumber ( void ) const;

    /**
     * @brief Finds out whether was value initialized.
     * @return Whether was value initialized
    */
    bool Empty ( void ) const;

    /**
     * @brief Finds out whether is value error.
     * @return Whether is value error.
    */
    bool Error ( void ) const;

    std::variant< int, std::string > m_Value;

    /** Indicating invalid value. */
    bool m_Error = false; 
    bool m_Empty = true;
};

/**
 * @struct TPosition
 * @brief Wrapper around pair of cell position coordinates in table.
*/
struct TPosition
{
    TPosition ( void ) = default;
    TPosition ( size_t row, size_t col )
     : m_Row ( row ), m_Col ( col ) {}

    /**
     * @brief Alphabetical comparison.
     * @return Whether is position alphabetically less than given position.
    */
    bool operator < ( const TPosition & pos ) const;

    size_t m_Row;
    size_t m_Col;
};

/**
 * @struct TRange
 * @brief Wrapper around pair of cell position coordinates 
*/
struct TRange
{
    TRange ( void ) = default;
    TRange ( TPosition from, TPosition to )
     : m_From ( from ), m_To ( to ) {}

    TRange ( size_t size )
     : m_From ( 0, 0 ), m_To ( size - 1, size - 1 )
     {}

    /** Left upper cell. */
    TPosition m_From;

    /** Right bottom cell. */
    TPosition m_To;
};

/**
 * @struct TCellProperties
 * @brief Wrapper around trio of cell properties assigned to cell when editing.
*/
struct TCellProperties
{
    /** String representation of expression assigned to cell. */
    std::string m_Formula;

    /** Root of tree representing value of expression assigned to cell. */
    std::unique_ptr< CExpression > m_Expression;

    /** List of cell references which is cell dependant on. */
    std::set< TPosition > m_Dependencies;
};

/**
 * @struct TNode
 * @brief Structure representing node in DFS algorithm applied on cells in table.
*/
struct TNode
{
    TNode ( std::set< TPosition > deps )
     : m_Dependancies ( std::vector< TPosition > ( deps . begin(), deps . end() ) )
     {}

    /**
     * @return Whether was node not opened.
    */
    bool Fresh ( void ) { return ! ( m_Opened || m_Closed ); }

    /**
     * @return Whether is node opened.
    */
    bool Open ( void ) { return m_Opened; }

    /**
     * @return Whether is node closed.
    */
    bool Close ( void ) { return m_Closed; }

    void SetOpen ( void ) { m_Opened = true; }

    void SetClose ( void ) { m_Opened = false; m_Closed = true; }

    bool m_Opened = false;
    bool m_Closed = false;

    /** List of dependencies got from appropriate cell. */
    std::vector< TPosition > m_Dependancies; 
};

/**
 * @struct CNameCounter
 * @brief Class implementing column and rows counter for graphical representation of table.
*/
class CNameCounter
{
  public:
    /**
     * Increments col and returns next column name.
    */
    static std::string NextCol ( std::string & col );

    /**
     * Increments row and returns next row name.
    */
    static std::string NextRow ( size_t & row );
};

    /* =============== CONFIGURATION LOADING =============== */

int config_fail ( void );
int config_parameter_fail ( void );

/**
 * @brief Function called to load parameters of table processor.
 * @param[out] init_size Initial size of new table.
 * @param[out] horizontal_line Character separating rows.
 * @param[out] vertical_line Character separating cells.
 * @param[out] in_stream Source of commands for table processor.
 * @param[out] out_stream Destination of processor's output.
 * @param[out] ifs Original file stream which is in_stream referenced to.
 * @param[out] ofs Original file stream which is out_stream referenced to.
 * @param[in] config_file File with configuration.
 * @return Whether loaded successfully.
*/
bool LoadConfig ( size_t & init_size, std::string & horizontal_line, std::string & vertical_line, std::istream *& in_stream, std::ostream *& out_stream, std::ifstream & ifs, std::ofstream & ofs, const std::string & config_file );

/**
 * @brief Set edited parameter value.
 * @param[in] param Parameter name.
 * @param[in] value Value of parameter.
 * @param[out] init_size Initial size of new table.
 * @param[out] horizontal_line Character separating rows.
 * @param[out] vertical_line Character separating cells.
 * @param[out] in_stream Source of commands for table processor.
 * @param[out] out_stream Destination of processor's output.
 * @param[out] ifs Original file stream which is in_stream referenced to.
 * @param[out] ofs Original file stream which is out_stream referenced to.
 * @return Whether successfully.
*/
bool ProcessParam ( std::string param, std::string value, size_t & init_size, std::string & horizontal_line, std::string & vertical_line, std::istream *& in_stream, std::ostream *& out_stream, std::ifstream & ifs, std::ofstream & ofs );

extern const char * QUIT_COMMAND;
extern const char * PRINT_COMMAND;
extern const char * PRINTF_COMMAND;
extern const char * DELETE_COMMAND;

extern const char * START_MESSAGE;
extern const char * ENTER_DIALOG;
extern const char * EXIT_DIALOG;
extern const char * FILE_NAME_CALL;

extern const char * ANSWER_YES;
extern const char * ANSWER_NO;

extern const char * EDIT_TABLE_WARNING;
extern const char * WRONG_ANSWER_WARNING;
extern const char * LOAD_TABLE_WARNING;
extern const char * OPEN_FILE_WARNING;

extern const char COL_MIN;
extern const char COL_MAX;
extern const char COL_RANGE;
extern const char ROW_MIN;
extern const char ROW_MAX;
extern const char TEXT_SYMBOL;
extern const size_t CELL_GAP_SIZE;

extern const char * OPERATOR_PLUS;
extern const char * OPERATOR_MINUS;
extern const char * OPERATOR_MULTI;
extern const char * OPERATOR_DIV;
extern const char * OPERATOR_CONCAT;
extern const char * OPERATOR_REPEAT;
extern const char * FUNCTION_ABS;
extern const char * FUNCTION_LOWER;
extern const char * FUNCTION_UPPER;
extern const char * LEFT_PAR;
extern const char * RIGHT_PAR;
extern const char * ERROR_VALUE;

#endif // HELPERS_H
