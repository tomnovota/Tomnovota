#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
  public:
    //---------------------------------------------------------------------------------------------
    CDate ( int y, int m, int d )
    : m_Year ( y )
    , m_Month ( m )
    , m_Day ( d ) {}
    //---------------------------------------------------------------------------------------------
    int compare ( const CDate& x ) const
    {
        if ( m_Year != x . m_Year )
            return m_Year - x . m_Year;
        if ( m_Month != x . m_Month )
            return m_Month - x . m_Month;
        return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int year ( void ) const 
    {
        return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int month ( void ) const 
    {
        return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int day ( void ) const 
    {
        return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream& operator << ( ostream& os, const CDate& x )
    {
        char oldFill = os . fill();
        return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-" 
                  << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-" 
                  << setw ( 2 ) << static_cast<int> ( x . m_Day ) 
                  << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
  private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};
#endif /* __PROGTEST__ */

// MapIterator = _Rb_tree_iterator < pair < const TName, set < shared_ptr<CInvoice> > > >;
// MapIteratorConst = _Rb_tree_const_iterator < pair < const TName, set < shared_ptr<CInvoice> > > >;
        
/* Struct representing company names used in register. */
struct TName
{
    TName ( void ) {}
    TName ( string standard, string official )
    : m_Standard ( standard ), m_Official ( official ) {}

    /* Compare function - comparing standard names */
    bool operator < ( const TName& x ) const { return m_Standard < x . m_Standard; }

    string m_Standard; // standardized by standardFormat function
    string m_Official;
};

/* Function creating standardized format of company name. */
string standardFormat ( string name )
{
    for_each ( name . begin(), name . end(), [] ( char& c ) { c = tolower ( static_cast < unsigned char > ( c ) ); } );
    istringstream iss ( name );

    string res;
    string tmp;
    iss >> res;
    while ( iss >> tmp )
        res += " " + tmp;

    return res;
}

/* Class representing invoice. */
class CInvoice
{
  public:
    /* Static function comparing invoices by member type date: */
    static bool LessDate ( const CInvoice& x, const CInvoice& y, bool asc )
    {
        return asc
            ? x . date() . compare ( y . date() ) < 0
            : y . date() . compare ( x . date() ) < 0;
    }

    /* Static function comparing invoices by member type seller: */
    static bool LessSeller ( const CInvoice& x, const CInvoice& y, bool asc )
    {
        return asc
            ? strcasecmp ( x . seller() . c_str(), y . seller() . c_str() ) < 0
            : strcasecmp ( y . seller() . c_str(), x . seller() . c_str() ) < 0;
    }

    /* Static function comparing invoices by member type buyer: */
    static bool LessBuyer ( const CInvoice& x, const CInvoice& y, bool asc )
    {
        return asc
            ? strcasecmp ( x . buyer() . c_str(), y . buyer() . c_str() ) < 0
            : strcasecmp ( y . buyer() . c_str(), x . buyer() . c_str() ) < 0;
    }

    /* Static function comparing invoices by member type amount: */
    static bool LessAmount ( const CInvoice& x, const CInvoice& y, bool asc )
    {
        return asc
            ? x . amount() < y . amount()
            : y . amount() < x . amount();
    }

    /* Static function comparing invoices by member type VAT: */
    static bool LessVat ( const CInvoice& x, const CInvoice& y, bool asc )
    {
        return asc
            ? x . vat() < y . vat()
            : y . vat() < x . vat();
    }

    // Constructor
    CInvoice ( const CDate& date,
               const string& seller,
               const string& buyer,
               unsigned int amount,
               double vat )

    : m_Date ( date )
    , m_Seller ( seller )
    , m_Buyer ( buyer )
    , m_Amount ( amount )
    , m_Vat ( vat )
    , m_Registred_By ( make_pair ( false, false ) ) {}

    // getters and setters
    CDate date ( void ) const { return m_Date; }
    string buyer ( void ) const { return m_Buyer; }
    string seller ( void ) const { return m_Seller; }
    unsigned int amount ( void ) const { return m_Amount; }
    double vat ( void ) const { return m_Vat; }

    bool registred ( void ) const { return m_Registred_By . first && m_Registred_By . second; }
    bool getRegistredBySeller ( void ) const { return m_Registred_By . first; }
    bool getRegistredByBuyer ( void ) const { return m_Registred_By . second; }
    void setRegistredBySeller ( bool val ) { m_Registred_By . first = val; }
    void setRegistredByBuyer ( bool val ) { m_Registred_By . second = val; }

    void setSeller ( const string& name ) { m_Seller = name; }
    void setBuyer ( const string& name ) { m_Buyer = name; }

    unsigned long int getIndex ( void ) const { return m_Index; }
    void setIndex ( unsigned long int x ) { m_Index = x; }

    /* Less function for class CInvoice - used in register. */
    bool operator < ( const CInvoice& y ) const
    {
        return m_Seller < y . m_Seller
            ? true
            : y . m_Seller < m_Seller ? false : m_Buyer < y . m_Buyer
                ? true
                : y . m_Buyer < m_Buyer ? false : m_Date . compare ( y . m_Date ) < 0
                    ? true
                    : y . m_Date . compare ( m_Date ) < 0 ? false : m_Amount < y . m_Amount
                        ? true
                        : y . m_Amount < m_Amount ? false : m_Vat < y . m_Vat
                            ? true
                            : false;
        return false;
    }

    /* Compare function for function equal. */
    bool operator == ( const CInvoice& y ) const { return ! (*this < y) && ! (y < *this); }

    /* Output operator. */
    friend ostream& operator << ( ostream& os, const CInvoice& x )
    {
        return os << "    Date: " << x . date()
            << " | Seller: " << x . seller()
            << " | Buyer: " << x . buyer()
            << " | Amount: " << x . amount()
            << " | VAT: " << x . vat()
            << " | INDEX: " << x . getIndex() << endl;
    }

  private:
    CDate            m_Date;
    string           m_Seller;
    string           m_Buyer;
    unsigned int     m_Amount;
    double           m_Vat;

    pair<bool, bool> m_Registred_By; // seller, buyer
    unsigned long int m_Index = 0; // rank in register
};

/* Class for sorting criteria. */
class CSortOpt
{
  public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    static const int CRITERIA_COUNT = 5;

    CSortOpt ( void ) {}

    /* Functor working as Compare function for sort function. */
    bool operator () ( const CInvoice& x, const CInvoice& y ) const
    {
        for ( int i = 0; i < m_Size; i ++ )
        {
            bool asc = m_Order [i] . second;
            if ( ! m_Order [i] . first ( x, y, asc )
                && ! m_Order [i] . first ( y, x, asc )
                ) continue;

            return m_Order [i] . first ( x, y, asc );
        }
        return x . getIndex() < y . getIndex();
    }

    /* Function accepting sorting criteria and saving them. */
    CSortOpt& addKey ( int key, bool ascending = true )
    {
        if ( m_Size >= CRITERIA_COUNT )
            return *this;
    
        switch ( key ) // adds right compare function into array of criteria
        {
            case BY_DATE: m_Order [ m_Size ++ ] = make_pair ( &CInvoice::LessDate, ascending ); break;
            case BY_BUYER: m_Order [ m_Size ++ ] = make_pair ( &CInvoice::LessBuyer, ascending ); break;
            case BY_SELLER: m_Order [ m_Size ++ ] = make_pair ( &CInvoice::LessSeller, ascending ); break;
            case BY_AMOUNT: m_Order [ m_Size ++ ] = make_pair ( &CInvoice::LessAmount, ascending ); break;
            case BY_VAT: m_Order [ m_Size ++ ] = make_pair ( &CInvoice::LessVat, ascending ); break;
            default: break;
        }
        return *this;
    }

  private:
    int m_Size = 0; // number of criteria
    pair < bool (*) ( const CInvoice &, const CInvoice &, bool ), bool > m_Order [CRITERIA_COUNT]; // array with compare functions ordered and created by given criteria
};

/* Functor working as compare function for set of invoices. */
class FLessSet
{
  public:
    // prodávající, kupující, datum, částka, DPH
    bool operator () ( const shared_ptr<CInvoice>& x, const shared_ptr<CInvoice>& y ) const { return *x < *y; }
};

/* Class representing register of accepted and issued invoices. */
class CVATRegister
{
  public:
    CVATRegister ( void ) {}

    /* Appends company into register. */
    bool registerCompany ( const string& name )
    {
        const auto [it, success] = m_Registr . insert ( make_pair ( TName ( standardFormat ( name ), name ), set < shared_ptr<CInvoice>, FLessSet > () ) );
        return success;
    }

    /* Adds invoice into register by seller. */
    bool addIssued ( const CInvoice& x )
    {
        map<TName, set<shared_ptr<CInvoice>, FLessSet>>::iterator itSeller, itBuyer;
        if ( ! findTraders ( x, itSeller, itBuyer ) ) // company not found
            return false;

        // new invoice instance created
        auto tmp = make_shared<CInvoice> ( x . date(), itSeller -> first . m_Official, itBuyer -> first . m_Official, x . amount(), x . vat() );

        auto itSellerSet = itSeller -> second . lower_bound ( tmp ); // finds invoice position - for seller
        if ( itSellerSet != itSeller -> second . end()
            && *tmp == **itSellerSet )
        {
            if ( ( *itSellerSet ) -> getRegistredBySeller() ) // invoice already exists
                return false;

            ( *itSellerSet ) -> setRegistredBySeller ( true );
            return true;
        }
        auto itBuyerSet = itBuyer -> second . lower_bound ( tmp ); // finds invoice position - for buyer

        // insert invoice into set
        tmp -> setIndex ( m_Index ++ );
        tmp -> setRegistredBySeller ( true );
        itSeller -> second . insert ( itSellerSet, tmp );
        itBuyer -> second . insert ( itBuyerSet, tmp );
        return true;
    }

    /* Adds invoice into register by buyer. */
    bool addAccepted ( const CInvoice& x )
    {
        map<TName, set<shared_ptr<CInvoice>, FLessSet>>::iterator itSeller, itBuyer;
        if ( ! findTraders ( x, itSeller, itBuyer ) ) // company not found
            return false;

        // new invoice instance created
        auto tmp = make_shared<CInvoice> ( x . date(), itSeller -> first . m_Official, itBuyer -> first . m_Official, x . amount(), x . vat() );
        auto itBuyerSet = itBuyer -> second . lower_bound ( tmp ); // finds invoice position - for buyer
        if ( itBuyerSet != itBuyer -> second . end()
            && *tmp == **itBuyerSet )
        {
            if ( ( *itBuyerSet ) -> getRegistredByBuyer() ) // invoice already exists
                return false;
            
            ( *itBuyerSet ) -> setRegistredByBuyer ( true );
            return true;
        }
        auto itSellerSet = itSeller -> second . lower_bound ( tmp ); // finds invoice position - for seller

        // insert invoice into set
        tmp -> setIndex ( m_Index ++ );
        tmp -> setRegistredByBuyer ( true );
        itSeller -> second . insert ( itSellerSet, tmp );
        itBuyer -> second . insert ( itBuyerSet, tmp );
        return true;
    }

    /* Deletes invoice from register by seller. */
    bool delIssued ( const CInvoice& x )
    {
        map<TName, set<shared_ptr<CInvoice>, FLessSet>>::iterator itSeller, itBuyer;
        if ( ! findTraders ( x, itSeller, itBuyer ) ) // company not found
            return false;

        // new invoice instance created for comparison
        auto tmp = make_shared<CInvoice> ( x . date(), itSeller -> first . m_Official, itBuyer -> first . m_Official, x . amount(), x . vat() );

        auto itSellerSet = itSeller -> second . lower_bound ( tmp );
        if ( itSellerSet == itSeller -> second . end() // company not found
            || ! ( *tmp == **itSellerSet )
            || ! (*itSellerSet) -> getRegistredBySeller() )
            return false;

        auto itBuyerSet = itBuyer -> second . lower_bound ( tmp );
        // extracts invoice from set
        auto invoice = *itSellerSet;

        invoice -> setRegistredBySeller ( false ); // deleting record of seller
        if ( ! invoice -> getRegistredByBuyer() ) // no company registred this invoice
        {
            itSeller -> second . erase ( itSellerSet );
            itBuyer -> second . erase ( itBuyerSet );
        }
        return true;
    }

    /* Deletes invoice from register by buyer. */
    bool delAccepted ( const CInvoice& x )
    {
        map<TName, set<shared_ptr<CInvoice>, FLessSet>>::iterator itSeller, itBuyer;
        if ( ! findTraders ( x, itSeller, itBuyer ) ) // company not found
            return false;

        // new invoice instance created for comparison
        auto tmp = make_shared<CInvoice> ( x . date(), itSeller -> first . m_Official, itBuyer -> first . m_Official, x . amount(), x . vat() );

        auto itBuyerSet = itBuyer -> second . lower_bound ( tmp );
        if ( itBuyerSet == itBuyer -> second . end() // záznam faktury neexistuje
            || ! ( *tmp == **itBuyerSet )
            || ! (*itBuyerSet) -> getRegistredByBuyer() )
            return false;

        auto itSellerSet = itSeller -> second . lower_bound ( tmp );
        // extracts invoice from set
        auto invoice = *itSellerSet;

        invoice -> setRegistredByBuyer ( false ); // deleting record of buyer
        if ( ! invoice -> getRegistredBySeller() ) // no company registred this invoice
        {
            itSeller -> second . erase ( itSellerSet );
            itBuyer -> second . erase ( itBuyerSet );
        }
        return true;
    }

    /* Returns list of invoices not registered by both subjects. */
    list<CInvoice> unmatched ( const string& company, const CSortOpt& sortBy ) const
    {
        list<CInvoice> res;
        map<TName, set<shared_ptr<CInvoice>, FLessSet>>::const_iterator it;
        if ( ! findTraders ( company, it ) ) // company not found
            return res;

        // iterate through invoices of particular company
        for ( const auto& el : it -> second ) 
        {
            if ( ! el -> registred() ) // not registered by both subjects
                res . push_back ( * el );
        }
        res . sort ( sortBy ); // sorting
        return res;
    }

    void print ( void )
    {
        for ( auto& c : m_Registr )
        {
            cout << c . first . m_Official << " - " << c . first . m_Standard << endl;
            cout << " ==>" << endl;

            for ( auto d : c . second )
                cout << *d << endl;
        }
        cout << "===========================================" << endl;
    }

  private:
    /* Function searching for companies from given invoice in register. */
    bool findTraders ( const CInvoice& invoice, map<TName, set<shared_ptr<CInvoice>, FLessSet>>::iterator& x1, map<TName, set<shared_ptr<CInvoice>, FLessSet>>::iterator& x2 )
    {
        // getting standard company name
        string standard_name_seller = standardFormat ( invoice . seller() );
        string standard_name_buyer = standardFormat ( invoice . buyer() );

        if ( standard_name_seller == standard_name_buyer ) // same name
            return false;
        
        x1 = m_Registr . lower_bound ( TName ( standard_name_seller, "" ) );
        x2 = m_Registr . lower_bound ( TName ( standard_name_buyer, "" ) );

        if ( x1 == m_Registr . end() || x2 == m_Registr . end() // not found
            || x1 -> first . m_Standard != standard_name_seller
            || x2 -> first . m_Standard != standard_name_buyer )
            return false;

        return true;
    }

    /* Function searching for given company in register. */
    bool findTraders ( const string& name, map<TName, set<shared_ptr<CInvoice>, FLessSet>>::const_iterator& x ) const
    {
        // getting standard company name
        string standard_name = standardFormat ( name );

        x = m_Registr . lower_bound ( TName ( standard_name, "" ) );

        if ( x == m_Registr . end()
            || x -> first . m_Standard != standard_name )
            return false; // not found

        return true;
    }

    // Variables
    map < TName, set < shared_ptr<CInvoice>, FLessSet > > m_Registr; // original / standard
    unsigned long int m_Index = 0; // counter of invoices
};

#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice>& a, const list<CInvoice>& b )
{
    // cout << "LIST: " << a . size() << endl;
    // for ( const auto& d : a )
    //     cout << d << endl;

    return equal ( a . begin(), a . end(), b . begin() )
        && equal ( b . begin(), b . end(), a . begin() );
}

int main ( void )
{
  CVATRegister r;
  assert ( r . registerCompany ( "first Company" ) );
  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );

  assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
