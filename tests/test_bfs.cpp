#include "catch.hpp"
#include "vertex.h"
#include "graph.h"
#include "edge.h"
#include "bfs.h"

#include <fstream>
#include <string>

using std::ifstream;
using std::string;
using std::pair;

/**
 * @brief 
 * 
 * @param g 
 * @param visited 
 */
void wasEntireGraphWasVisited(Graph* g, bool visited) {
  for (Edge* e : g->getEdges()) {
    REQUIRE( e->wasExplored() == visited );
  }

  for (pair<string, Vertex*> p : g->getVertices()) {
    REQUIRE( p.second->wasExplored() == visited );
  }
}

/**
 * Traversal on the following graph: 
 *      
 *      A ----- B
 *      |       |
 *      |       |
 *      C ----- D
 * 
 */
TEST_CASE("Simple BFS on graph with 1 connected component", "[bfs]") {
  Graph g;

	Vertex* a = g.addVertex("0x1");
  Vertex* b = g.addVertex("0x2");
  Vertex* c = g.addVertex("0x3");
  Vertex* d = g.addVertex("0x4");

  g.addEdge(a, b, 1, 1, 1);
  g.addEdge(a, c, 1, 1, 1);
  g.addEdge(b, d, 1, 1, 1);
  g.addEdge(c, d, 1, 1, 1);

  wasEntireGraphWasVisited(&g, false);

  int num_connected_components = bfs(&g);
  REQUIRE( num_connected_components == 1 );

  wasEntireGraphWasVisited(&g, true);
}

/**
 * Traversal on the following graph: 
 *      
 *      A ----- B
 *               
 *      C ----- D
 * 
 */
TEST_CASE("Simple BFS on graph with 2 connected components", "[bfs]") {
  Graph g;

	Vertex* a = g.addVertex("0x1");
  Vertex* b = g.addVertex("0x2");
  Vertex* c = g.addVertex("0x3");
  Vertex* d = g.addVertex("0x4");

  g.addEdge(a, b, 1, 1, 1);
  g.addEdge(c, d, 1, 1, 1);

  wasEntireGraphWasVisited(&g, false);

  int num_connected_components = bfs(&g);
  REQUIRE( num_connected_components == 2 );

  wasEntireGraphWasVisited(&g, true);
}

/**
 * Traversal on the following graph: 
 *      
 *      A    B
 *               
 *      C    D
 * 
 */
TEST_CASE("Simple BFS on graph with 4 connected components", "[bfs]") {
  Graph g;

	Vertex* a = g.addVertex("0x1");
  Vertex* b = g.addVertex("0x2");
  Vertex* c = g.addVertex("0x3");
  Vertex* d = g.addVertex("0x4");

  wasEntireGraphWasVisited(&g, false);

  int num_connected_components = bfs(&g);
  REQUIRE( num_connected_components == 4 );

  wasEntireGraphWasVisited(&g, true);
}

/**
 * Traversal on the following graph: 
 * 
 *        A               E
 *        | \           / |
 *        |  \         /  | 
 *        |   C ----- D   |
 *        |  /         \  |
 *        | /           \ |
 *        B               F
 * 
 */
TEST_CASE("BFS on complex graph 1", "[bfs]") {
  Graph g;

	Vertex* a = g.addVertex("0x1");
  Vertex* b = g.addVertex("0x2");
  Vertex* c = g.addVertex("0x3");
  Vertex* d = g.addVertex("0x4");
  Vertex* e = g.addVertex("0x5");
  Vertex* f = g.addVertex("0x6");

  g.addEdge(a, b, 1, 1, 1);
  g.addEdge(a, c, 1, 1, 1);
  g.addEdge(b, c, 1, 1, 1);
  g.addEdge(c, d, 1, 1, 1);
  g.addEdge(d, e, 1, 1, 1);
  g.addEdge(d, f, 1, 1, 1);
  g.addEdge(e, f, 1, 1, 1);

  wasEntireGraphWasVisited(&g, false);

  int num_connected_components = bfs(&g);
  REQUIRE( num_connected_components == 1 );

  wasEntireGraphWasVisited(&g, true);
}

/**
 * Traversal on the following graph:
 * 
 *         E ---- A
 *         |      |
 *         |      |
 *         F      B
 *          \   /   \
 *           \ /     \
 *            C ----- D
 * 
 */
TEST_CASE("BFS on complex graph 2", "[bfs]") {
  Graph g;

  Vertex* a = g.addVertex("0x1");
  Vertex* b = g.addVertex("0x2");
  Vertex* c = g.addVertex("0x3");
  Vertex* d = g.addVertex("0x4");
  Vertex* e = g.addVertex("0x5");
  Vertex* f = g.addVertex("0x6");

  g.addEdge(a, b, 1, 1, 1);
  g.addEdge(a, e, 1, 1, 1);
  g.addEdge(e, f, 1, 1, 1);
  g.addEdge(f, c, 1, 1, 1);
  g.addEdge(b, c, 1, 1, 1);
  g.addEdge(b, d, 1, 1, 1);
  g.addEdge(d, c, 1, 1, 1);

  wasEntireGraphWasVisited(&g, false);

  int num_connected_components = bfs(&g);
  REQUIRE( num_connected_components == 1 );

  wasEntireGraphWasVisited(&g, true);
}

/**
 * Traversal on the following graph: 
 * 
 *     A ----- B ----- D
 *     |         ____/ | \____
 *     |        /      |      \
 *     C ----- E ----- G ----- F
 *                     |
 *                     H
 * 
 */
TEST_CASE("BFS on complex graph 3", "[bfs]") {
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

  wasEntireGraphWasVisited(&graph, false);

  int num_connected_components = bfs(&graph);
  REQUIRE( num_connected_components == 1 );

  wasEntireGraphWasVisited(&graph, true);
}

/**
 * Traversal on graph defined in data/10mb_data.csv.
 */
TEST_CASE("BFS on a large graph from a CSV file", "[bfs]") {
  // check if data exists
  string path = "data/10mb_data.csv";
  if (!ifstream(path).is_open()) 
    FAIL("Could not load file " + path + ". Make sure to run `make data` before running this test case.");

  Graph* graph = Graph::fromFile(path, true);

  wasEntireGraphWasVisited(graph, false);
  int num_connected_components = bfs(graph);
  wasEntireGraphWasVisited(graph, true);

  delete graph;
}

/**
 * Traversal on graph defined in data/30mb_data.csv. 
 */
TEST_CASE("BFS on a large graph from a CSV file 2", "[bfs]") {
  // check if data exists
  string path = "data/30mb_data.csv";
  if (!ifstream(path).is_open()) 
    FAIL("Could not load " + path + ". Make sure to run `make data` before running this test case.");

  Graph* graph = Graph::fromFile(path, true);

  wasEntireGraphWasVisited(graph, false);
  int num_connected_components = bfs(graph);
  wasEntireGraphWasVisited(graph, true);

  delete graph;
}
