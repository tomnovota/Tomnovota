#ifndef CPRINT_H
#define CPRINT_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "CTable.h"
#include "CCell.h"
#include "Helpers.h"

/**
 * @class CPrint
 * @brief Class providing printing of table range.
*/
class CPrint
{
  public:
    CPrint ( CTable & table, std::string horizontal_line, std::string vertical_line );

    /**
     * @brief Prints to given stream given table range.
     * @param[in] os Destination for printing.
     * @param[in] range Table range to be printed.
    */
    void operator () ( std::ostream & os, TRange range ) const;

    /**
     * @brief Gets cell and table propositions for current printing.
     * @param[in] range Table range to be printed.
     * @param[out] max_cols Array of column widths.
     * @param[out] max_number Column width of numbering column.
     * @param[out] row_len Character length of every row in table.
    */
    void CountMaxes ( TRange range, std::vector< size_t > & max_cols, size_t & max_number, size_t & row_len ) const;

    /**
     * @brief Abstract method getting content to be printed from cell on given position.
     * @param[in] pos Coordinates of cell in table.
     * @return String representation of content in cell.
    */
    virtual std::string GetContent ( TPosition pos ) const = 0;

    /**
     * @brief Prints given cell content and requiered gap.
     * @param[in] os Destination for printing.
     * @param[in] content String to be printed.
     * @param[in] gap Col width of column the current cell is in.
    */
    void PrintContent ( std::ostream & os, const std::string & content, size_t gap ) const;

    /**
     * @brief Prints continuous line with given length.
     * @param[in] os Destination for printing.
     * @param[in] row_len Character length of row in printed table.
    */
    void PrintLine ( std::ostream & os, size_t row_len ) const;

    /**
     * @brief Prints given number times space.
     * @param[in] os Destination for printing.
     * @param[in] n Number of spaces to be printed.
    */
    static void PrintSpaces ( std::ostream & os, size_t n );

  protected:
    CTable & m_Table;
    /** Character separating rows. */
    std::string m_Horizontal_Line; 

    /** Character separating columns. */
    std::string m_Vertical_Line; 
};

/**
 * @class CPrintFormula
 * @brief Class prividing printing of cell formulas in given table range. 
*/
class CPrintFormula : public CPrint
{
  public:
    CPrintFormula ( CTable & table, std::string horizontal_line, std::string vertical_line );

    /**
     * @brief Method getting formula assigned to cell on given position.
     * @param[in] pos Coordinates of cell in table.
     * @return String representation of formula assigned to cell.
    */
    std::string GetContent ( TPosition pos ) const override;
};

/**
 * @class CPrintValue
 * @brief Class prividing printing of cell values in given table range. 
*/
class CPrintValue : public CPrint
{
  public:
    CPrintValue ( CTable & table, std::string horizontal_line, std::string vertical_line );

    /**
     * @brief Method getting value calculated from expression assigned to cell on given position.
     * @param[in] pos Coordinates of cell in table.
     * @return String representation of value assigned to cell.
    */
    std::string GetContent ( TPosition pos ) const override;
};

#endif // CPRINT_H