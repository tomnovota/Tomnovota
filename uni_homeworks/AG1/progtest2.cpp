#ifndef __PROGTEST__
#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>

#endif

// @formatter:off

char Val ( char c )
{
    return c == '\n' ? '\\' : c;
}

struct TextEditorBackend
{
    /************
    * Node.
    ***********************************************/

    struct Node
    {
        Node ( void ) = default;

        Node ( char val, size_t count, Node * parent )
                : m_Val ( val )
                , m_Count ( count )
                , m_Lines ( val == '\n' )
                , m_Parent ( parent )
        {}

        Node ( char val, Node * parent )
        : m_Val ( val )
        , m_Lines ( val == '\n' )
        , m_Parent ( parent )
        {}

        char Sgn ( void ) const
        {
            return ( m_Right
                    ? m_Right -> m_Height
                    : 0 ) -
                   ( m_Left
                    ? m_Left -> m_Height
                    : 0 );
        }

        bool FromLeft ( void )
        {
            return m_Parent && m_Parent -> m_Left == this;
        }

        size_t LeftLines ( void ) const
        {
            return m_Left ? m_Left -> m_Lines : 0;
        }

        size_t LeftCount ( void ) const
        {
            return m_Left ? m_Left -> m_Count : 0;
        }

        char NewLine ( void ) const
        {
            return m_Val == '\n';
        }

        size_t GetHeight ( Node * v ) { return v ? v -> m_Height : 0; }

        size_t GetLines ( Node * v ) { return v ? v -> m_Lines : 0; }

        size_t GetCount ( Node * v ) { return v ? v -> m_Count : 0; }

        void UpdateHeight ( void )
        {
            size_t new_height = 1 + std::max ( GetHeight ( m_Left ), GetHeight ( m_Right ) );

            if ( new_height != m_Height )
            {
                m_Height = new_height;
                if ( m_Parent ) m_Parent -> UpdateHeight();
            }
        }

        void Update ( void )
        {
            m_Height = 1 + std::max ( GetHeight ( m_Left ), GetHeight ( m_Right ) );
            m_Lines = NewLine() + GetLines ( m_Left ) + GetLines ( m_Right );
            m_Count = 1 + GetCount( m_Left ) + GetCount ( m_Right );
            if ( m_Parent ) m_Parent -> Update();
        }

        char m_Val;
        size_t m_Count = 1;
        size_t m_Lines = 0;
        size_t m_Height = 1;

        Node * m_Parent = nullptr;
        Node * m_Left = nullptr;
        Node * m_Right = nullptr;
    };

    /************
    * Variables.
    ***********************************************/
    Node * m_Root = nullptr;
    size_t m_Size = 0;

    /************
    * Tree.
    ***********************************************/

    ~TextEditorBackend ( void )
    {
        Destroy ( m_Root );
    }

    void Destroy( Node * v )
    {
        if ( ! v ) return;
        Destroy ( v -> m_Left );
        Destroy ( v -> m_Right );
        delete v;
    }

    void print ( void ) const
    {
        std::cout << "ROOT = " << ( m_Root ? m_Root -> m_Val : '_' );
        std::cout << ", SIZE = " << size();
        std::cout << std::endl;
        show ( m_Root );
        std::cout << std::endl;
    }

    void show ( Node * v ) const
    {
        if ( ! v )
            return;

        show ( v -> m_Left );

        for ( size_t i = 0; i < v -> m_Height; ++ i )
            std::cout << ".";

        std::cout << (v -> m_Val == '\n' ? '\\' : v -> m_Val);
        std::cout << " (sgn=" << int ( v -> Sgn() ) << ", hg=" << v -> m_Height << ", cnt=" << v -> m_Count << ", ln=" << v -> m_Lines << ")" << std::endl;
        std::cout << std::endl;

        show ( v -> m_Right );
    }

    /************
    * Interface.
    ***********************************************/
    TextEditorBackend(const std::string &text);

    size_t size() const;

    size_t lines() const;

    char at(size_t i) const;

    void edit(size_t i, char c);

    void insert(size_t i, char c);

    void erase(size_t i);

    size_t line_start(size_t r) const;

    size_t line_length(size_t r) const;

    size_t char_to_line(size_t i) const;

    /************
    * Methods.
    ***********************************************/

    size_t GetHeight ( Node * v ) { return v ? v -> m_Height : 0; }

    Node * Build ( const std::string & text, Node * parent ) {
        size_t id = text . size() / 2;
        auto node = new Node ( text [ id ], text . size(), parent );

        if ( id > 0 ) // L(v) exists
        {
            node -> m_Left = Build ( text . substr ( 0, id ), node );
            node -> m_Lines += node -> m_Left -> m_Lines;
        }
        if ( ( ++ id ) < text . size() ) // R(v) exists
        {
            node -> m_Right = Build ( text . substr (id, text . size() - id ), node );
            node -> m_Lines += node -> m_Right -> m_Lines;
        }
        node -> UpdateHeight();
        return node;
    }

    /* Returns node with index id */
    Node * BVSFindID ( size_t id ) const
    {
        Node * v = m_Root;
        while ( v )
        {
            if ( v -> LeftCount() == id ) // node found
                return v;

            if ( id < v -> LeftCount() ) // node in L(v)
                v = v -> m_Left;

            else // node in R(v)
            {
                id -= v -> LeftCount() + 1;
                v = v -> m_Right;
            }
        }
        throw std::out_of_range("BVSFindID");
        return v;
    }

    /* Returns number of lines before index */
    size_t BVSFindLineIndex ( size_t id ) const
    {
        Node * v = m_Root;
        size_t lines_passed = 0;
        while ( v )
        {
            if ( v -> LeftCount() == id ) // node found
                return lines_passed + v -> LeftLines();

            if ( id < v -> LeftCount() ) // node in L(v)
                v = v -> m_Left;

            else // node in R(v)
            {
                lines_passed += v -> LeftLines() + v -> NewLine();
                id -= v -> LeftCount() + 1;
                v = v -> m_Right;
            }
        }
        throw std::out_of_range("BVSFindLineIndex");
        return lines_passed;
    }

    /* Returns index of r-th line separator */
    size_t BVSFindLine ( size_t r ) const
    {
        Node * v = m_Root;
        size_t id = 0;
        while ( v )
        {
            if ( v -> NewLine() && r == v -> LeftLines() + 1 ) // line separator found
                return id + v -> LeftCount();

            if ( r <= v -> LeftLines() ) // line in L(v)
                v = v -> m_Left;

            else // line in R(v)
            {
                r -= v -> LeftLines() + v -> NewLine();
                id += v -> LeftCount() + 1;
                v = v -> m_Right;
            }
        }
        return id;
    }

    /* ----------------------------------------------------------------------------------- */

    /* Can return nullptr */
    Node * BVSSucc ( Node * v )
    {
        if ( v && v -> m_Right )
            return BVSMin ( v -> m_Right );

        while ( v && v != m_Root )
        {
            if ( LeftNode ( v ) )
                return v -> m_Parent;

            v = v -> m_Parent;
        }
        return nullptr; // last element
    }

    Node * BVSMin ( Node * v ) const
    {
        while ( v && v -> m_Left )
            v = v -> m_Left;

        return v;
    }

    Node * BVSMax ( Node * v ) const
    {
        while ( v && v -> m_Right )
            v = v -> m_Right;

        return v;
    }

    bool LeftNode ( Node * v )
    {
        return v && v -> FromLeft();
    }

    void Balance ( Node * v, bool insert )
    {
        if ( ! v ) // no node
            return;

        switch ( v -> Sgn() )
        {
            case 0:
            {
                if ( ! insert )
                    Balance ( v -> m_Parent, insert );
                return;
            }
            case 2:
            {
                if ( v -> m_Right -> Sgn() == -1 ) // schizo
                    RotateRight ( v -> m_Right );

                if ( ! insert )
                    return RotateLeft ( v, true );
                return RotateLeft ( v );
            }
            case -2:
            {
                if ( v -> m_Left -> Sgn() == 1 ) // schizo
                    RotateLeft ( v -> m_Left );

                if ( ! insert )
                    return RotateRight ( v, true );
                return RotateRight ( v );
            }
            default: // +1, -1
            {
                if ( insert )
                    Balance ( v -> m_Parent, insert );
                return;
            }
        };
    }

    void RotateLeft ( Node * x, bool edge = false )
    {
        Node * y = x -> m_Right;
//        std::cout << "RotateLeft ... " << x -> m_Val << ", " << y -> m_Val << std::endl;

        x -> m_Right = y -> m_Left; // move subtree
        y -> m_Left = x; // rotate x,y

        if ( x -> m_Parent ) // modify predecessor
        {
            if ( LeftNode ( x ) )
                x -> m_Parent -> m_Left = y;
            else
                x -> m_Parent -> m_Right = y;
        }
        else
            m_Root = y;

        // successor references
        if ( x -> m_Right )
            x -> m_Right -> m_Parent = x;
        y -> m_Parent = x -> m_Parent;
        x -> m_Parent = y;

        x -> Update();

        if ( y -> m_Parent && edge )
            Balance ( y -> m_Parent, false );
    }

    void RotateRight ( Node * x, bool edge = false )
    {
        Node * y = x -> m_Left;
//        std::cout << "RotateLeft ... " << x -> m_Val << ", " << y -> m_Val << std::endl;

        x -> m_Left = y -> m_Right; // move subtree
        y -> m_Right = x; // rotate x, y

        if ( x -> m_Parent ) // modify predecessor
        {
            if ( LeftNode ( x ) )
                x -> m_Parent -> m_Left = y;
            else
                x -> m_Parent -> m_Right = y;
        }
        else
            m_Root = y;

        // successor references
        if ( x -> m_Left )
            x -> m_Left -> m_Parent = x;
        y -> m_Parent = x -> m_Parent;
        x -> m_Parent = y;

        x -> Update();

        if ( y -> m_Parent && edge )
            Balance ( y -> m_Parent, false );
    }

    void BVSInsert ( size_t i, char c )
    {
        ++ m_Size;
        Node * v = m_Root;
        if ( ! v ) // empty tree
        {
            m_Root = new Node ( c, nullptr );
            return;
        }
        while ( v )
        {
            if ( i <= v -> LeftCount() ) // in L(v)
            {
                if ( v -> m_Left ) // deeper in L(v)
                {
                    v = v -> m_Left;
                    continue;
                }
                v -> m_Left = new Node ( c, v );
                v -> Update();
                Balance ( v, true );
                return;
            }
            else // in R(v)
            {
                if ( v -> m_Right ) // vlož do existující R(curr)
                {
                    i -= v -> LeftCount() + 1;
                    v = v -> m_Right;
                    continue;
                }
                v -> m_Right = new Node ( c, v );
                v -> Update();
                Balance ( v, true );
                return;
            }
        }
    }

    void BVSDelete ( Node * v )
    {
        if ( ! v ) return; // nothing to delete

        if ( ! v -> m_Left && ! v -> m_Right ) // leaf
        {
            // modify predecessor
            if ( v -> m_Parent )
            {
                if ( LeftNode ( v ) )
                    v -> m_Parent -> m_Left = nullptr;
                else
                    v -> m_Parent -> m_Right = nullptr;
            }
            else
                m_Root = nullptr;
        }
        else if ( v -> m_Left && ! v -> m_Right ) // has only L(v)
        {
            // modify predecessor
            if ( v -> m_Parent )
            {
                if ( LeftNode ( v ) )
                    v -> m_Parent -> m_Left = v -> m_Left;
                else
                    v -> m_Parent -> m_Right = v -> m_Left;
            }
            else
                m_Root = v -> m_Left;

            // modify successor
            v -> m_Left -> m_Parent = v -> m_Parent;
        }
        else if ( ! v -> m_Left && v -> m_Right ) // has only R(v)
        {
            // modify predecessor
            if ( v -> m_Parent )
            {
                if ( LeftNode ( v ) )
                    v -> m_Parent -> m_Left = v -> m_Right;
                else
                    v -> m_Parent -> m_Right = v -> m_Right;
            }
            else
                m_Root = v -> m_Right;

            // modify successor
            v -> m_Right -> m_Parent = v -> m_Parent;
        }
        else // both L(v) and R(v)
        {
            Node * succ = BVSSucc ( v );
            if ( succ -> m_Val == v -> m_Val )
                return BVSDelete ( succ );

            v -> m_Val = succ -> m_Val;
            v -> Update();
            return BVSDelete ( succ );
        }
        -- m_Size;
        v -> m_Parent -> Update();
        Balance ( v -> m_Parent, false );
        delete v;
    }
};

TextEditorBackend::TextEditorBackend ( const std::string & text )
{
    m_Size = text . size();
    m_Root = text . empty()
            ? nullptr
            : Build ( text, nullptr );
//    print();
}

size_t TextEditorBackend::size() const
{
    return m_Size;
}

size_t TextEditorBackend::lines() const
{
//    print();
    return m_Root
    ? m_Root -> m_Lines + 1
    : 1;
}

char TextEditorBackend::at ( size_t i ) const
{
    if ( i >= m_Size )
        throw std::out_of_range ( "" );

    auto node = BVSFindID ( i );
    return node -> m_Val;
}

void TextEditorBackend::edit ( size_t i, char c )
{
//    std::cout << "EDIT ... " << i << ", c=" << Val(c) << std::endl;
    if ( i >= m_Size )
        throw std::out_of_range ( "" );

    auto node = BVSFindID ( i );

    if ( c == node -> m_Val ) // no change
        return;

    node -> m_Val = c;
    node -> Update();
//    print();
}

void TextEditorBackend::insert ( size_t i, char c )
{
//    std::cout << "INSERT ... " << i << ", c=" << Val(c) << std::endl;
    if ( i > m_Size )
        throw std::out_of_range ( "" );

    BVSInsert ( i, c );
//    print();
}

void TextEditorBackend::erase ( size_t i )
{
//    std::cout << "ERASE ... " << i << ", c=" << std::endl;
    if ( i >= m_Size )
        throw std::out_of_range ( "" );

    auto node = BVSFindID ( i );
    BVSDelete ( node );
//    print();
}

size_t TextEditorBackend::line_start ( size_t r ) const
{
    if ( r >= lines() )
        throw std::out_of_range ( "" );

    return r == 0
    ? 0
    : ( BVSFindLine ( r ) + 1 );
}

size_t TextEditorBackend::line_length ( size_t r ) const
{
    if ( r >= lines() )
        throw std::out_of_range ( "" );

    return ( r + 1 >= lines()
    ? m_Size
    : line_start ( r + 1 ) ) - line_start ( r );
}

size_t TextEditorBackend::char_to_line ( size_t i ) const
{
    if ( i >= m_Size )
        throw std::out_of_range ( "" );

    return BVSFindLineIndex ( i );
}

/* ----------------------------------------------------------------------------------- */

#ifndef __PROGTEST__

////////////////// Dark magic, ignore ////////////////////////

template < typename T >
auto quote(const T& t) { return t; }

std::string quote(const std::string& s) {
  std::string ret = "\"";
  for (char c : s) if (c != '\n') ret += c; else ret += "\\n";
  return ret + "\"";
}

#define STR_(a) #a
#define STR(a) STR_(a)

#define CHECK_(a, b, a_str, b_str) do { \
    auto _a = (a); \
    decltype(a) _b = (b); \
    if (_a != _b) { \
      std::cout << "Line " << __LINE__ << ": Assertion " \
        << a_str << " == " << b_str << " failed!" \
        << " (lhs: " << quote(_a) << ")" << std::endl; \
      fail++; \
    } else ok++; \
  } while (0)

#define CHECK(a, b) CHECK_(a, b, #a, #b)

#define CHECK_ALL(expr, ...) do { \
    std::array _arr = { __VA_ARGS__ }; \
    for (size_t _i = 0; _i < _arr.size(); _i++) \
      CHECK_((expr)(_i), _arr[_i], STR(expr) "(" << _i << ")", _arr[_i]); \
  } while (0)

#define CHECK_EX(expr, ex) do { \
    try { \
      (expr); \
      fail++; \
      std::cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but no exception was raised." << std::endl; \
    } catch (const ex&) { ok++; \
    } catch (...) { \
      fail++; \
      std::cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but got different exception." << std::endl; \
    } \
  } while (0)
 
////////////////// End of dark magic ////////////////////////


std::string text(const TextEditorBackend& t) {
  std::string ret;
  for (size_t i = 0; i < t.size(); i++) ret.push_back(t.at(i));
  return ret;
}

void test1(int& ok, int& fail) {
	TextEditorBackend s("123\n456\n789");
  CHECK(s.size(), 11);
	CHECK(text(s), "123\n456\n789");
  CHECK(s.lines(), 3);
  CHECK_ALL(s.char_to_line, 0,0,0,0, 1,1,1,1, 2,2,2);
  CHECK_ALL(s.line_start, 0, 4, 8);
  CHECK_ALL(s.line_length, 4, 4, 3);
}

void test2(int& ok, int& fail) {
	TextEditorBackend t("123\n456\n789\n");
  CHECK(t.size(), 12);
	CHECK(text(t), "123\n456\n789\n");
  CHECK(t.lines(), 4);
  CHECK_ALL(t.char_to_line, 0,0,0,0, 1,1,1,1, 2,2,2,2);
  CHECK_ALL(t.line_start, 0, 4, 8, 12);
  CHECK_ALL(t.line_length, 4, 4, 4, 0);
}

void test3(int& ok, int& fail) {
	TextEditorBackend t("asdfasdfasdf");

  CHECK(t.size(), 12);
  CHECK(text(t), "asdfasdfasdf");
	CHECK(t.lines(), 1);
  CHECK_ALL(t.char_to_line, 0,0,0,0, 0,0,0,0, 0,0,0,0);
	CHECK(t.line_start(0), 0);
	CHECK(t.line_length(0), 12);
	
	t.insert(0, '\n');
  CHECK(t.size(), 13);
  CHECK(text(t), "\nasdfasdfasdf");
	CHECK(t.lines(), 2);
  CHECK_ALL(t.line_start, 0, 1);
	
  t.insert(4, '\n');
  CHECK(t.size(), 14);
  CHECK(text(t), "\nasd\nfasdfasdf");
	CHECK(t.lines(), 3);
  CHECK_ALL(t.line_start, 0, 1, 5);
	
  t.insert(t.size(), '\n');
  CHECK(t.size(), 15);
  CHECK(text(t), "\nasd\nfasdfasdf\n");
	CHECK(t.lines(), 4);
  CHECK_ALL(t.line_start, 0, 1, 5, 15);
	
	t.edit(t.size() - 1, 'H');
  CHECK(t.size(), 15);
  CHECK(text(t), "\nasd\nfasdfasdfH");
	CHECK(t.lines(), 3);
  CHECK_ALL(t.line_start, 0, 1, 5);

	t.erase(8);
  CHECK(t.size(), 14);
  CHECK(text(t), "\nasd\nfasfasdfH");
	CHECK(t.lines(), 3);
  CHECK_ALL(t.line_start, 0, 1, 5);

  t.erase(4);
  CHECK(t.size(), 13);
  CHECK(text(t), "\nasdfasfasdfH");
	CHECK(t.lines(), 2);
  CHECK_ALL(t.line_start, 0, 1);
}

void test_ex(int& ok, int& fail) {
	TextEditorBackend t("123\n456\n789\n");
  CHECK_EX(t.at(12), std::out_of_range);

  CHECK_EX(t.insert(13, 'a'), std::out_of_range);
  CHECK_EX(t.edit(12, 'x'), std::out_of_range);
  CHECK_EX(t.erase(12), std::out_of_range);

  CHECK_EX(t.line_start(4), std::out_of_range);
  CHECK_EX(t.line_start(40), std::out_of_range);
  CHECK_EX(t.line_length(4), std::out_of_range);
  CHECK_EX(t.line_length(6), std::out_of_range);
  CHECK_EX(t.char_to_line(12), std::out_of_range);
  CHECK_EX(t.char_to_line(25), std::out_of_range);
}

int main() {
  int ok = 0, fail = 0;

//    std::string text = "/nabc\\ndef\\ghi\\n";
//  TextEditorBackend TB ( text );
//    for ( size_t i = 0; i <= text . size(); i ++ )
//    {
//        assert(TB.size() == text . size());
//        assert(TB.lines());
//        assert(TB.at(0));
//        assert(TB.edit(0, '\n'));
//        assert(TB.insert(0, '\n'));
//        assert(TB.erase(0));
//        assert(TB.line_start(0));
//        assert(TB.line_length(0));
//        assert(TB.char_to_line(0));
//    }
//
    test1(ok, fail);
  std::cout << "test1 passed" << std::endl;

  if (!fail) test2(ok, fail);
    std::cout << "test2 passed" << std::endl;

  if (!fail) test3(ok, fail);
    std::cout << "test3 passed" << std::endl;

  if (!fail) test_ex(ok, fail);
    std::cout << "test4 passed" << std::endl;

  if (!fail) std::cout << "Passed all " << ok << " tests!" << std::endl;
  else std::cout << "Failed " << fail << " of " << (ok + fail) << " tests." << std::endl;
}

#endif


