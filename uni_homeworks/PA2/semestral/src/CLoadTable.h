#ifndef CLOADTABLE_H
#define CLOADTABLE_H

#include <sstream>
#include <fstream>
#include <string>
#include <deque>
#include <stdexcept>

#include "CTable.h"
#include "CParser.h"
#include "Helpers.h"

/**
 * @class CLoadTable
 * @brief Class ensuring loading table save.
*/
class CLoadTable
{
  public:
    CLoadTable ( CTable & table, std::string horizontal_line, std::string vertical_line );

    /**
     * @brief Interface method beginning table loading.
     * @param[in] file_name Source file of table save.
    */
    void Load ( const std::string & file_name );

  private:
    /**
     * @brief Reads line from file and saves it in m_Line.
     * @return If successfully.
    */
    bool ReadLine ( void );

    /**
     * @brief Checks first read line containing column names and calculates table parameters.
     * @return Whether correct format of header.
    */
    bool CheckHeader ( void );

    /**
     * @brief Checks line separating rows.
     * @return Whether correct format of line separator.
    */
    bool CheckLineSep ( void );

    /**
     * @brief Checks first cell on row containing row number.
     * @param[in] row Currently proccessed row number.
     * @return Whether correct format of row number.
    */
    bool CheckRowNumber ( size_t row );

    /**
     * @brief Reads cell content in cycle and saves it to appropriate position in table.
     * @param[in] row Currently proccessed row number.
     * @return Whether correct format of line.
    */
    bool LoadLine ( size_t row );

    /**
     * @brief Helper function proccessing parameters from header.
     * @param[in] table_size Proccessed table size.
     * @return Desc.
    */
    void PrepareParameters ( size_t table_size );

    // Variables
    CTable & m_Table;
    CParser m_Parser;

    /** Source file of table save. */
    std::ifstream m_File; 

    /** Currently proccessed line */
    std::stringstream m_Line; 

    /** Character length of all rows in table. */
    size_t m_Row_Len; 

    /** Column width of column width numbering. */
    size_t m_Numbering_Width; 

    /** Array of table's column lengths. */
    std::deque< size_t > m_Col_Widths; 

    /** Character separating lines. */
    std::string m_Horizontal_Line; 

    /** Character separating cells. */
    std::string m_Vertical_Line; 
};

#endif // CLOADTABLE_H