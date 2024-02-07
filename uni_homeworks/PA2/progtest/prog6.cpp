#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect 
{
  public:
    CRect ( double x, double y, double w, double h )
    : m_X ( x )
    , m_Y ( y )
    , m_W ( w )
    , m_H ( h ) {}

    friend ostream& operator << ( ostream& os, const CRect& x )
    {
        return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};

#endif /* __PROGTEST__ */

/* Abstract class representing control element. */
class CControlElem
{
  public:
    CControlElem ( int id, const CRect& relPos )
    : m_ID ( id ), m_RelPos ( relPos ), m_AbsPos ( relPos ) {}

    virtual ~CControlElem ( void ) {}

    virtual CControlElem* copy ( void ) const = 0;
    virtual ostream& print ( ostream& os ) const = 0;
    virtual ostream& printWindow ( ostream& os, bool combo ) const = 0;
    virtual bool combo ( void ) const = 0;
    void resize ( CRect window_pos )
    {
        window_pos . m_X = window_pos . m_X + m_RelPos . m_X * window_pos . m_W;
        window_pos . m_Y = window_pos . m_Y + m_RelPos . m_Y * window_pos . m_H;
        window_pos . m_W *= m_RelPos . m_W;
        window_pos . m_H *= m_RelPos . m_H;
        m_AbsPos = window_pos;
    }

    size_t getID ( void ) const { return m_ID; }

    friend ostream& operator << ( ostream& os, const CControlElem& el );

  protected:
    size_t m_ID;
    CRect m_RelPos;
    CRect m_AbsPos;
};

class CWindow
{
  public:
    static bool lessID ( CControlElem* x, CControlElem* y ) { return x -> getID() < y -> getID(); }
    static bool lessIDVal ( CControlElem* y, size_t id ) { return y -> getID() < id; }

    /* Constructor. */
    CWindow ( int id, const string& title, const CRect& absPos )
    : m_ID ( id ), m_Title ( title ), m_AbsPos ( absPos ) {}

    CWindow ( const CWindow& src )
    : m_ID ( src . m_ID ), m_Title ( src . m_Title ), m_AbsPos ( src . m_AbsPos )
    {
        for ( const auto& el : src . m_Elems )
            m_Elems . push_back ( el -> copy() );
    }

    CWindow& operator = ( const CWindow& src )
    {
        m_ID = src . m_ID;
        m_Title = src . m_Title;
        m_AbsPos = src . m_AbsPos;
        for ( auto& el : m_Elems )
            delete el;

        m_Elems . clear();
        for ( const auto& el : src . m_Elems )
            m_Elems . push_back ( el -> copy() );
        return *this;
    }

    ~CWindow ( void )
    {
        for ( auto& el : m_Elems )
            delete el;
    }

    // add
    CWindow& add ( const CControlElem& el )
    {
        CControlElem* tmp = el . copy();
        tmp -> resize ( m_AbsPos );
        m_Elems . insert ( lower_bound ( m_Elems . begin(), m_Elems . end(), tmp, lessID ), tmp );

        return *this;
    }

    // search
    CControlElem* search ( size_t id )
    {
        return *lower_bound ( m_Elems . begin(), m_Elems . end(), id, lessIDVal );
    }

    // setPosition
    void setPosition ( const CRect& absPos )
    {
        m_AbsPos = absPos;
        for ( auto& el : m_Elems ) { el -> resize ( absPos ); }
    }

    ostream& print ( ostream& os ) const;

    // output operator
    friend ostream& operator << ( ostream& os, const CWindow& el );

  private:
    size_t m_ID;
    string m_Title;
    CRect m_AbsPos;
    vector< CControlElem* > m_Elems;
};

class CButton : public CControlElem
{
  public:
    /* Constructor. */
    CButton ( int id, const CRect& relPos, const string& name )
    : CControlElem ( id, relPos ), m_Name ( name ) {}

    virtual CControlElem* copy ( void ) const override { return new CButton ( *this ); }

    virtual ostream& print ( ostream& os ) const override;

    virtual ostream& printWindow ( ostream& os, bool combo ) const override;

    virtual bool combo ( void ) const override { return false; }

  protected:
    string m_Name;
};

class CInput : public CControlElem
{
  public:
    /* Constructor. */
    CInput ( int id, const CRect& relPos, const string& value )
    : CControlElem ( id, relPos ), m_Value ( value ) {}

    virtual CControlElem* copy ( void ) const override { return new CInput ( *this ); }

    virtual ostream& print ( ostream& os ) const override;

    virtual ostream& printWindow ( ostream& os, bool combo ) const override;

    virtual bool combo ( void ) const override { return false; }

    // setValue 
    void setValue ( const string& val ) { m_Value = val; }

    // getValue 
    string getValue ( void ) const { return m_Value; }

  protected:
    string m_Value;
};

class CLabel : public CControlElem
{
  public:
    /* Constructor. */
    CLabel ( int id, const CRect& relPos, const string& label )
    : CControlElem ( id, relPos ), m_Label ( label ) {}

    virtual CControlElem* copy ( void ) const override { return new CLabel ( *this ); }

    virtual ostream& print ( ostream& os ) const override;

    virtual ostream& printWindow ( ostream& os, bool combo ) const override;

    virtual bool combo ( void ) const override { return false; }

  protected:
    string m_Label;
};

class CComboBox : public CControlElem
{
  public:
    /* Constructor. */
    CComboBox ( int id, const CRect& relPos )
    : CControlElem ( id, relPos ) {}

    virtual CControlElem* copy ( void ) const override { return new CComboBox ( *this ); }

    virtual ostream& print ( ostream& os ) const override;

    virtual ostream& printWindow ( ostream& os, bool combo ) const override;

    virtual bool combo ( void ) const override { return true; }

    // add
    CComboBox& add ( const string& val )
    {
        m_Menu . push_back ( val );
        return *this;
    }

    // setSelected
    void setSelected ( size_t index ) { m_Choice = index; }

    // getSelected
    size_t getSelected ( void ) const { return m_Choice; }

  protected:
    vector<string> m_Menu;
    size_t m_Choice = 0;
};

// output operators

ostream& CWindow::print ( ostream& os ) const
{
    return os << "[" << m_ID << "] Window \"" << m_Title << "\" " << m_AbsPos << "\n";
}

ostream& CButton::print ( ostream& os ) const
{
    return os << "[" << m_ID << "] Button \"" << m_Name << "\" " << m_AbsPos << "\n";
}

ostream& CButton::printWindow ( ostream& os, bool combo ) const
{
    return print ( os );
}

ostream& CInput::print ( ostream& os ) const
{
    return os << "[" << m_ID << "] Input \"" << m_Value << "\" " << m_AbsPos << "\n";
}

ostream& CInput::printWindow ( ostream& os, bool combo ) const
{
    return print ( os );
}

ostream& CLabel::print ( ostream& os ) const
{
    return os << "[" << m_ID << "] Label \"" << m_Label << "\" " << m_AbsPos << "\n";
}

ostream& CLabel::printWindow ( ostream& os, bool combo ) const
{
    return print ( os );
}

ostream& CComboBox::print ( ostream& os ) const
{
    os << "[" << m_ID << "] ComboBox " << m_AbsPos << "\n";

    for ( size_t i = 0; i < m_Menu . size(); i ++ )
    {
        if ( i == m_Choice )
            os << "+->" << m_Menu [i] << "<" << "\n";
        else
            os << "+- " << m_Menu [i] << "\n";
    }
    return os;
}

ostream& CComboBox::printWindow ( ostream& os, bool combo ) const
{
    os << "[" << m_ID << "] ComboBox " << m_AbsPos << "\n";

    string tmp = combo ? "|" : " ";

    for ( size_t i = 0; i < m_Menu . size(); i ++ )
    {
        if ( i == m_Choice )
            os << tmp << "  +->" << m_Menu [i] << "<" << "\n";
        else
            os << tmp << "  +- " << m_Menu [i] << "\n";
    }
    return os;
}

ostream& operator << ( ostream& os, const CWindow& win )
{
    win . print ( os );
    for ( const auto& el : win . m_Elems )
    {
        os << "+- ";
        el -> printWindow ( os, el -> combo() && el != *(win . m_Elems . end() - 1) );
    }
    return os;
}

ostream& operator << ( ostream& os, const CControlElem& el )
{
    return el . print ( os );
}

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T& x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int main ( void )
{

// |------------ PROGTEST -------------|

  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
