#pragma once
#include "core/vertex.h"
#include "core/graph.h"

/**
 * @brief Perform breadth first search upon all connected compontents within the
 * passed graph. 
 * 
 * @param g a Graph* to perform the search upon.
 * @return an int indicating the number of connected components in the graph.
 */
int bfs(Graph* g);

/** 
 * @brief Perform breadth first search upon all connected compontents within the
 * passed graph starting at the passed Vertex.
 * 
 * @param g a Graph* to perform the search upon.
 * @param start the Vertex* to begin the search from.
 */
int bfs(Graph* g, Vertex* start);

/**
 * @brief Perform breadth first search upon all connected compontents within the
 * passed graph and print out the CPU running time of the algorithm along with
 * sanity checks to indicate that the search visits all vertices and edges.
 * 
 * @param g a Graph* to perform the search upon.
 */
void run_bfs(Graph* g);

/**
 * @brief Find the largest connected component in the passed graph by running a
 * BFS traversal on each component in the graph. 
 * 
 * @param g a Graph* to perform the search upon.
 * @return a std::vector<Vertex*> containing the Vertex* that make up the 
 * largest connected component of the passed graph.
 */
std::vector<Vertex*> find_largest_component(Graph* g);

/**
 * @brief Find the largest connected component in the passed graph by running a
 * BFS traversal on each component in the graph. This function traverses a 
 * single connected component and returns a vector containing all the vertices
 * in the component. 
 * 
 * @param g a Graph* to perform the search upon.
 * @param start the Vertex* to begin the search from.
 * @return a std::vector<Vertex*> containing the Vertex* that make up the 
 * largest connected component of the passed graph.
 */
std::vector<Vertex*> find_largest_component(Graph* g, Vertex* start);
