/*
 * # TopSort
 * 
 * Your task is to implement the function `std::pair<bool, std::vector<Vertex>>
 * topsort(const Graph& G)`. It should return either
 * 
 * - `true` and a list of all vertices of `G` in a topological order or
 * - `false` and a list of vertices that form a cycle (there must be an
 *   edge from the last to the first vertex).
 * 
 * The class `Graph` represents a directed graph. Important methods are:
 * 
 * - `vertices()`: The number of vertices.
 * - `operator[](Vertex v)`: A list of successors of `v`. Vertices are integers starting with 0.
 * - Methods `begin()` and `end()` allow iteration over all vertices and
 *   using `Graph` in range-for like `for (Vertex v : G) ...`.
 * - `reversed()`: Returns a new graph created by flipping the direction of all edges.
 * 
 * The time limit is 5 seconds for the small and 3 seconds for the large
 * test.
 * 
 */

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
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <random>
#include <type_traits>


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

#define CHECK(succ, ...) do { \
    if (!(succ)) throw TestFailed(fmt(__VA_ARGS__)); \
  } while (0)

enum Vertex : size_t {
  NO_VERTEX = -size_t(1),
  ROOT = -size_t(2)
};

struct Graph {
  Graph() : Graph(0) {}
  explicit Graph(size_t vertices) : _adj(vertices) {}
  Graph(const std::vector<std::vector<size_t>>& adj) : Graph(adj.size()) {
    for (size_t i = 0; i < adj.size(); i++)
      for (size_t v : adj[i]) add_edge(Vertex{i}, Vertex{v});
  }

  size_t vertices() const { return _adj.size(); }

  void add_edge(Vertex u, Vertex v) {
    _adj[u].push_back(v);
  }

  const std::vector<Vertex>& operator [] (Vertex v) const {
    CHECK(size_t(v) < _adj.size(),
      "Graph: index %zu out of range [0..%zu).", size_t(v), _adj.size());
    return _adj[v];
  }

  Graph reversed() const {
    Graph ret(vertices());
    for (Vertex v : *this) for (Vertex w : operator[](v))
      ret.add_edge(w, v);
    return ret;
  }
  
  struct Iterator {
    Iterator() = default;

    Iterator& operator ++ () { _v++; return *this; }
    Vertex operator * () const { return Vertex{_v}; }

    friend bool operator == (Iterator a, Iterator b) { return a._v == b._v; }
    friend bool operator != (Iterator a, Iterator b) { return !(a == b); }

    private:
    friend struct Graph;
    Iterator(size_t v) : _v(v) {}

    size_t _v = NO_VERTEX;
  };

  Iterator begin() const { return { 0 }; }
  Iterator end() const { return { vertices() }; }

  private:
  std::vector<std::vector<Vertex>> _adj;
};

std::ostream& operator << (std::ostream& out, const Graph& G) {
  out << "{{ ";
  for (Vertex v : G) {
    out << v << "{";
    for (Vertex w : G[v]) out << w << ",";
    out << "}, ";
  }
  return out << "}}";
}

#endif

struct CPair
{
    CPair ( void ) = default;

    CPair ( Vertex ver )
    : m_Vertex ( ver ) {}

    CPair ( Vertex ver, size_t deg )
    : m_Vertex ( ver ), m_Degree ( deg ) {}

    friend bool operator < ( const CPair & l, const CPair & r );
    friend bool operator < ( const CPair & l, const Vertex & r );

    Vertex m_Vertex;
    size_t m_Degree;
};

bool operator < ( const CPair & l, const CPair & r )
{
    return l . m_Vertex < r . m_Vertex;
}

class CSet
{
  public:
    CSet ( const Graph& G )
    {
        m_Arr . resize ( G . vertices() );

        size_t i = 0;
        for ( auto ptr = G . begin(); ptr != G . end(); ++ ptr, ++ i )
            m_Arr [ i ] = CPair ( * ptr, 0 );

        std::sort ( m_Arr . begin(), m_Arr . end() );
    }

    size_t & operator [] ( Vertex vertex )
    {
        auto it = std::lower_bound ( m_Arr . begin(), m_Arr . end(), CPair ( vertex ) );
        return it -> m_Degree;
    }

    void del ( Vertex vertex )
    {
        auto it = std::lower_bound ( m_Arr . begin(), m_Arr . end(), CPair ( vertex ) );
        if ( it != m_Arr . end() )
            m_Arr . erase ( it );
    }

    Vertex get_last ( void )
    {
        return m_Arr . back() . m_Vertex;
    }

    std::vector < Vertex > zero_deg ( void )
    {
        std::vector < Vertex > res;
        for ( size_t i = 0; i < m_Arr . size(); ++ i )
        {
            if ( m_Arr [ i ] . m_Degree == 0 )
            {
                res . push_back ( m_Arr [ i ] . m_Vertex );
                m_Arr . erase ( m_Arr . begin() + (i --) );
            }
        }
        return res;
    }

    bool contains ( Vertex ver )
    {
        auto it = std::lower_bound ( m_Arr . begin(), m_Arr . end(), CPair ( ver ) );
        return it != m_Arr . end();
    }

    size_t size ( void ) { return m_Arr . size(); }

  private:
    std::vector< CPair > m_Arr;
};

/* Tento vrchol vložit do rekurzivní funkce, která přijme vrchol cn a c1, pro všechny následníky spustím stejnou rekurzivní funkci, pokud je nějaký následník = c1, potom vrátí rekurzivní funkce true a všechny vrcholy, které vrátí true se uloží do výsledné kružnice */
bool findCycle ( const Graph & G, Vertex cn, Vertex c1, std::vector< Vertex > & cycle, CSet & delta )
{
    for ( const auto & w : G [ cn ] )
    {
        if ( w == c1
            || findCycle ( G, w, c1, cycle, delta ) )
        {
            // std::cout << "CN = " << cn << std::endl;
            cycle . push_back ( w );
            delta . del ( w );
            return true;
        }
        delta . del ( w );
    }
    return false;
}

// Algoritmus TopSort(orientovaný G)

// Returns either true and a topological order or false and a cycle
std::pair<bool, std::vector<Vertex>> topsort(const Graph& G)
{
    std::vector< Vertex > topology;
    // (1) Q je prázdná fronta
    std::deque< Vertex > Q;

    // (2) δ[] = pole vstupních stupňů vrcholů G, na počátku vynulované
    CSet delta ( G );

    // (3) Pro každou hranu (u, v) ∈ E(G):
    for ( const auto & u : G )
    {
        for ( const auto & v : G [ u ] )
        {
            // (4) δ[v] + +
            delta [ v ] ++;
        }
    }
    // (5) Vlož do Q všechny vrcholy z s δ[z] = 0
    auto zeros = delta . zero_deg();
    Q . insert ( Q . begin(), zeros . begin(), zeros . end() );

    // (6) Dokud fronta Q není prázdná:
    while ( ! Q . empty() )
    {
        // (7) Odeber prvek z ze začátku fronty Q
        auto z = Q . front();
        Q . pop_front();

        // (8) Vypiš z
        topology . push_back ( z );

        // (9) Pro každou hranu (z, w) vedoucí ze z:
        for ( const auto & w : G [ z ] )
        {
            // (10) δ[w] --
            delta [ w ] --;

            // (11) Pokud δ[w] = 0: 
            if ( delta [ w ] == 0 )
            {
                // zařaď w do Q
                Q . push_back ( w );
                delta . del ( w );
            }
        }
    }
    // (12)Pokud nebyly zpracovány všechny vrcholy:
    while ( delta . size() > 0 )
    {
        std::vector< Vertex > cycle;
        Vertex c1 = delta . get_last();

        // (13) graf G obsahuje orientovaný cyklus
        if ( findCycle ( G, c1, c1, cycle, delta ) )
        {
            for ( const auto & el : cycle ) std::cout << el << " ";
            std::cout << std::endl;
            return std::make_pair ( false, std::vector< Vertex > ( cycle . rbegin(), cycle . rend() ) );
        }
    }
    return std::make_pair ( true, topology );
}

#ifndef __PROGTEST__

const Graph SMALL_DAGS[] = {
  {{ {1}, {2}, {3}, {4}, {} }},
  {{ {1}, {2, 4}, {3}, {4}, {}, {} }},
  {{ {1}, {2, 5}, {3}, {4}, {}, {} }},
};

const Graph SMALL_CYCLIC[] = {
  {{ {1}, {2}, {3}, {4}, {0} }},
  {{ {1}, {2, 5}, {3}, {4}, {0}, {4} }},
};

struct RandomGraphGenerator {
  RandomGraphGenerator(uint32_t seed) : my_rand(seed) {}

  uint32_t num(uint32_t max) { return my_rand() % max; }
  Vertex vertex(const Graph& G) { return Vertex{num(G.vertices())}; }

  Graph graph1(uint32_t s, size_t edges) {
    Graph G(s);
    double rev_chance = 1.2 / edges;

    while (edges--) {
      auto u = vertex(G);
      auto v = vertex(G);
      if (u == v) continue;
      if (u < v) std::swap(u, v);
      if (num(1'000'000'000) <= rev_chance * 1'000'000'000) std::swap(u, v);
      G.add_edge(u, v);
    }

    return G;
  }

  Graph graph2(uint32_t s, double density) {
    Graph G(s);
    double rev_chance = 0.8 / (s*s/2);

    for (Vertex u : G) for (Vertex v : G) {
      if (u < v) {
        if (num(1'000'000'000) < 1'000'000'000*density) G.add_edge(u, v);
      } else {
        if (num(1'000'000'000) < 1'000'000'000*rev_chance) G.add_edge(u, v);
      }
    }

    return G;
  }

  Graph cycle(uint32_t n) {
    Graph G(n);
    for (uint32_t i = 0; i < n; i++)
      G.add_edge(Vertex{i}, Vertex{(i + 1) % n});
    return G;
  }

  private:
  std::mt19937 my_rand;
};


void verify_toporder(const Graph& G, const std::vector<Vertex>& order) {
  CHECK(order.size() == G.vertices(),
    "Top order has %zu vertices but the graph has %zu vertices.",
    order.size(), G.vertices()
  );

  std::vector<size_t> index(G.vertices());
  for (size_t i = 0; i < order.size(); i++) index[order[i]] = i;

  for (Vertex v : G) for (Vertex w : G[v]) CHECK(index[v] < index[w],
    "Edge %zu --> %zu goes backwards.", size_t(v), size_t(w));
}

void verify_cycle(const Graph& G, const std::vector<Vertex>& cycle) {
  auto has_edge_to = [&](Vertex u, Vertex v) {
    for (Vertex w : G[u]) if (w == v) return true;
    return false;
  };

  CHECK(cycle.size(), "Cycle has length zero.");
  CHECK(has_edge_to(cycle.back(), cycle.front()),
    "Missing edge from last (%zu) to first vertex (%zu).",
    size_t(cycle.back()), size_t(cycle.front())
  );

  for (size_t i = 1; i < cycle.size(); i++) CHECK(has_edge_to(cycle[i-1], cycle[i]),
    "Missing edge from vertex %zu to vertex %zu.", size_t(cycle[i-1]), size_t(cycle[i]));
}

void test_topsort_inner(const Graph& G) {
  auto [ is_dag, data ] = topsort(G);
  // std::cout << is_dag;

  std::vector<bool> seen(G.vertices(), false);
  for (Vertex v : data) {
    CHECK(v < G.vertices(),
      "Vertex %zu >= # of vertices == %zu.", size_t(v), G.vertices());

    CHECK(!seen[v], "Vertex %zu is repeated.", size_t(v));
    seen[v] = true;
  }

  if (is_dag) verify_toporder(G, data);
  else verify_cycle(G, data);
}

void test_topsort(const Graph& G) {
  try {
    test_topsort_inner(G);
  } catch (const TestFailed& e) {
    std::cout << "Test failed: G = " << G << "\n"
              << e.what() << std::endl;
    throw;
  }
}


void run_tests() {
  std::cout << "Small DAGs..." << std::endl;
  RandomGraphGenerator rgg(53323);
  for (const Graph& G : SMALL_DAGS) test_topsort(G);

  std::cout << "Small cyclic graphs..." << std::endl;
  for (const Graph& G : SMALL_CYCLIC) test_topsort(G);

  std::cout << "Small random graphs..." << std::endl;
  for (size_t i = 0; i < 30; i++) {
    Graph G = rgg.graph1(20 + i, 14 + i);
    test_topsort(G);
  }
  for (size_t i = 0; i < 30; i++) {
    Graph G = rgg.graph2(10 + i, 0.7);
    test_topsort(G);
  }

  std::cout << "Big random graphs..." << std::endl;
  for (size_t i = 0; i < 100; i++) {
    Graph G = rgg.graph1(11'000 + 50*i, 50'000 + 50*i);
    test_topsort(G);
  }
  for (size_t i = 0; i < 20; i++) {
    Graph G = rgg.graph2(900 + i, 0.7);
    test_topsort(G);
  }
  std::cout << "Long cycle..." << std::endl;
  test_topsort(rgg.cycle(50'000));
}

int main() {
  try {
    run_tests();

    std::cout << "All tests passed." << std::endl;
  } catch (const TestFailed&) {}
}

#endif
