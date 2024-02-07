#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

const size_t STD_SIZE = 100;

class CString
{
  public:
    // Constructor, ...
    CString ( const char* src = "" )
    : m_Len ( strlen ( src ) )
    , m_Arr ( new char [ m_Len + 1 ] ) { memcpy ( m_Arr, src, m_Len + 1 ); }

    CString ( const CString& src )
    : m_Len ( src . m_Len )
    , m_Arr ( new char [ m_Len + 1 ] ) { memcpy ( m_Arr, src . m_Arr, m_Len + 1 ); }

    CString ( CString && src ) noexcept
    : m_Len ( src . m_Len )
    , m_Arr ( src . m_Arr ) { src . m_Arr = nullptr; }

    ~CString ( void ) noexcept { delete [] m_Arr; }

    CString& operator = ( const CString& src )
    {
        if ( this == &src )
            return *this;

        if ( m_Len < src . m_Len )
        {
            delete [] m_Arr;
            m_Len = src . m_Len;
            m_Arr = new char [ m_Len + 1 ];
        }
        memcpy ( m_Arr, src . m_Arr, m_Len + 1 );
        return *this;
    }

    CString& operator = ( CString&& src ) noexcept
    {
        if ( this == &src )
            return *this;

        delete [] m_Arr;
        m_Arr = src . m_Arr;
        m_Len = src . m_Len;

        src . m_Arr = nullptr;
        return *this;
    }

    // Methods
    bool operator == ( const CString& str ) const { return m_Len == str . m_Len && strcmp ( m_Arr, str . m_Arr ) == 0; }
    friend ostream& operator << ( ostream& os, const CString& str ) { return os << str . m_Arr; }

    // Variables

  private:
    // Methods

    // Variables
    size_t m_Len = 0;
    char* m_Arr = nullptr;
};

class CMail
{
  public:
    CMail ( void ) {}

    CMail ( const char* from, const char* to, const char* body )
    : m_Sender ( from )
    , m_Reciever ( to )
    , m_Body ( body ) {}

    CMail ( const CMail& mail )
    : m_Sender ( mail . m_Sender )
    , m_Reciever ( mail . m_Reciever )
    , m_Body ( mail . m_Body ) {}

    bool operator == ( const CMail& x ) const
    {
        return x . m_Sender == m_Sender
            && x . m_Reciever == m_Reciever
            && x . m_Body == m_Body;
    }

    const CString& getSender () const { return m_Sender; }
    const CString& getReciever () const { return m_Reciever; }

    bool matchSender ( const CString& email ) const { return m_Sender == email; }
    bool matchReciever ( const CString& email ) const { return m_Reciever == email; }

    friend ostream& operator << ( ostream& os, const CMail& m )
    {
        return os << "From: " << m . m_Sender
                  << ", To: " << m . m_Reciever
                  << ", Body: " << m . m_Body;
    }

  private:
    CString m_Sender;
    CString m_Reciever;
    CString m_Body;
};

class CVectorMail
{
  public:
    // Constructor, ...
    CVectorMail ( void )
    : m_Capacity ( STD_SIZE )
    , m_Len ( 0 )
    , m_Arr ( new CMail [ m_Capacity ] ) {}

    CVectorMail ( CMail mail, CString owner )
    : m_Capacity ( STD_SIZE )
    , m_Len ( 1 )
    , m_Arr ( new CMail [ m_Capacity ] )
    , m_Owner ( owner ) { m_Arr [0] = mail; }

    CVectorMail ( const CVectorMail& src )
    : m_Capacity ( src . m_Capacity )
    , m_Len ( src . m_Len )
    , m_Arr ( new CMail [ m_Capacity ] )
    , m_Owner ( src . m_Owner )
    {
        for ( size_t i = 0; i < src . m_Len; i ++ )
            m_Arr [i] = src . m_Arr [i];
    }

    ~CVectorMail ( void ) noexcept { delete [] m_Arr; }

    CVectorMail& operator = ( const CVectorMail& src )
    {
        if ( this == &src )
            return *this;

        m_Len = src . m_Len;
        m_Owner = src . m_Owner;
        if ( m_Capacity <= src . m_Capacity )
        {
            delete [] m_Arr;
            m_Capacity = src . m_Capacity;
            m_Arr = new CMail [ m_Capacity ];
        }
        for ( size_t i = 0; i < m_Len; i ++ )
            m_Arr [i] = src . m_Arr [i];

        return *this;
    }

    // Methods
    size_t size ( void ) const { return m_Len; }

    const CString& getOwner ( void ) const { return m_Owner; }

    const CMail& operator [] ( size_t pos ) const { return m_Arr [ pos ]; }

    void add ( CMail mail )
    {
        if ( m_Len >= m_Capacity )
        {
            m_Capacity *= 2;
            CMail* tmp = new CMail [ m_Capacity ];
            for ( size_t i = 0; i < m_Len; i ++ )
                tmp [i] = move ( m_Arr [i] );

            delete [] m_Arr;
            m_Arr = tmp;
        }
        m_Arr [ m_Len ++ ] = move ( mail );
    }

    // Variables

  private:
    // Methods

    // Variables
    size_t m_Capacity = 0;
    size_t m_Len = 0;
    CMail* m_Arr = nullptr;
    CString m_Owner;
};

class CMailIterator 
{
  public:
    CMailIterator ( void )
    : m_Box ( nullptr ) {}

    CMailIterator ( CVectorMail* box )
    : m_Box ( box )
    , m_Size ( box -> size() ) {}

    explicit operator bool ( void ) const { return m_Current < m_Size; }

    bool operator ! ( void ) const { return m_Current >= m_Size; }

    const CMail& operator * ( void ) const { return ( *m_Box ) [ m_Current ]; }

    CMailIterator& operator ++ ( void )
    {
        if ( m_Current < m_Size )
            m_Current ++;

        return *this;
    }

  private:
    CVectorMail* m_Box;
    size_t m_Current = 0;
    size_t m_Size = 0;
};

class CVectorBox
{
  public:
    CVectorBox ( void )
    : m_Capacity ( STD_SIZE )
    , m_Len ( 0 )
    , m_Arr ( new CVectorMail [ m_Capacity ] ) {}

    CVectorBox ( const CVectorBox& src )
    : m_Capacity ( src . m_Capacity )
    , m_Len ( src . m_Len )
    , m_Arr ( new CVectorMail [ m_Capacity ] )
    {
        for ( size_t i = 0; i < m_Len; i ++ )
            m_Arr [i] = src . m_Arr [i];
    }

    CVectorBox& operator = ( const CVectorBox& src )
    {
        if ( this == &src )
            return *this;

        m_Len = src . m_Len;
        if ( m_Capacity <= src . m_Capacity )
        {
            delete [] m_Arr;
            m_Capacity = src . m_Capacity;
            m_Arr = new CVectorMail [ m_Capacity ];
        }
        for ( size_t i = 0; i < m_Len; i ++ )
            m_Arr [i] = src . m_Arr [i];
        return *this;
    }

    ~CVectorBox ( void ) { delete [] m_Arr; }

    // Methods
    bool findBox ( const CString& email, CVectorMail*& box ) const
    {
        for ( size_t i = 0; i < m_Len; i ++ )
        {
            if ( m_Arr [i] . getOwner() == email )
            {
                box = &m_Arr [i];
                return true;
            }
        }
        return false;
    }

    void add ( CMail mail, CString owner )
    {
        if ( m_Len == m_Capacity )
        {
            m_Capacity *= 2;
            CVectorMail* tmp = new CVectorMail [ m_Capacity ];
            for ( size_t i = 0; i < m_Len; i ++ )
                tmp [i] = move ( m_Arr [i] );

            delete [] m_Arr;
            m_Arr = tmp;
        }
        m_Arr [ m_Len ++ ] = CVectorMail ( mail, owner );
    }

  private:
    // Variables
    size_t m_Capacity = 0;
    size_t m_Len = 0;
    CVectorMail* m_Arr = nullptr;
};

class CMailServer 
{
  public:
    CMailServer ( void ) {}

    CMailServer ( const CMailServer& src )
    : m_Inbox ( src . m_Inbox )
    , m_Outbox ( src . m_Outbox ) {}

    CMailServer& operator = ( const CMailServer& src )
    {
        m_Inbox = src . m_Inbox;
        m_Outbox = src . m_Outbox;
        return *this;        
    }

    ~CMailServer ( void ) {}

    void sendMail ( const CMail& m )
    {
        CVectorMail* box;
        if ( ! m_Outbox . findBox ( m . getSender(), box ) )
            m_Outbox . add ( m, m . getSender() );
        else
            box -> add ( m );

        if ( ! m_Inbox . findBox ( m . getReciever(), box ) )
            m_Inbox . add ( m, m . getReciever() );
        else
            box -> add ( m );
    }

    CMailIterator outbox ( const char* email ) const
    {
        CVectorMail* box;
        if ( ! m_Outbox . findBox ( email, box ) )
            return CMailIterator ();

        return CMailIterator ( box );
    }

    CMailIterator inbox ( const char* email ) const
    {
        CVectorMail* box;
        if ( ! m_Inbox . findBox ( email, box ) )
            return CMailIterator ();

        return CMailIterator ( box );
    }

  private:
    // Methods

    // Variables
    CVectorBox m_Inbox;
    CVectorBox m_Outbox;
};

#ifndef __PROGTEST__
bool matchOutput ( const CMail& m, const char* str )
{
    ostringstream oss;
    oss << m;
    return oss . str() == str;
}

int main ( void )
{

    // PROGTEST
  char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  CMailServer s0;
  s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
  CMailIterator i0 = s0 . inbox ( "alice" );
  assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i0 );

  CMailIterator i1 = s0 . inbox ( "john" );
  assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
  assert ( ! ++i1 );

  CMailIterator i2 = s0 . outbox ( "john" );
  assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ! ++i2 );

  CMailIterator i3 = s0 . outbox ( "thomas" );
  assert ( ! i3 );

  CMailIterator i4 = s0 . outbox ( "steve" );
  assert ( ! i4 );

  CMailIterator i5 = s0 . outbox ( "thomas" );
  s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! i5 );

  CMailIterator i6 = s0 . outbox ( "thomas" );
  assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
  assert ( ! ++i6 );

  CMailIterator i7 = s0 . inbox ( "alice" );
  s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i7 );

  CMailIterator i8 = s0 . inbox ( "alice" );
  assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ! ++i8 );

  CMailServer s1 ( s0 );
  s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
  s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  CMailIterator i9 = s0 . inbox ( "alice" );
  assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ! ++i9 );

  CMailIterator i10 = s1 . inbox ( "alice" );
  assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
  assert ( ! ++i10 );

  CMailServer s2;
  s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  CMailIterator i11 = s2 . inbox ( "alice" );
  assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
  assert ( ! ++i11 );

  s2 = s0;
  s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
  s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  CMailIterator i12 = s0 . inbox ( "alice" );
  assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
  assert ( ! ++i12 );

  CMailIterator i13 = s2 . inbox ( "alice" );
  assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
  assert ( ! ++i13 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
