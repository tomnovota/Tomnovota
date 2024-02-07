#ifndef __PROGTEST__
#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <array>
#include <random>
#include <type_traits>

// We use std::set as a reference to check our implementation.
// It is not available in progtest :)
#include <set>

template < typename T >
struct Ref {
  size_t size() const { return _data.size(); }
  const T* find(const T& value) const {
    auto it = _data.find(value);
    if (it == _data.end()) return nullptr;
    return &*it;
  }
  bool insert(const T& value) { return _data.insert(value).second; }
  bool erase(const T& value) { return _data.erase(value); }

  auto begin() const { return _data.begin(); }
  auto end() const { return _data.end(); }

  private:
  std::set<T> _data;
};

#endif

/**********************************************************************************************************************************/

namespace config {
  // Enable to check that the tree is AVL balanced.
  inline constexpr bool CHECK_DEPTH = true;

  // Disable if your implementation does not have parent pointers
  inline constexpr bool PARENT_POINTERS = true;
}

template < typename T >
struct Tree
{
    struct Node
    {
        Node ( void ) = default;
        Node ( T val, Node * parent )
        : m_Val ( new T ( val ) )
        , m_Parent ( parent )
        {}

        ~Node ( void )
        {
            delete m_Val;
        }

        char Sgn ( void )
        {
            return (m_Right
                    ? m_Right -> m_Height
                    : 0
                    ) -
                    (m_Left
                    ? m_Left -> m_Height
                    : 0);
        }

        T * m_Val;
        Node * m_Parent = nullptr;
        Node * m_Left = nullptr;
        Node * m_Right = nullptr;
        size_t m_Height = 1;
    };

    ~Tree ( void )
    {
        Destroy ( m_Root );
    }

    void Destroy ( Node * v )
    {
        if ( ! v ) return;
        Destroy ( v -> m_Left );
        Destroy ( v -> m_Right );
        delete v;
    }

    void print ( void )
    {
        std::cout << "ROOT = " << (m_Root ? *(m_Root -> m_Val) : 0) << std::endl;
        show ( m_Root, 0 );
        std::cout << std::endl;
    }

    void show ( Node * v, size_t h )
    {
        if ( ! v )
            return;

        show ( v -> m_Left, h + 1 );
        
        for ( size_t i = 0; i < h; ++ i )
            std::cout << " ";
        std::cout << *(v -> m_Val) << " [";
        std::cout << int(v -> Sgn()) << "], (" << v -> m_Height << ")" << std::endl;

        show ( v -> m_Right, h + 1 );
    }

    // počet prvků ve stromě
    size_t size () const
    {
        return m_Size;
    }

    // Nalezne prvek t ve stromu a vrátí na něj const ukazatel, nebo nullptr, pokud prvek ve stromu není.
    const T * find ( const T & value ) const
    {
        Node * ptr = BVSFind ( m_Root, value );
        if ( ptr )
            return ptr -> m_Val;
        return nullptr;
    }

    // Pokusí se vložit t. Vrátí, zda se vložení povedlo. Vložení selže, pokud strom již prvek se stejnou hodnotou obsahuje.
    bool insert ( T value )
    {
//        std::cout << "Inserting: " << value << std::endl;
        bool r = BVSInsert ( m_Root, & value );
        return r;
//        return print(), r;
    }

    // Smaže prvek t, pokud je t ve stromě. Vrací, zda byl prvek smazán.
    bool erase ( const T & value )
    {
//        std::cout << "Deleting: " << value << std::endl;
        bool r = BVSDelete ( m_Root, & value );
        return r;
//        return print(), r;
    }

    /******************
     * Vlastní metody.
     ******************/

    size_t GetHeight ( Node * v )
    {
        return v ? v -> m_Height : 0;
    }

    void UpdateHeight ( Node * v )
    {
        if ( ! v )
            return;

        size_t tmp = 1 + std::max ( GetHeight ( v -> m_Left ), GetHeight ( v -> m_Right ) );

        if ( tmp != v -> m_Height )
        {
//            std::cout << "Updating... " << *(v -> m_Val) << " tmp = " << tmp << std::endl;
            v -> m_Height = tmp;
            UpdateHeight ( v -> m_Parent );
        }
    }

    Node * BVSMin ( Node * v ) const
    {
        while ( v -> m_Left )
            v = v -> m_Left;
        return v;
    }

    Node * BVSFind ( Node * v, const T & value ) const
    {
        while ( v )
        {
            if ( value == * ( v -> m_Val ) ) // v je vrchol s klíčem value
            {
                return v;
            }
            if ( value < * ( v -> m_Val ) ) // value patří do L(v)
            {
                v = v -> m_Left;
                continue;
            }
            if ( value > * ( v -> m_Val ) ) // value patří do R(v)
            {
                v = v -> m_Right;
                continue;
            }
        }
        return nullptr;
    }

    bool BVSInsert ( Node * v, const T * val )
    {
        if ( ! v )
        {
            m_Root = new Node ( * val, nullptr );
            UpdateHeight ( v );
            ++ m_Size;
            return true;
        }
        while ( v )
        {
            if ( * ( v -> m_Val ) == * val ) // element already exists
                return false;

            if ( * val < * ( v -> m_Val ) ) // prvek patří do L(curr)
            {
                if ( v -> m_Left ) // vlož do existující L(curr)
                {
                    v = v -> m_Left;
                    continue;
                }
                v -> m_Left = new Node ( * val, v );
                UpdateHeight ( v );
                Balance ( v, true );
                ++ m_Size;
                return true;
            }
            if ( * val > * ( v -> m_Val ) ) // prvek patří do R(curr)
            {
                if ( v -> m_Right ) // vlož do existující R(curr)
                {
                    v = v -> m_Right;
                    continue;
                }
                v -> m_Right = new Node ( * val, v );
                UpdateHeight( v );
                Balance ( v, true );
                ++ m_Size;
                return true;
            }
        }
        return false;
    }

    bool BVSDelete ( Node * v, const T * val )
    {
        if ( ! v ) // empty tree
            return false;
        
        while ( v )
        {
            if ( * ( v -> m_Val ) == * val ) // element found
            {
                if ( ! v -> m_Left && ! v -> m_Right ) // mazaný vrchol je list
                {
                    if ( v -> m_Parent ) // modify predecessor
                    {
                        if ( LeftNode ( v ) )
                            v -> m_Parent -> m_Left = nullptr;
                        else
                            v -> m_Parent -> m_Right = nullptr;
                    }
                    else
                        m_Root = nullptr;
                }
                else if ( v -> m_Left && ! v -> m_Right ) // mazaný vrchol následuje jen levým podstromem
                {
                    if ( v -> m_Parent )
                    {
                        if ( LeftNode ( v ) ) // modify predecessor
                            v -> m_Parent -> m_Left = v -> m_Left;
                        else
                            v -> m_Parent -> m_Right = v -> m_Left;
                    }
                    else
                    {
                        m_Root = v -> m_Left;
                    }
                    v -> m_Left -> m_Parent = v -> m_Parent; // modify successor
                }
                else if ( ! v -> m_Left && v -> m_Right ) // mazaný vrchol následuje jen pravým podstromem
                {
                    if ( v -> m_Parent )
                    {
                        if ( LeftNode ( v ) ) // modify predecessor
                            v -> m_Parent -> m_Left = v -> m_Right;
                        else
                            v -> m_Parent -> m_Right = v -> m_Right;
                    }
                    else
                    {
                        m_Root = v -> m_Right;
                    }
                    v -> m_Right -> m_Parent = v -> m_Parent; // modify successor
                }
                else // mazaný vrchol má oba potomky
                {
                    Node * min = BVSMin ( v -> m_Right );
                    delete v -> m_Val;
                    v -> m_Val = new T ( * (min -> m_Val) );
                    return BVSDelete ( min, min -> m_Val );
                }
                -- m_Size;
                UpdateHeight ( v -> m_Parent );
                Balance ( v -> m_Parent, false );
                delete v;
                return true;
            }
            if ( * val < * ( v -> m_Val ) ) // prvek patří do L(curr)
            {
                if ( v -> m_Left ) // prohledej L(curr)
                {
                    v = v -> m_Left;
                    continue;
                }
                return false; // prvek neexistuje
            }
            if ( * val > * ( v -> m_Val ) ) // prvek patří do R(curr)
            {
                if ( v -> m_Right ) // prohledej R(curr)
                {
                    v = v -> m_Right;
                    continue;
                }
                return false; // prvek neexistuje
            }
        }
        return false;
    }

    void Balance ( Node * v, bool insert )
    {
        if ( ! v )
            return;

//        std::cout << "Balancing... " << *(v -> m_Val) << " sgn = " << int(v -> Sgn()) << std::endl;
        switch ( v -> Sgn() )
        {
            case 0:
                if ( ! insert )
                    Balance ( v -> m_Parent, insert );
                return;
            case 2:
            {
                if ( v -> m_Right -> Sgn() == -1 )
                    RotateRight ( v -> m_Right );

                if ( ! insert )
                    return RotateLeft ( v, true );
                return RotateLeft ( v );
            }
            case -2:
            {
                if ( v -> m_Left -> Sgn() == 1 )
                    RotateLeft ( v -> m_Left );

                if ( ! insert )
                    return RotateRight ( v, true );
                return RotateRight ( v );
            }
            default: // +1, -1
                if ( insert )
                    Balance ( v -> m_Parent, insert );
                return;
        };
    }

    void RotateLeft ( Node * x, bool edge = false )
    {
        Node * y = x -> m_Right;
        // std::cout << "  Rotate Left : " << *(x -> m_Val) << " <-> " << *(y -> m_Val) << std::endl;

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

        // predecessor references
        if ( x -> m_Right )
            x -> m_Right -> m_Parent = x;
        y -> m_Parent = x -> m_Parent;
        x -> m_Parent = y;

        UpdateHeight ( x );
        UpdateHeight ( y );
        if ( y -> m_Parent )
        {
            UpdateHeight(y -> m_Parent);
            if ( edge )
                Balance ( y -> m_Parent, false );
        }
    }

    void RotateRight ( Node * x, bool edge = false )
    {
        Node * y = x -> m_Left;
        // std::cout << "  Rotate Right : " << *(x -> m_Val) << " <-> " << *(y -> m_Val) << std::endl;

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

        // predecessor references
        if ( x -> m_Left )
            x -> m_Left -> m_Parent = x;
        y -> m_Parent = x -> m_Parent;
        x -> m_Parent = y;

        UpdateHeight ( x );
        UpdateHeight ( y );
        if ( y -> m_Parent )
        {
            UpdateHeight(y -> m_Parent);
            if ( edge )
                Balance ( y -> m_Parent, false );
        }
    }

    bool LeftNode ( Node * v )
    {
        return ! v -> m_Parent || v -> m_Parent -> m_Left == v;
    }

    /************
     * Proměnné.
     ************/
    size_t m_Size = 0;
    Node * m_Root = nullptr;

    // Needed to test the structure of the tree.
    struct TesterInterface
    {
        // Replace Node with the real type of your nodes
        // and implementations with the ones matching
        // your attributes.
        // using Node = ...;

        // Vrátí pointer na kořen stromu. (Na uzel, nikoliv na hodnotu v něm uloženou.)
        static const Node * root ( const Tree * t )
        {
            return t -> m_Root;
        }

        // Parent of root must be nullptr
        // Vrátí pointer na otce uzlu n. Pro kořen vrátí nullptr. Pokud vaše implementace nemá ukazatele na otce a máte nastaveno config::PARENT_POINTERS na false, můžete metodu smazat nebo vždy vracet nullptr.
        static const Node * parent ( const Node * n )
        {
            return n -> m_Parent;
        }
        
        // Vrátí pointer na pravého syna n nebo nullptr, pokud neexistuje.
        static const Node * right ( const Node * n )
        {
            return n -> m_Right;
        }

        // Vrátí pointer na levého syna n nebo nullptr, pokud neexistuje.
        static const Node * left ( const Node * n )
        {
            return n -> m_Left;
        }

        // Vrátí const referenci na hodnotu uloženou v uzlu n.
        static const T & value ( const Node * n )
        {
            return * ( n -> m_Val );
        }
    };
};

/**********************************************************************************************************************************/

#ifndef __PROGTEST__

struct TestFailed : std::runtime_error {
  using std::runtime_error::runtime_error;
};

std::string fmt(const char *f, ...) {
  va_list args1;
  va_list args2;
  va_start(args1, f);
  va_copy(args2, args1);
  
  std::string buf(vsnprintf(nullptr, 0, f, args1), '\0');
  va_end(args1);
  
  vsnprintf(buf.data(), buf.size() + 1, f, args2);
  va_end(args2);

  return buf;
}

template < typename T >
struct Tester {
  Tester() = default;

  void size() const {
    size_t r = ref.size();
    size_t t = tested.size();
    if (r != t) throw TestFailed(fmt("Size: got %zu but expected %zu.", t, r));
  }

  void find(const T& x) const {
    auto r = ref.find(x);
    auto t = tested.find(x);
    bool found_r = r != nullptr;
    bool found_t = t != nullptr;

    if (found_r != found_t) _throw("Find mismatch", found_r);
    if (found_r && *t != x) throw TestFailed("Find: found different value");
  }

  void insert(const T& x, bool check_tree_ = false) {
    auto succ_r = ref.insert(x);
    auto succ_t = tested.insert(x);
    if (succ_r != succ_t) _throw("Insert mismatch", succ_r);
    size();
    if (check_tree_) check_tree();
  }

  void erase(const T& x, bool check_tree_ = false) {
    bool succ_r = ref.erase(x);
    auto succ_t = tested.erase(x);
    if (succ_r != succ_t) _throw("Erase mismatch", succ_r);
    size();
    if (check_tree_) check_tree();
  }

  struct NodeCheckResult {
    const T* min = nullptr;
    const T* max = nullptr;
    int depth = -1;
    size_t size = 0;
  };

  void check_tree() const {
    using TI = typename Tree<T>::TesterInterface;
    auto ref_it = ref.begin();
    bool check_value_failed = false;
    auto check_value = [&](const T& v) {
      if (check_value_failed) return;
      check_value_failed = (ref_it == ref.end() || *ref_it != v);
      if (!check_value_failed) ++ref_it;
    };

    auto r = check_node(TI::root(&tested), decltype(TI::root(&tested))(nullptr), check_value);
    size_t t_size = tested.size();

    if (t_size != r.size) throw TestFailed(
      fmt("Check tree: size() reports %zu but expected %zu.", t_size, r.size));

    if (check_value_failed) throw TestFailed(
      "Check tree: element mismatch");

    size();
  }

  template < typename Node, typename F >
  NodeCheckResult check_node(const Node* n, const Node* p, F& check_value) const {
    if (!n) return {};
    
    using TI = typename Tree<T>::TesterInterface;
    if constexpr(config::PARENT_POINTERS) {
      if (TI::parent(n) != p) throw TestFailed("Parent mismatch.");
    }

    auto l = check_node(TI::left(n), n, check_value);
    check_value(TI::value(n));
    auto r = check_node(TI::right(n), n, check_value);

    if (l.max && !(*l.max < TI::value(n)))
      throw TestFailed("Max of left subtree is too big.");
    if (r.min && !(TI::value(n) < *r.min))
      throw TestFailed("Min of right subtree is too small.");

    if (config::CHECK_DEPTH && abs(l.depth - r.depth) > 1) throw TestFailed(fmt(
      "Tree is not avl balanced: (%i) left depth %i and right depth %i.",
      TI::value(n), l.depth, r.depth
    ));

    return {
      l.min ? l.min : &TI::value(n),
      r.max ? r.max : &TI::value(n),
      std::max(l.depth, r.depth) + 1, 1 + l.size + r.size
    };
  }

  static void _throw(const char *msg, bool s) {
    throw TestFailed(fmt("%s: ref %s.", msg, s ? "succeeded" : "failed"));
  }

  Tree<T> tested;
  Ref<T> ref;
};


void test_insert() {
  Tester<int> t;

  for (int i = 0; i < 10; i++) t.insert(i, true);
  for (int i = -10; i < 20; i++) t.find(i);

  for (int i = 0; i < 10; i++) t.insert((1 + i * 7) % 17, true);
  for (int i = -10; i < 20; i++) t.find(i);
}

void test_erase() {
  Tester<int> t;

  for (int i = 0; i < 10; i++) t.insert((1 + i * 7) % 17, true);
  for (int i = -10; i < 20; i++) t.find(i);

  for (int i = 3; i < 22; i += 2) t.erase(i, true);
  for (int i = -10; i < 20; i++) t.find(i);
  
  for (int i = 0; i < 10; i++) t.insert((1 + i * 13) % 17 - 8, true);
  for (int i = -10; i < 20; i++) t.find(i);
  
  for (int i = -4; i < 10; i++) t.erase(i, true);
  for (int i = -10; i < 20; i++) t.find(i);
}

enum RandomTestFlags : unsigned {
  SEQ = 1, NO_ERASE = 2, CHECK_TREE = 4
};

void test_random(size_t size, unsigned flags = 0) {
  Tester<size_t> t;
  std::mt19937 my_rand(24707 + size);

  bool seq = flags & SEQ;
  bool erase = !(flags & NO_ERASE);
  bool check_tree = flags & CHECK_TREE;

  for (size_t i = 0; i < size; i++)
    t.insert(seq ? 2*i : my_rand() % (3*size), check_tree);

  t.check_tree();

  for (size_t i = 0; i < 3*size + 1; i++) t.find(i);

  for (size_t i = 0; i < 30*size; i++) switch (my_rand() % 5) {
    case 1: t.insert(my_rand() % (3*size), check_tree);
      break;
    case 2: if (erase) t.erase(my_rand() % (3*size), check_tree);
      break;
    default:
      t.find(my_rand() % (3*size));
  }

  t.check_tree();
}

int main() {

    Tree<int> t;
    t . insert ( 10 );
    t . insert ( 20 );
    t . insert ( 15 );
    t . insert ( 25 );
    t . insert ( 30 );
    t . insert ( 16 );
    t . insert ( 18 );
    t . insert ( 19 );

    t . erase ( 30 );
    t . erase ( 18 );
    t . erase ( 20 );
    t . erase ( 25 );

    t . print();

  try {
    std::cout << "Insert test..." << std::endl;
    test_insert();

    std::cout << "Erase test..." << std::endl;
    test_erase();
    
    std::cout << "Tiny random test..." << std::endl;
    test_random(20, CHECK_TREE);
    
    std::cout << "Small random test..." << std::endl;
    test_random(200, CHECK_TREE);
    
    std::cout << "Big random test..." << std::endl;
    test_random(50'000);
    
    std::cout << "Big sequential test..." << std::endl;
    test_random(50'000, SEQ);

    std::cout << "All tests passed." << std::endl;
  } catch (const TestFailed& e) {
    std::cout << "Test failed: " << e.what() << std::endl;
  }
}

#endif


