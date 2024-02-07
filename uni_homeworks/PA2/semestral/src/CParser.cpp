#include "CParser.h"

CParser::CParser ( CTable & table )
 : m_Table ( table ) {}

        /* ============ CELL NAME ============ */

TPosition CParser::ParseCellName ( const std::string & name ) const
{
    auto c = name . begin();
    for ( ; IsLetter ( * c ); c ++ ); // skip letter characters
    TPosition res = TPosition ( ParseRowName ( std::string ( c, name . end() ) ), ParseColName ( std::string ( name . begin(), c ) ) );
    return res;
}

size_t CParser::ParseRowName ( std::string row ) const
{
    size_t res;
    if ( ! row . empty() && ( row [0] == '-' || row [0] == ROW_MIN ) )
        throw std::invalid_argument ( "Incorrect cell name" );
    try
    {
        res = std::stoull ( row ) - 1; // converts to row number
    }
    catch ( const std::exception & ex )
    {
        throw std::invalid_argument ( "Incorrect cell name" );
    }
    return res;
}

size_t CParser::ParseColName ( std::string col ) const
{
    size_t res = 0, i = 0;
    auto r = col . rbegin();
    for ( ; r != col . rend(); r ++, i ++ )
    {
        if ( ! IsLetter ( * r ) )
            throw std::invalid_argument ( "Incorrect cell name" );
        res += ( * r - COL_MIN + bool ( i ) ) * pow ( COL_RANGE, i ); // sums value of letter characters
    }
    if ( col . empty() )
        throw std::invalid_argument ( "Incorrect cell name" );
    return res;
}

        /* ============ RANGE ============ */

TRange CParser::ParseRange ( std::stringstream & ss ) const
{
    TRange result;
    std::string tmp;

    if ( ! ( ss >> tmp ) ) // no parameter
        return TRange ( TPosition ( 0, 0 ), TPosition ( m_Table . Size() - 1, m_Table . Size() - 1 ) );

    result . m_From = ParseCellName ( tmp );
    if ( ! ( ss >> tmp ) ) // 1 cell
    {
        result . m_To = result . m_From;
        return result;
    }
    if ( tmp != ":" || ! ( ss >> tmp ) ) // bad format
        throw std::invalid_argument ( "Incorrect table range" );

    result . m_To = ParseCellName ( tmp );

    // converts to format where from pos start in left upper corner and to pos ends in right lower corner
    if ( result . m_From . m_Row > result . m_To . m_Row )
        std::swap ( result . m_From . m_Row, result . m_To . m_Row );
    if ( result . m_From . m_Col > result . m_To . m_Col )
        std::swap ( result . m_From . m_Col, result . m_To . m_Col );

    return result;
}

        /* ============ EXPRESSION ============ */

TCellProperties CParser::ParseExpression ( std::stringstream & ss ) const
{
    TCellProperties result;
    result . m_Formula = ss . str(); // text representing expression

    if ( ! ReadBeginning ( ss ) )
        throw std::invalid_argument ( "Missing '=' before expression" );

    result . m_Expression = ShuntingYard ( ss, result . m_Dependencies );
    return result;
}

bool CParser::ReadBeginning ( std::stringstream & ss ) const
{
    std::string tmp;
    if ( ! ( ss >> tmp ) || tmp != "=" )
        return false;
    return true;
}

std::unique_ptr< CExpression > CParser::ShuntingYard ( std::stringstream & ss, std::set<TPosition> & deps ) const
{
    std::deque< std::unique_ptr< COperator > > operators;
    std::deque< std::unique_ptr< CExpression > > output;
    std::string token;

    while ( ss >> token ) // token on input
    {
        if ( ! ReadOperand ( token, output, deps, ss ) ) // token might be operator
        {
            if ( token == LEFT_PAR )
            {
                operators . push_back ( std::make_unique< CPar > () );
            }
            else if ( token == RIGHT_PAR )
            {
                ProcessPar ( operators, output );
            }
            else
                ProcessOperator ( token, operators, output );
        }
    }
    while ( ! operators . empty() )
    {
        if ( operators . back() -> Precedence() == 0 )
            throw std::invalid_argument ( "Incorrect parentheses" );

        ApplyOperator ( operators, output );
    }
    if ( output . size() != 1 )
        throw std::invalid_argument ( "Wrong expression syntax" );

    return std::move ( output . back() );
}

void CParser::ApplyOperator ( std::deque< std::unique_ptr< COperator > > & operators, std::deque< std::unique_ptr< CExpression > > & output ) const
{
    // extract operator
    std::unique_ptr< COperator > op = std::move ( operators . back() );
    operators . pop_back();

    op -> SetOperands ( output );
    output . push_back ( std::move ( op ) ); // insert result to output 
}

bool CParser::ProcessPar ( std::deque< std::unique_ptr< COperator > > & operators, std::deque< std::unique_ptr< CExpression > > & output ) const
{
    while ( ! operators . empty() && operators . back() -> Precedence() > 0 )
    {
        ApplyOperator ( operators, output ); // not left parenthesis operators
    }
    if ( operators . empty() )
        throw std::invalid_argument ( "Incorrect parentheses" );

    operators . pop_back(); // left parenthesis
    return true;
}

bool CParser::ProcessOperator ( std::string token, std::deque< std::unique_ptr< COperator > > & operators, std::deque< std::unique_ptr< CExpression > > & output ) const
{
    std::unique_ptr< COperator > op = CreateOperator ( token );
    while ( ! operators . empty() && op -> Precedence() <= operators . back() -> Precedence() )
    {
        ApplyOperator ( operators, output );
    }
    operators . push_back ( std::move ( op ) );
    return true;
}

std::unique_ptr< COperator > CParser::CreateOperator ( std::string token ) const
{
    if ( token == OPERATOR_PLUS )
        return std::make_unique< COperatorPlus > ();

    if ( token == OPERATOR_MINUS )
        return std::make_unique< COperatorMinus > ();

    if ( token == OPERATOR_MULTI )
        return std::make_unique< COperatorMulti > ();

    if ( token == OPERATOR_DIV )
        return std::make_unique< COperatorDiv > ();

    if ( token == OPERATOR_CONCAT )
        return std::make_unique< COperatorConcat > ();

    if ( token == OPERATOR_REPEAT )
        return std::make_unique< COperatorRepeat > ();

    if ( token == FUNCTION_ABS )
        return std::make_unique< CFunctionAbs > ();

    if ( token == FUNCTION_LOWER )
        return std::make_unique< CFunctionLower > ();

    if ( token == FUNCTION_UPPER )
        return std::make_unique< CFunctionUpper > ();

    throw std::invalid_argument ( "Unknown operator" );
}

bool CParser::ReadOperand ( std::string token, std::deque< std::unique_ptr< CExpression > > & output, std::set<TPosition> & deps, std::stringstream & ss ) const
{
    // TEXT
    if ( * token . begin() == TEXT_SYMBOL )
    {
        std::string text = token;
        char letter;
        ss >> std::noskipws;
        while ( text . size() <= 1 || * ( text . end() - 1 ) != TEXT_SYMBOL ) // reads whole text
        {
            if ( ! ( ss >> letter ) ) // read all text inside text
                throw std::invalid_argument ( "Incorrect text format" );

            text += letter;
        }
        ss >> std::skipws; // turn on skipping white spaces
        output . push_back ( std::make_unique< COperandValue > ( std::string ( text . begin() + 1, text . end() - 1 ) ) );
        return true;
    }
    // NUMBER
    if ( IsNumber ( token ) )
    {
        output . push_back ( std::make_unique< COperandValue > ( std::stoi ( token ) ) );
        return true;
    }
    // CELL REFERENCE
    if ( IsLetter ( * token . begin() ) )
    {
        TPosition pos;
        try
        {
            pos = ParseCellName ( token );
        }
        catch ( const std::exception & ex )
        {
            return false;
        }
        deps . insert ( pos );
        output . push_back ( std::make_unique< COperandCell > ( m_Table . At ( pos ) ) );
        return true;
    }
    return false;
}

        /* ============ HELPERS ============ */

bool CParser::IsNumber ( const std::string & token )
{
    auto start = token . begin() != token . end()
        && ( * token . begin() == '-' && token . size() != 1 ) // if number is negative and is not only minus
        ? token . begin() + 1
        : token . begin();

    for ( auto & c = start; c != token . end(); c ++ )
        if ( ! IsNumber ( * c ) )
            return false;

    return true;
}

bool CParser::IsLetter ( char c )
{
    return c >= COL_MIN && c <= COL_MAX;
}

bool CParser::IsNumber ( char c )
{
    return c >= ROW_MIN && c <= ROW_MAX;
}