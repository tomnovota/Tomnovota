#ifndef CCONTROL_H
#define CCONTROL_H

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Helpers.h"
#include "CTable.h"
#include "CParser.h"
#include "CPrint.h"
#include "CLoadTable.h"

/**
 * @class CControl
 * @brief Class providing user interaction with table.
*/
class CControl
{
  public:
    CControl ( size_t init_size, std::string horizontal_line, std::string vertical_line, std::istream & in_stream, std::ostream & out_stream );

    /**
     * @brief Begin table processing.
    */
    void Start ( void );

  private:

    /**
     * @brief Processing forming table user commands from terminal.
     * @return whether user input ended properly.
    */
    bool FormTable ( void );

    /**
     * @brief Dialog about table initialization.
     * @return whether got proper answer.
    */
    bool EnterDialog ( void );

    /**
     * @brief Dialog about table saving.
     * @return whether got proper answer.
    */
    bool ExitDialog ( void );

    /**
     * @brief Dialog about file name.
     * @param[in] message Message showed to user.
     * @return Answer from user.
    */
    std::string Dialog ( const char * message );

    // Variables
    CTable m_Table;
    CLoadTable m_Loader;
    CParser m_Parser;
    CPrintValue m_Print;
    CPrintFormula m_PrintF;

    /** Input source. */
    std::istream & m_In_Stream; 

    /** Output destination. */
    std::ostream & m_Out_Stream; 

};

#endif // CCONTROL_H
