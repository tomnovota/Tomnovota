#ifndef CPARSER_H
#define CPARSER_H

#include <set>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>
#include <cmath>
#include <deque>

#include <iostream>

#include "CTable.h"
#include "Helpers.h"
#include "COperand.h"
#include "COperator.h"

/**
 * @class CParser
 * @brief Class parsing given string constructs and returning result in suitable form.
*/
class CParser
{
  public:
    CParser ( CTable & table );

    /**
     * @brief Returns if given char is letter.
     * @return Whether given character is letter.
    */
    static bool IsLetter ( char c );

    /**
     * @brief Returns if given char is number.
     * @return Whether given character is number.
    */
    static bool IsNumber ( char c );

    /**
     * @brief Returns if given string is number.
     * @return Whether given string is number.
    */
    static bool IsNumber ( const std::string & token );

    /**
     * @brief Returns position of given cell name.
     * @param[in] name String representation of cell name to be parsed.
     * @return Position coordinates of given cell name.
    */
    TPosition ParseCellName ( const std::string & name ) const;

    /**
     * @brief Proccess range contained in given source and creates appropriate representation of table range.
     * @param[in] ss Source of string representation of range.
     * @return Returns range of cells given in stream.
    */
    TRange ParseRange ( std::stringstream & ss ) const;

    /**
     * @brief Returns cell properties contained in text expression in given stream.
     * @param[in] ss Source of string representation of expression.
     * @return Cell properties parsed from given source.
    */
    TCellProperties ParseExpression ( std::stringstream & ss ) const;

  private:
    /**
     * @brief Parsing row name from given string.
     * @param[in] row String with text representation of row.
     * @return Row coordinates.
    */
    size_t ParseRowName ( std::string row ) const;

    /**
     * @brief Parsing col name from given string.
     * @param[in] col String with text representation of column.
     * @return Column coordinates.
    */
    size_t ParseColName ( std::string col ) const;

    /**
     * @brief Algorithm processing given text source of expression and creating expression tree belonging to particular cell and extracting other properties.
     * @param[in] ss Source of text representation of expression.
     * @param[out] deps Cell references included in proccessed expression.
     * @return Root node of tree representing expression belonging to cell.
    */
    std::unique_ptr< CExpression > ShuntingYard ( std::stringstream & ss, std::set<TPosition> & deps ) const;

    /**
     * @brief Finds out whether expression begins with beginning character.
     * @param[in] ss Source of text to be parsed.
     * @return Whether is beginning character present.
    */
    bool ReadBeginning ( std::stringstream & ss ) const;

    /**
     * @brief Tries to read operand from source and saves it into operand container and records cell dependancies.
     * @param[in] token Current token to be proccessed.
     * @param[out] output Operand container.
     * @param[out] deps Cell references included in proccessed expression.
     * @param[in] ss Source of text to be parsed.
     * @return Whether read operand successfully.
    */
    bool ReadOperand ( std::string token, std::deque< std::unique_ptr< CExpression > > & output, std::set<TPosition> & deps, std::stringstream & ss ) const;

    /**
     * @brief Assigns operands to operator on top of stack and saves result to operand container.
     * @param[in] operators Operator stack.
     * @param[out] output Operand container.
    */
    void ApplyOperator ( std::deque< std::unique_ptr< COperator > > & operators, std::deque< std::unique_ptr< CExpression > > & output ) const;

    /**
     * @brief Methods called after finding right parenthesis and proccessing nested expression.
     * @param[in] operators Operator stack.
     * @param[out] output Operand container.
     * @return Whether proccessed parenthesis successfully.
    */
    bool ProcessPar ( std::deque< std::unique_ptr< COperator > > & operators, std::deque< std::unique_ptr< CExpression > > & output ) const;

    /**
     * @brief ShuntingYard algorithm part proccessing common operator token.
     * @param[in] token Current token to be proccessed.
     * @param[in] operators Operator stack.
     * @param[out] output Operand container.
     * @return Whether proccessed successfully.
    */
    bool ProcessOperator ( std::string token, std::deque< std::unique_ptr< COperator > > & operators, std::deque< std::unique_ptr< CExpression > > & output ) const;

    /**
     * @brief Recognises token and creates appropriate operator node.
     * @param[in] token Token to be recognised.
     * @return Appropriate operator node.
    */
    std::unique_ptr< COperator > CreateOperator ( std::string token ) const;

    CTable & m_Table;
};

#endif // CPARSER_H
