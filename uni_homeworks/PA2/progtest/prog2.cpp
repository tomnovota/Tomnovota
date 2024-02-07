#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */


/**
 * Class representing an employee.
 */
class CPerson
{
  public:
    // Constructor, Destructor, ...
    CPerson () {}
    CPerson ( const string& name, const string& surname, const string& email, unsigned int salary )
    : m_Name (name), m_Surname (surname), m_Email (email), m_Salary (salary)
    {}
    ~CPerson () {}
    
    /**
     * Compares 2 CPerson objects based on name.
     * @param left First CPerson object.
     * @param right Second CPerson object.
     * @return true if left object should be before right object in ascending order.
     */
    static bool cmpName ( const CPerson* left, const CPerson* right )
    {
        int cmp = left -> m_Surname . compare ( right -> m_Surname );
        if ( cmp != 0 )
            return cmp < 0;
        return left -> m_Name . compare ( right -> m_Name ) < 0;        
    }

    /**
     * Compares CPerson object and pair of name and surname based on name.
     * @param left CPerson object.
     * @param name First part of full name to be compared.
     * @param surname Second part of full name to be compared.
     * @return true if left object should be before right operand in ascending order.
     */
    static bool cmpNameParams ( const CPerson* left, const string& name, const string& surname )
    {
        int cmp = left -> m_Surname . compare ( surname );
        if ( cmp != 0 )
            return cmp < 0;
        return left -> m_Name . compare ( name ) < 0;        
    }

    /**
     * Compares 2 CPerson objects based on email.
     * @param left First CPerson object.
     * @param right Second CPerson object.
     * @return true if left object should be before right object in ascending order.
     */
    static bool cmpEmail ( const CPerson* left, const CPerson* right )
    {
        return left -> m_Email . compare ( right -> m_Email ) < 0;        
    }

    /**
     * Compares CPerson object and email.
     * @param left CPerson object.
     * @param email Email to be compared.
     * @return true if left object should be before right operand in ascending order.
     */
    static bool cmpEmailParams ( const CPerson* left, const string& email )
    {
        return left -> m_Email . compare ( email ) < 0;
    }

    /**
     * Setter for name and surname.
     */
    void changeName ( const string& name, const string& surname )
    {
        m_Name = name;
        m_Surname = surname;
    }

    /**
     * Setter for email.
     */
    void changeEmail ( const string& email )
    {
        m_Email = email;
    }

    /**
     * Setter for salary.
     */
    void changeSalary ( unsigned int salary )
    {
        m_Salary = salary;
    }

    /**
     * Getter for name.
     */
    const string& getName ()
    {
        return m_Name;
    }

    /**
     * Getter for surname.
     */
    const string& getSurname ()
    {
        return m_Surname;
    }

    /**
     * Getter for email.
     */
    const string& getEmail ()
    {
        return m_Email;
    }

    /**
     * Getter for salary.
     */
    const unsigned& getSalary ()
    {
        return m_Salary;
    }

  private:
    // Variables
    string m_Name;
    string m_Surname;
    string m_Email;
    unsigned int m_Salary;
};

/** Custom name for vector iterator of CPerson objects. **/
using CPersonIterator = vector<CPerson*>::iterator;
using CPersonConstIterator = vector<CPerson*>::const_iterator; // const version used in lower_bound / begin() / end()


/**
 * Class implementing database of employees.
 */
class CPersonalAgenda
{
  public:
// Constructor, Destructor, ...

    // initialize empty database
    CPersonalAgenda  ( void ) {}

    // destructor
    ~CPersonalAgenda ( void )
    {
        for ( auto x : m_SortedName )
            delete x;
    }

// Methods

    /**
     * Adds a record of employee into database.
     * @param name First name of employee.
     * @param surname Second name of employee.
     * @param email Email of employee.
     * @param salary Salary of employee.
     * @return true if successful addition.
     */
    bool add ( const string& name, const string& surname, const string& email, unsigned int salary )
    {
        CPersonConstIterator idName, idEmail;
        if ( searchRecordName ( idName, name, surname ) || searchRecordEmail ( idEmail, email ) )
            return false;
        auto* newPerson = new CPerson ( name, surname, email, salary );
        m_SortedName . insert ( idName, newPerson );
        m_SortedEmail . insert ( idEmail, newPerson );
        return true;
    }

    /**
     * Removes the record of employee from database base on name.
     * @param name First name of employee.
     * @param surname Second name of employee.
     * @return true if successful removal.
     */
    bool del ( const string& name, const string& surname )
    {
        CPersonConstIterator id;
        if ( ! searchRecordName ( id, name, surname ) )
            return false;
        erase ( *id );
        return true;
    }

    /**
     * Removes the record of employee from database base on email.
     * @param email Email of employee.
     * @return true if successful removal.
     */
    bool del ( const string& email )
    {
        CPersonConstIterator id;
        if ( ! searchRecordEmail ( id, email ) )
            return false;
        erase ( *id );
        return true;
    }

    /**
     * Changes name of employee base on email.
     * @param email Email of employee.
     * @param newName New first name of employee.
     * @param newSurname New second name of employee.
     * @return true if successful change of name.
     */
    bool changeName ( const string& email, const string& newName, const string& newSurname )
    {
        CPersonConstIterator idEmail, idName, idName_new;
        if ( ! searchRecordEmail ( idEmail, email ) || searchRecordName ( idName_new, newName, newSurname ) )
            return false;

        CPerson* tmp = *idEmail;
        searchRecordName ( idName, tmp -> getName(), tmp -> getSurname() );
        m_SortedName . erase ( idName );
        
        if ( CPerson::cmpNameParams ( tmp, newName, newSurname ) )
            idName_new --;
        tmp -> changeName ( newName, newSurname );
        m_SortedName . insert ( idName_new, tmp );
        return true;
    }

    /**
     * Changes first name and second name of employee based on email.
     * @param name First name of employee.
     * @param surname Second name of employee.
     * @param newEmail New email of employee.
     * @return true if successful change of email.
     */
    bool changeEmail ( const string& name, const string& surname, const string& newEmail )
    {
        CPersonConstIterator idEmail, idEmail_new, idName;
        if ( ! searchRecordName ( idName, name, surname ) || searchRecordEmail ( idEmail_new, newEmail ) )
            return false;

        CPerson* tmp = *idName;
        searchRecordEmail ( idEmail, tmp -> getEmail() );
        m_SortedEmail . erase ( idEmail );
        
        if ( CPerson::cmpEmailParams ( tmp, newEmail ) )
            idEmail_new --;
        tmp -> changeEmail ( newEmail );
        m_SortedEmail . insert ( idEmail_new, tmp );
        return true;
    }

    /**
     * Changes salary of employee based on name.
     * @param name First name of employee.
     * @param surname Second name of employee.
     * @param salary New salary of employee.
     * @return true if successful change of salary.
     */
    bool setSalary ( const string& name, const string& surname, unsigned int salary )
    {
        CPersonConstIterator id;
        if ( ! searchRecordName ( id, name, surname ) )
            return false;
        (*id) -> changeSalary ( salary );
        return true;
    }

    /**
     * Changes salary of employee based on email.
     * @param email Email of employee.
     * @param salary New salary of employee.
     * @return true if successful change of salary.
     */
    bool setSalary ( const string& email, unsigned int salary )
    {
        CPersonConstIterator id;
        if ( ! searchRecordEmail ( id, email ) )
            return false;
        (*id) -> changeSalary ( salary );
        return true;
    }

    /**
     * Query for salary of employee based on name.
     * @param name First name of employee.
     * @param surname Second name of employee.
     * @return selary of the employee.
     */
    unsigned int getSalary ( const string& name, const string& surname ) const
    {
        CPersonConstIterator id;
        if ( ! searchRecordName ( id, name, surname ) )
            return 0;
        
        return (*id) -> getSalary();
    }

    /**
     * Query for salary of employee based on email.
     * @param email Email of employee.
     * @return selary of the employee.
     */
    unsigned int getSalary ( const string& email ) const
    {
        CPersonConstIterator id;
        if ( ! searchRecordEmail ( id, email ) )
            return 0;

        return (*id) -> getSalary();
    }

    /**
     * Query for rank range of the employee's salary.
     * @param name First name of employee.
     * @param surname Second name of employee.
     * @param rankMin Output param for lower boundary of the rank range.
     * @param rankMax Output param for upper boundary of the rank range.
     * @return true if successfuly found the employee.
     */
    bool getRank ( const string& name, const string& surname, int& rankMin, int& rankMax ) const
    {
        CPersonConstIterator id;
        size_t less_cnt = 0, equal_cnt = 0;
        if ( ! searchRecordName ( id, name, surname ) )
            return false;
        unsigned int salary = (*id) -> getSalary();
        for ( auto c : m_SortedName )
        {
            if ( salary < c -> getSalary() )
                continue;
            if ( salary > c -> getSalary() )
                less_cnt ++;
            equal_cnt ++;
        }
        rankMin = less_cnt;
        rankMax = equal_cnt - 1;
        return true;
    }

    /**
     * Query for rank range of the employee's salary.
     * @param email Email of employee.
     * @param rankMin Output param for lower boundary of the rank range.
     * @param rankMax Output param for upper boundary of the rank range.
     * @return true if successfuly found the employee.
     */
    bool getRank ( const string& email, int& rankMin, int& rankMax ) const
    {
        CPersonConstIterator id;
        size_t less_cnt = 0, equal_cnt = 0;
        if ( ! searchRecordEmail ( id, email ) )
            return false;
        unsigned int salary = (*id) -> getSalary();
        for ( auto c : m_SortedEmail )
        {
            if ( salary < c -> getSalary() )
                continue;
            if ( salary > c -> getSalary() )
                less_cnt ++;
            equal_cnt ++;
        }
        rankMin = less_cnt;
        rankMax = equal_cnt - 1;
        return true;
    }

    /**
     * Query for first employee in alphabetically sorted list of employees.
     * @param outName Output param for first name of employee.
     * @param outSurname Output param for second name of employee.
     * @return true if database not empty.
     */
    bool getFirst ( string& outName, string& outSurname ) const
    {
        if ( m_SortedName . empty() )
            return false;
        CPersonConstIterator id = m_SortedName . begin();
        outName = (*id) -> getName();
        outSurname = (*id) -> getSurname();
        return true;
    }

    /**
     * Query for next employee in alphabetically sorted list of employees.
     * @param name Input param for first name of employee.
     * @param surname Input param for second name of employee.
     * @param outName Output param for first name of employee.
     * @param outSurname Output param for second name of employee.
     * @return true if database not empty or input employee is not the last in the sorted list.
     */
    bool getNext ( const string& name, const string& surname, string& outName, string& outSurname ) const
    {
        CPersonConstIterator id;
        if ( ! searchRecordName ( id, name, surname )
            || (++id) == m_SortedName . end()
            ) return false;
        outName = (*id) -> getName();
        outSurname = (*id) -> getSurname();
        return true;
    }

  private:
    /** Array of employees alphabetically sorted based on name **/
    vector <CPerson *> m_SortedName;
    /** Array of employees alphabetically sorted based on email **/
    vector <CPerson *> m_SortedEmail;
    
// Methods
    /**
     * Function for searching in array sorted based on name.
     * @param id Output parameter for iterator pointing to first position where given employees name could be inserted without breaking sorting.
     * @param name Input param for first name of employee.
     * @param surname Input param for second name of employee.
     * @return true if no matching employee in database or no matching employee in database.
     */
    bool searchRecordName ( CPersonConstIterator& id, const string& name, const string& surname ) const
    {
        CPerson tmp = CPerson ( name, surname, "", 0 );
        id = std::lower_bound ( m_SortedName . begin(), m_SortedName . end(), &tmp, CPerson::cmpName );
        if ( id == m_SortedName . end()
            || CPerson::cmpName ( &tmp, *id )
            || CPerson::cmpName ( *id, &tmp )
            ) return false;
        return true;
    }

    /**
     * Function for searching in array sorted based on email.
     * @param id Output parameter for iterator pointing to first position where given employees name could be inserted without breaking sorting.
     * @param email Input param for email of employee.
     * @return true if no matching employee in database or no matching employee in database.
     */
    bool searchRecordEmail ( CPersonConstIterator& id, const string& email ) const
    {
        CPerson tmp = CPerson ( "", "", email, 0 );
        id = std::lower_bound ( m_SortedEmail . begin(), m_SortedEmail . end(), &tmp, CPerson::cmpEmail );
        if ( id == m_SortedEmail . end() ||
            CPerson::cmpEmail ( &tmp, *id ) ||
            CPerson::cmpEmail ( *id, &tmp )
            ) return false;
        return true;
    }

    /**
     * Function for deleting employee from database.
     * @param id Parameter with pointer to employee to be deleted.
     */
    void erase ( CPerson* id )
    {
        CPersonConstIterator idName, idEmail;
        searchRecordName ( idName, id -> getName(), id -> getSurname() );
        searchRecordEmail ( idEmail, id -> getEmail() );
        m_SortedName . erase ( idName );
        m_SortedEmail . erase ( idEmail );
        delete id;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . setSalary ( "john", 32000 ) );
  assert ( b1 . getSalary ( "john" ) ==  32000 );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . getSalary ( "john" ) ==  35000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  assert ( b1 . getSalary ( "peter" ) ==  23000 );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "james" ) ==  23000 );
  assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . getFirst ( outName, outSurname ) );
  assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . setSalary ( "joe", 90000 ) );
  assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . getSalary ( "joe" ) ==  0 );
  assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . getRank ( "joe", lo, hi ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . del ( "Joe", "Black" ) );
  assert ( ! b2 . del ( "joe" ) );
  assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . setSalary ( "peter", 37000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  0 );
  assert ( ! b2 . getRank ( "peter", lo, hi ) );
  assert ( ! b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . del ( "peter" ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
