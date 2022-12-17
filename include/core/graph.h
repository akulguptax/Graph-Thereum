#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <stack>

#include "vertex.h"
#include "edge.h"

/**
 * @brief A collection of edges and vertices. This class uses an adjacency list
 * graph implementation. Vertices are indexed by their associated blockchain 
 * address in the unordered map used to store all vertices. Edges are simply 
 * stored in a vector with arbitrary ordering.
 */
class Graph {
  public:
    /**
     * @brief Construct a new Graph object. The graph starts with no edges and 
     * no vertices.
     */
    Graph();

    /**
     * @brief Construct a new Graph object by making a deep copying of the 
     * passed Graph object. This deep copy includes making deep copies of all 
     * owned Edge and Vertex objects.
     * 
     * @param other a Graph to take a deep copy of.
     */
    Graph(const Graph& other);

    /**
     * Assignment operator for a Graph.
     *
     * @param rhs The right hand side of the assignment statement.
     * @return A reference for performing chained assignments.
     */
    const Graph& operator=(const Graph& rhs);

    /**
     * @brief Destroy the Graph object by deleting all of the associated Vertex
     * objects and Edge objects allocated on the heap.
     */
    ~Graph();

    /**
     * @brief Construct a new Edge object and adds it to the graph. Adds the 
     * edge to both the source and destination's incident edges list.
     * 
     * @param source a Vertex* indicating the source of the transaction.
     * @param destination a Vertex* indicating the destination of the 
     * transaction.
     * @param value a double indicating the value exchanged in the transaction 
     * in the trillions (multiply by 10^12 to get the actual value).
     * @param gas a uint64_t indicating the gas used in the transaction.
     * @param gas_price a uint64_t indicating the price of gas at the time of 
     * the transaction.
     * @return an Edge* pointing to the newly created Edge.
     */
    Edge* addEdge(Vertex* source, Vertex* destination, double value, 
                  uint64_t gas, uint64_t gas_price); 

    /**
     * @brief Construct a new Vertex object and adds it to the graph. By default 
     * Vertices are initialized with an empty list of indicent edges. 
     * 
     * @param address a string indicating the address of the individual that 
     * this Vertex represents. 
     * @return a Vertex* pointing to the newly created Vertex.
     */
    Vertex* addVertex(const std::string& address);

    /**
     * @brief Get the vertex associated with the passed address.
     * 
     * @param address a string indicating the blockchain address with which to 
     * search for the associated vertex in this graph.
     * @return a Vertex* representing the individual associated with the passed 
     * address. If the vertex does not exist in the graph, returns NULL.
     */
    Vertex* getVertex(const std::string& address) const;

    /**
     * @brief Check if the graph contains a vertex associated with the passed 
     * address.
     * 
     * @param address a string indicating the blockchain address with which to 
     * search for the associated vertex in this graph.
     * @return true if this graph CONTAINS a vertex associated with the passed 
     * address.
     * @return false if this graph DOES NOT contain a vertex associated with the
     * passed address.
     */
    bool containsVertex(const std::string& address) const;

    /**
     * @brief Get all the vertices in this graph. Returns a const reference to 
     * the std::unordered_map<std::string, Vertex*> of vertices. 
     * 
     * @return a std::unordered_map<std::string, Vertex*> containing all the
     * vertices in this graph, indexed by the associated blockchain addresses.
     */
    const std::unordered_map<std::string, Vertex*>& getVertices() const;

    /**
     * @brief Get all the edges in this graph.
     * 
     * @return a std::vector<Edge*> containing all the edges in this graph, 
     * indexed arbitrarily.
     */
    const std::vector<Edge*>& getEdges() const;

    /**
     * @brief Push a Vertex* to the stack of vertices that are ordered by 
     * distance from some arbitrary start vertex after running Dijkstra's 
     * algorithm on this graph.
     * 
     * @param v a Vertex* to add to the stack.
     */
    void pushDistanceOrderedVertex(Vertex* v);

    /**
     * @brief Pop and return the Vertex* at the top of the stack of vertices 
     * that are ordered by distance from some arbitrary start vertex after 
     * running Dijkstra's algorithm on this graph.
     * 
     * @return the Vertex* at the top of the stack.
     */
    Vertex* popDistanceOrderedVertex();

    /**
     * @brief Reset and empty the stack of vertices that are ordered by distance 
     * from some arbitrary start vertex after running Dijkstra's algorithm on 
     * this graph. Note that this DOES NOT destruct the Vertex* in the stack.
     */
    void resetDistanceOrderedVertices();

    /**
     * @brief Checks whether or not the stack of vertices that are ordered by 
     * distance from some arbitrary start vertex after running Dijkstra's 
     * algorithm on this graph contains any vertices.
     * 
     * @return true if the stack contains any vertices.
     * @return false if the stack is empty.
     */
    bool hasDistanceOrderedVertices() const;

    /**
     * @brief Incremene the counter tracking the number of unique shortest paths 
     * in this graph.
     */
    void incrementPathCounter() { ++path_counter_; }
    
    /**
     * @brief Get the number of unique paths after having computed the number
     * while running Brandes' algorithm to compute betweenness centrality.
     * 
     * @return A uint64_t indicating the number of unique shortest paths in this 
     * graph.
     */
    uint64_t getPathCounter() { return path_counter_; }

    /**
     * @brief Constructs a Graph with all edges and vertices allocated on the 
     * heap by loading etherium transactions from a CSV file. CSV records are 
     * formatted as follows:
     * 
     * idx,from_address,to_address,truncated_value,gas,gas_price
     * 
     * @param path a string indicating the path to the CSV file to load.
     * @param suppress_print a bool indicating whether or not to suppress 
     * printing in this function.
     * @return Graph* a graph constructed from the data in the CSV file.
     */
    static Graph* fromFile(const std::string& path, bool suppress_print=false);

    /**
     * @brief Constructs a Graph with all of the vertices in the passed vector.
     * This function will find all of the edges that link all of the given 
     * vertices and disregard any edges that do not connect 2 vertices in the
     * given vector. This function will then make a new copy of each Vertex*
     * and Edge* contained in the given vector and add them to a new Graph*
     * allocated on the heap.
     * 
     * @param vertices a vector of Vertex* to add to the new graph.
     * @return Graph* a new Graph allocated on the heap.
     */
    static Graph* fromVertexList(const std::vector<Vertex*>& vertices);
  private:
    /**
     * @brief A collection of all the edges in this graph in an arbitrary order.
     */
    std::vector<Edge*> edges_;

    /**
     * @brief A collection of all the vertices in this graph ordered by their 
     * associated address in the etherium blockchain.
     */
    std::unordered_map<std::string, Vertex*> vertices_;

    /**
     * @brief A stack of vertices that are inherently ordered by distance from
     * some arbitrary start vertex after running Dijkstra's algorithm on this 
     * graph. This stack is used in the accumulation stage of Brandes' algorithm
     * to compute betweenness centrality.
     */
    std::stack<Vertex*> distance_ordered_vertices_;

    /**
     * @brief A uint64_t indicating the number of unique shortest paths in this 
     * graph.
     */
    uint64_t path_counter_;

    /**
     * @brief Delete all of the associated Vertex objects and Edge objects 
     * that are allocated on the heap.
     */
    void _delete();

    /**
     * @brief Create a deep copy of the passed Graph object and of all of the 
     * associated Vertex objects and Edge objects allocated on the heap.
     * 
     * @param other a Graph to take a deep copy of.
     */
    void _copy(const Graph& other);
};
