#include "catch.hpp"
#include "dijkstras.h"
#include "vertex.h"
#include "graph.h"
#include "edge.h"
#include "bfs.h"

#include <unordered_map>
#include <string>
#include <vector>

using std::unordered_map;
using std::vector;
using std::string;
using std::pair;

/**
 * @brief Check whether the each shortest path computed after running a single 
 * source shortest path algorithm matches the expected shortest path for each 
 * vertex in the graph, excluding the starting vertex. The shortest paths are 
 * stored by saving predecessors in each vertex and recovered by backtracking
 * from the end vertex to the starting vertex using the saved predecessors.
 * 
 * Also check whether the computed distance of each shortest path matches the 
 * distance of each expected shortest path. The distances from the starting 
 * vertex to the ending vertex are stored in each ending vertex.
 * 
 * @param graph a Graph object modified by running a single source shortest path 
 * algorithm. The graph will be used extract computed shortest paths and 
 * distances in order to verify for correctness. 
 * @param distances an unordered map of vertex addresses to expected distances 
 * of the shortest path from the start to the vertex indicated by the address.
 * @param paths an unordered map of vertex addresses to vectors of vertex 
 * pointers denoting a path in reverse from end to start. These paths are the 
 * expected shortest path from the start to the vertex indicated by the address.
 */
void verifyShortestPathsAndDistances(
    const Graph& graph, unordered_map<string, uint64_t>& distances, 
    unordered_map<string, vector<Vertex*>>& paths) {
  for (pair<string, uint64_t> p : distances) {
    Vertex* v = graph.getVertex(p.first);

    vector<Vertex*> shortest_path;
    Vertex* current = v;
    while(current != NULL) {
      shortest_path.push_back(current);
      current = current->getParent();
    }

    REQUIRE( paths[p.first] == shortest_path ); // verify correct shortest path
    REQUIRE( v->getDistance() == p.second );    // verify correct distance
  }
}

/**
 *          1
 *    A --------- B
 *     \          |
 *      \       2 |
 *     4 \        |     3
 *        ------- C --------- D
 * 
 * Expected Shortest Paths From Vertex A:
 *  - B: A --> D --> B        (distance:  8)
 *  - C: A --> D --> B --> C  (distance: 15)
 *  - D: A --> D              (distance:  3)
 *
 */
TEST_CASE("Simple Dijkstra's Algorithm - shortest path on small graph", "[dijkstras]") {
  Graph g;

  Vertex* a = g.addVertex("0x1");
  Vertex* b = g.addVertex("0x2");
  Vertex* c = g.addVertex("0x3");
  Vertex* d = g.addVertex("0x4");

  g.addEdge(a, b, 1, 1, 2);
  g.addEdge(a, c, 1, 4, 12);
  g.addEdge(b, c, 1, 2, 3);
  g.addEdge(c, d, 1, 3, 2);

  dijkstra(&g, a);

  unordered_map<string, vector<Vertex*>> paths = {
    {"0x2", {b, a}}, {"0x3", {c, b, a}}, {"0x4", {d, c, b, a}}
  };

  unordered_map<string, uint64_t> distances = {
    {"0x2", 1}, {"0x3", 3}, {"0x4", 6}
  };

  verifyShortestPathsAndDistances(g, distances, paths);
}

/**
 * All Edge Weights are 1:
 *
 *     A ----- B ----- D
 *     |         ____/ | \____
 *     |        /      |      \
 *     C ----- E ----- G ----- F
 *                     |
 *                     H
 *
 * Potential Shortest Paths from A --> H:
 *  - A --> B --> D --> G --> H
 *  - A --> C --> E --> G --> H
 *
 * Expected Shortest Paths From Vertex A:
 *  - B: A --> B                   (distance: 1)
 *  - C: A --> C                   (distance: 1)
 *  - D: A --> B --> D             (distance: 2)
 *  - E: A --> C --> E             (distance: 2)
 *  - F: A --> B --> D --> F       (distance: 3)
 *  - G: A --> C --> E --> G       (distance: 3)
 *  - H: A --> C --> E --> G --> H (distance: 4)
 *
 */
TEST_CASE("Simple Dijkstra's Algorithm - shortest path on graph with equal edge weights and multiple shortest paths", "[dijkstras]") {
  Graph graph;

  Vertex* a = graph.addVertex("0x1");
  Vertex* b = graph.addVertex("0x2");
  Vertex* c = graph.addVertex("0x3");
  Vertex* d = graph.addVertex("0x4");
  Vertex* e = graph.addVertex("0x5");
  Vertex* f = graph.addVertex("0x6");
  Vertex* g = graph.addVertex("0x7");
  Vertex* h = graph.addVertex("0x8");

  graph.addEdge(a, b, 1, 1, 50);
  graph.addEdge(a, c, 1, 1, 21);
  graph.addEdge(b, d, 1, 1, 42);
  graph.addEdge(c, e, 1, 1, 57);
  graph.addEdge(d, e, 1, 1, 53);
  graph.addEdge(d, f, 1, 1, 57);
  graph.addEdge(d, g, 1, 1, 40);
  graph.addEdge(e, g, 1, 1, 63);
  graph.addEdge(f, g, 1, 1, 51);
  graph.addEdge(g, h, 1, 1, 1);

  dijkstra(&graph, a);

  unordered_map<string, vector<Vertex*>> paths = {
    {"0x2", {b, a}},       {"0x3", {c, a}},          {"0x4", {d, b, a}},
    {"0x5", {e, c, a}},    {"0x6", {f, d, b, a}}, 
    {"0x7", {g, e, c, a}}, {"0x8", {h, g, e, c, a}}
  };

  unordered_map<string, uint64_t> distances = {
    {"0x2", 1}, {"0x3", 1}, {"0x4", 2}, {"0x5", 2}, 
    {"0x6", 3}, {"0x7", 3}, {"0x8", 4}
  };

  verifyShortestPathsAndDistances(graph, distances, paths);
}

/**
 * Example adapted from Lecture #38, changed to make graph undirected
 * 
 *                        7
 *          ------- B --------- C
 *      10 /        |     ____/ |
 *        /       5 |    /      |
 *       /    3     |   / 6     |
 *      A --------- D  /        |
 *      |             /         | 4
 *      |      5     /   3      |
 *    7 |    ------ E ------    |
 *      |   /       |       \   |
 *      |  /        | 2      \  |
 *      | /         |         \ |
 *      F --------- G --------- H
 *            4           5
 * 
 * Expected Shortest Paths From Vertex A:
 *  - B: A --> D --> B        (distance:  8)
 *  - C: A --> D --> B --> C  (distance: 15)
 *  - D: A --> D              (distance:  3)
 *  - E: A --> F --> E        (distance: 12)
 *  - F: A --> F              (distance:  7)
 *  - G: A --> F --> G        (distance: 11)
 *  - H: A --> F --> E --> H  (distance: 15)
 * 
 */
TEST_CASE("Complex Dijkstra's Algorithm - single connected component with different edge weights", "[dijkstras]") {
  Graph graph;

  Vertex* a = graph.addVertex("0x1");
  Vertex* b = graph.addVertex("0x2");
  Vertex* c = graph.addVertex("0x3");
  Vertex* d = graph.addVertex("0x4");
  Vertex* e = graph.addVertex("0x5");
  Vertex* f = graph.addVertex("0x6");
  Vertex* g = graph.addVertex("0x7");
  Vertex* h = graph.addVertex("0x8");

  graph.addEdge(a, b, 0, 10, 0);
  graph.addEdge(a, f, 0, 7, 0);
  graph.addEdge(a, d, 0, 3, 0);
  graph.addEdge(b, c, 0, 7, 0);
  graph.addEdge(b, d, 0, 5, 0);
  graph.addEdge(c, h, 0, 4, 0);
  graph.addEdge(c, e, 0, 6, 0);
  graph.addEdge(h, g, 0, 5, 0);
  graph.addEdge(h, e, 0, 3, 0);
  graph.addEdge(g, e, 0, 2, 0);
  graph.addEdge(g, f, 0, 4, 0);
  graph.addEdge(f, e, 0, 5, 0);

  dijkstra(&graph, a);

  unordered_map<string, vector<Vertex*>> paths = {
    {"0x2", {b, d, a}},    {"0x3", {c, b, d, a}},  {"0x4", {d, a}},    
    {"0x5", {e, f, a}},    {"0x6", {f, a}},        {"0x7", {g, f, a}},
    {"0x8", {h, e, f, a}}
  };

  unordered_map<string, uint64_t> distances = {
    {"0x2", 8}, {"0x3", 15}, {"0x4", 3}, {"0x5", 12},
    {"0x6", 7}, {"0x7", 11}, {"0x8", 15}
  };

  verifyShortestPathsAndDistances(graph, distances, paths);
}
