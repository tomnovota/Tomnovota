#include "../src/CCell.h"
#include "../src/CControl.h"
#include "../src/CExpression.h"
#include "../src/CLoadTable.h"
#include "../src/COperand.h"
#include "../src/COperator.h"
#include "../src/CParser.h"
#include "../src/CPrint.h"
#include "../src/CTable.h"
#include "../src/Helpers.h"

#include <cassert>

int main ( void )
{
    CTable t1 ( 10 );
    CParser parser ( t1 );
    CPrintFormula printF ( t1, "-", "|" );
    TCellProperties p1, p2, p3;
    std::stringstream ss1, ss2, ss3;

    // table initialization
    assert ( t1 . Size() == 10 );

    ss1 . str ( "= 50 + E5" );
    p1 = std::move ( parser . ParseExpression ( ss1 ) );
    t1 . Edit ( TPosition ( 11, 5 ), std::move ( p1 ) );

    // table expansion
    assert ( t1 . Size() == 12 );

    ss2 . str ( "= 5" );
    p2 = std::move ( parser . ParseExpression ( ss2 ) );
    bool detected = false;
    try { t1 . Edit ( TPosition ( 4, 4 ), std::move ( p2 ) ); }
    catch ( const std::exception & ex ) { detected = true; }
    // cycle detection
    assert ( ! detected );

    t1 . Evaluate();
    assert ( ! t1 . At ( TPosition ( 11, 5 ) ) -> Value() . Empty() );
    assert ( ! t1 . At ( TPosition ( 11, 5 ) ) -> Value() . Error() );
    assert ( t1 . At ( TPosition ( 11, 5 ) ) -> GetFormula() == "= 50 + E5" );
    assert ( t1 . At ( TPosition ( 11, 5 ) ) -> GetValue() == "55" );
    assert ( ! t1 . At ( TPosition ( 4, 4 ) ) -> Value() . Empty() );
    assert ( ! t1 . At ( TPosition ( 4, 4 ) ) -> Value() . Error() );

    ss3 . str ( "= F12" );
    p3 = std::move ( parser . ParseExpression ( ss3 ) );
    detected = false;
    try { t1 . Edit ( TPosition ( 4, 4 ), std::move ( p3 ) ); }
    catch ( const std::exception & ex ) { detected = true; }
    // cycle detection
    assert ( detected );

    t1 . Evaluate();
    assert ( t1 . At ( TPosition ( 11, 5 ) ) -> Value() . Empty() );
    assert ( ! t1 . At ( TPosition ( 11, 5 ) ) -> Value() . Error() );
    assert ( t1 . At ( TPosition ( 11, 5 ) ) -> GetFormula() == "= 50 + E5" );
    assert ( t1 . At ( TPosition ( 11, 5 ) ) -> GetValue() == "" );
    assert ( t1 . At ( TPosition ( 4, 4 ) ) -> Value() . Empty() );
    assert ( ! t1 . At ( TPosition ( 4, 4 ) ) -> Value() . Error() );

    std::cout << "ALL TESTS DONE SUCCESSFULLY!" << std::endl;

    return 0;
}