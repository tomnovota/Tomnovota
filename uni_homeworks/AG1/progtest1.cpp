#ifndef __PROGTEST__
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum Point : size_t {};

struct Path {
  Point from, to;
  unsigned length;

  Path(size_t f, size_t t, unsigned l) : from{f}, to{t}, length{l} {}

  friend bool operator == (const Path& a, const Path& b) {
    return std::tie(a.from, a.to, a.length) == std::tie(b.from, b.to, b.length);
  }
  
  friend bool operator != (const Path& a, const Path& b) { return !(a == b); }
};

#endif

enum Predecesor {
  ROOT,
  KNOWN,
  UNKNOWN
};

class Succesor
{
  public:
    Succesor ( void ) = default;
    Succesor ( const Point & suc, unsigned len )
    : m_Point ( suc )
    , m_Length ( len )
    {}

    const Point & GetPoint ( void ) const { return m_Point; }
    
    const unsigned & Length ( void ) const { return m_Length; }

  private:
    Point m_Point;
    unsigned m_Length;
};

class Vertex
{
  public:
    Vertex ( void ) = default;

    void SetPoint ( Point p ) { m_Point = p; }

    const Point & GetPoint ( void ) const { return m_Point; }

    void AddSuccessor ( const Point & suc, unsigned len )
    {
        m_Successors . push_back ( Succesor ( suc, len ) );
    }

    void IncrInDegree ( void ) { m_In_Degree ++; }

    size_t InDegree ( void ) const { return m_In_Degree; }

    void IncrInPassed ( void ) { m_In_Passed ++; }

    bool Visited ( void ) const { return m_In_Passed == m_In_Degree; }

    Predecesor & PredecesorType ( void ) { return m_Predecesor_Type; }

    const Predecesor & PredecesorType ( void ) const { return m_Predecesor_Type; }

    const std::vector< Succesor > & GetSuccesors ( void ) const { return m_Successors; }

    unsigned BestLen ( void ) const { return m_Best_Len; }

    void SetBestPredecesor ( const Point & p, unsigned len )
    {
        m_Best_Predecesor = p;
        m_Best_Len = len;
        m_Predecesor_Type = KNOWN;
    }

    Point GetPredecesor ( void ) const { return m_Best_Predecesor; }

  private:
    Point m_Point;
    Point m_Best_Predecesor;
    Predecesor m_Predecesor_Type = UNKNOWN;
    size_t m_In_Degree = 0;
    size_t m_In_Passed = 0;
    unsigned m_Best_Len = 0;
    std::vector< Succesor > m_Successors;    
};

class Graph
{
  public:
    Graph ( void ) = default;
    Graph ( size_t points, const std::vector< Path > & all_paths )
    {
        m_Arr . resize ( points );
        for ( const auto & path : all_paths )
        {
            m_Arr [ path . from ] . AddSuccessor ( path . to, path . length );
            m_Arr [ path . to ] . IncrInDegree();
        }
    }

    void PrepareBFS ( std::queue< Point > & Q )
    {
        size_t i = 0;
        for ( auto & ver : m_Arr )
        {
            m_Arr [ i ] . SetPoint ( Point ( i ) );
            i ++;
            if ( ver . InDegree() == 0 )
            {
                Q . push ( ver . GetPoint() );
                ver . PredecesorType() = ROOT;
            }
        }
    }

    Vertex & operator [] ( const Point & p )
    {
        return m_Arr [ p ];
    }

    const std::vector< Vertex > & Get ( void ) const { return m_Arr; }

  private:
    std::vector< Vertex > m_Arr;
};

std::vector< Path > longest_track ( size_t points, const std::vector< Path > & all_paths )
{
    Graph G ( points, all_paths );
    std::queue< Point > Q;
    G . PrepareBFS ( Q );
    while ( ! Q . empty() )
    {
        const Vertex & v = G [ Q . front() ];
        Q . pop();
        for ( const auto & suc : v . GetSuccesors() )
        {
            G [ suc . GetPoint() ] . IncrInPassed();
            if ( G [ suc . GetPoint() ] . BestLen() < v . BestLen() + suc . Length() ) // "<="
            {
                // std::cout << "suc = " << suc . GetPoint() << "v = " << v . GetPoint() << std::endl;
                G [ suc . GetPoint() ] . SetBestPredecesor ( v . GetPoint(), v . BestLen() + suc . Length() );
            }
            if ( G [ suc . GetPoint() ] . Visited() )
                Q . push ( suc . GetPoint() );
        }
    }
    unsigned max_len = 0;
    const Vertex * end_of_best_path = nullptr;
    for ( const auto & ver : G . Get() )
    {
        if ( ver . BestLen() >= max_len )
        {
            max_len = ver . BestLen();
            end_of_best_path = & ver;
        }
    }
    std::vector< Path > longest_path;
    Vertex & x = G [ end_of_best_path -> GetPredecesor() ],
                   y = * end_of_best_path;
    while ( y . PredecesorType() != ROOT )
    {
        // std::cout << "x = " << x . GetPoint() << std::endl;
        // std::cout << "y = " << y . GetPoint() << std::endl;
        longest_path . push_back ( Path ( x . GetPoint(), y . GetPoint(), ( y . BestLen() - x . BestLen() ) ) );
        y = x;
        x = G [ x . GetPredecesor() ];
    }
    return std::vector< Path > ( longest_path . rbegin(), longest_path . rend() );
}


#ifndef __PROGTEST__


struct Test {
  unsigned longest_track;
  size_t points;
  std::vector<Path> all_paths;
};

inline const Test TESTS[] = {
  {13, 5, { {3,2,10}, {3,0,9}, {0,2,3}, {2,4,1} } },
  {11, 5, { {3,2,10}, {3,1,4}, {1,2,3}, {2,4,1} } },
  {16, 8, { {3,2,10}, {3,1,1}, {1,2,3}, {1,4,15} } },
};

#define CHECK(cond, ...) do { \
    if (cond) break; \
    printf("Fail: " __VA_ARGS__); \
    printf("\n"); \
    return false; \
  } while (0)

bool run_test(const Test& t) {
  auto sol = longest_track(t.points, t.all_paths);

  unsigned length = 0;
  for (auto [ _, __, l ] : sol) length += l;

  CHECK(t.longest_track == length,
    "Wrong length: got %u but expected %u", length, t.longest_track);

  for (size_t i = 0; i < sol.size(); i++) {
    CHECK(std::count(t.all_paths.begin(), t.all_paths.end(), sol[i]),
      "Solution contains non-existent path: %zu -> %zu (%u)",
      sol[i].from, sol[i].to, sol[i].length);

    if (i > 0) CHECK(sol[i].from == sol[i-1].to,
      "Paths are not consecutive: %zu != %zu", sol[i-1].to, sol[i].from);
  }

  return true;
}
#undef CHECK

int main() {
  int ok = 0, fail = 0;

  for (auto&& t : TESTS) (run_test(t) ? ok : fail)++;
  
  if (!fail) printf("Passed all %i tests!\n", ok);
  else printf("Failed %u of %u tests.\n", fail, fail + ok);
}

#endif