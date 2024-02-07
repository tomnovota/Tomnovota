#ifndef CCELL_H
#define CCELL_H

#include <string>
#include <set>
#include <memory>

#include "Helpers.h"
#include "CExpression.h"

/**
 * @class CCell
 * @brief Class representing cell in a table.
*/
class CCell
{
  public:
    /**
     * @brief Assigns cell given content.
     * @param[in] props New content of cell.
    */
    void Edit ( TCellProperties props );

    /**
     * @brief Deletes content of cell.
    */
    void Delete ( void );

    /**
     * @brief Calls evaluation of cell's expression.
    */
    void Evaluate ( void );

    /**
     * @brief Value getter.
    */
    CValue Value ( void ) const;

    /**
     * @brief Formula getter.
    */
    std::string GetFormula ( void ) const;

    /**
     * @brief Get string representation of calculated value.
    */
    std::string GetValue ( void ) const;

    /**
     * @brief Indicates wheter cell is empty.
    */
    bool Empty ( void ) const;

    /**
     * @brief Indicates wheter cell contains error value.
    */
    bool Error ( void ) const;

  private:
    friend class CTable;

    /** Final value of cell. */
    CValue m_Value; 

    /** User text assigned expression. */
    std::string m_Formula; 

    /** Root of tree representing assigned expression. */ 
    std::unique_ptr< CExpression > m_Expression; 
    
    /** Cells that current cell is dependant on. */ 
    std::set< TPosition > m_Dependancies; 
};

#endif // CCELL_H