#pragma once
#include <string>
#include <vector>
#include "edge.h"

/**
 * @brief Predeclare the Edge class to resolve the circular dependency issue
 * that arises out of our adjacency list graph implementation. The compiler 
 * needs to know the size of each type contained as members of the Vertex class. 
 * We are only using Edge*, so the exact size of Edge does not matter. This 
 * means we can predeclare the Edge class in the Vertex class header file and 
 * finish the implementation of the Edge class in its own respective header and 
 * implementation files. 
 */
class Edge;

/**
 * @brief This class represents an individual involved in a transaction. An 
 * individual is represented solely by their address on the blockchain and by 
 * the transactions they are involved in. Refer to the Edge class for more 
 * detail on how transactions are represented. 
 */
class Vertex {
  public:
    /**
     * @brief Construct a new Vertex object. By default Vertices are initialized 
     * with an empty list of indicent edges. 
     * 
     * @param address a string indicating the address of the individual that 
     * this Vertex represents. 
     */
    Vertex(const std::string& address);

    /**
     * @brief Adds a new edge to the list of incident edges. Assumed to be used 
     * safely (edge should border this vertex, and not already exist).
     * 
     * @param e an Edge* to be added as an incident edge to this vertex.
     */
    void addEdge(Edge* e);

    /**
     * @brief Gets all incident edges to this vertex. Each indicent edge 
     * represents a transaction that the person represented by this Vertex was
     * involved in.
     * 
     * @return const std::vector<Edge*>& the edges indicident to this Vertex.
     */
    const std::vector<Edge*>& getIncidentEdges() const;

    /**
     * @brief Returns the blockchain address of the person represented by this 
     * Vertex.
     * 
     * @return a const std::string& indicating the address of the person that 
     * this Vertex represents.
     */
    const std::string& getAddress() const;

    /**
     * @brief Get whether or not this Vertex has been explored in some iteration 
     * of a graph algorithm. 
     * 
     * @return true if this Vertex HAS been explored in the current iteration of 
     * some graph algorithm.
     * @return false if this Vertex has NOT yet been explored in the current 
     * iteration of some graph algorithm.
     */
    bool wasExplored() const;

    /**
     * @brief Set the whether or not this Vertex has been explored in some 
     * iteration of a graph algorithm. 
     * 
     * @param explored a bool indicating the new exploration state of this 
     * Vertex.
     */
    void setExplored(bool explored);

    /**
     * @brief Set the distance from some other arbitary Vertex. Used in the 
     * implementation of Dijkstra's algorithm to find the shortest path between
     * two vertices. 
     * 
     * @param distance a uint64_t with which to update the distance.
     */
    void setDistance(uint64_t distance);

    /**
     * @brief Get the distance from some other arbitary Vertex. Used in the 
     * implementation of Dijkstra's algorithm to find the shortest path between
     * two vertices. 
     * 
     * @return a uint64_t used in the implementation of Dijkstra's algorithm to 
     * find the shortest path between two vertices.
     */
    uint64_t getDistance() const;

    /**
     * @brief Get the predecessor of this vertex on some shortest path between 2 
     * arbitrary vertices. This field is used when running Dijkstra's algorithm 
     * to find the single source shortest path from one vertex to all other 
     * vertices.
     * 
     * @return a Vertex* indicating the predecessor of this Vertex in some
     * shortest path between 2 arbitrary vertices.
     */
    Vertex* getParent() const;

    /**
     * @brief Set the predecessor of this vertex on some shortest path between 2 
     * arbitrary vertices. This field is used when running Dijkstra's algorithm 
     * to find the single source shortest path from one vertex to all other 
     * vertices.
     * 
     * @param parent a Vertex* indicating the predecessor of this Vertex in some
     * shortest path between 2 arbitrary vertices.
     */
    void setParent(Vertex* parent);

    /**
     * @brief Append a predecessor Vertex to the vector of Vertex* that stores 
     * the parents of this vertex in any number of shortest paths. This vector 
     * is used when computing betweenness centrality.
     * 
     * @param parent a Vertex* indicating the predecessor of this Vertex in some
     * shortest path between 2 arbitrary vertices.
     */
    void addCentralityParent(Vertex* parent);

    /**
     * @brief Get all the Vertex* that precede this Vertex in any number of 
     * shortest paths between two arbitrary vertices. This vector is used when 
     * computing betweenness centrality.
     * 
     * @return a const std::vector<Vertex*>& containing pointers to predecessor
     * vertices filled after computing shortest paths between some arbitrary 
     * vertex and all other vertices in the graph. 
     */
    const std::vector<Vertex*>& getCentralityParents() const;

    /**
     * @brief Increment the centrality score used when computing betweenness 
     * centrality by some amount. This value represents the number of shortest 
     * paths between 2 other arbitrary vertices that this vertex appears in.
     * 
     * @param amount a double indicating the number of shortest paths between 2 
     * other arbitrary vertices that this vertex appears in.
     */
    void incrementCentrality(double amount);

    /**
     * @brief Get the centrality score used when computing betweenness 
     * centrality. This value represents the number of shortest paths between 2 
     * other arbitrary vertices that this vertex appears in.
     * 
     * @return a double indicating the number of shortest paths between 2 
     * other arbitrary vertices that this vertex appears in.
     */
    double getCentrality() const;

    /**
     * @brief Increment the dependency score of this vertex. This dependency 
     * metric is used when computing betweenness centrality to represent the 
     * ratio of shortest paths between 2 other arbitrary vertices that this 
     * vertex lies on.
     * 
     * @param amount a double indicating the ratio of shortest paths between 2 
     * other arbitrary vertices that this vertex lies on with which to increment
     * this node's dependency score.
     */
    void incrementDependency(double amount);

    /**
     * @brief Get the dependency score of this vertex. This dependency metric is 
     * used when computing betweenness centrality to represent the ratio of 
     * shortest paths between 2 other arbitrary vertices that this vertex lies 
     * on.
     * 
     * @return a double indicating the ratio of shortest paths between 2 other 
     * arbitrary vertices that this vertex lies on.
     */
    double getDependency() const;

    /**
     * @brief Reset all internal state that is accessed and updated when running
     * various graph algorithms. This function is meant to be used at the start 
     * of graph algorithms when resetting the all vertex labels and state. This 
     * function DOES NOT clear this Vertex's collection of indicent edges or the 
     * blockchain address.
     */
    void reset();
  private: 
    /**
     * @brief The blockchain address of the person represented by this 
     * Vertex.
     */
    std::string address_;

    /**
     * @brief All of the edges that are indicident to this Vertex. Each indicent 
     * edge represents a transaction that the person represented by this Vertex 
     * was involved in.
     */
    std::vector<Edge*> incident_edges_;

    /**
     * @brief A bool state used by graph algorithms to track whether or not this 
     * Vertex has been explored in the current iteration of the algorithm. 
     */
    bool was_explored_;

    /**
     * @brief A uint64_t used in the implementation of Dijkstra's algorithm to 
     * find the shortest path between two vertices.
     */
    uint64_t distance_; 

    /**
     * @brief A Vertex* that stores the predecessor of this vertex in some 
     * shortest path between 2 arbitrary vertices. This field is used when 
     * running Dijkstra's algorithm to find the shortest path from one vertex
     * to all other vertices. 
     */
    Vertex* parent_;

    /**
     * @brief A vector of Vertex* that stores the parents of this vertex in any
     * number of shortest paths. This vector is used when computing betweenness
     * centrality.
     */
    std::vector<Vertex*> centrality_parents_;

    /**
     * @brief A double used when computing betweenness centrality. This value 
     * represents the number of shortest paths between 2 other arbitrary 
     * vertices that this vertex appears in.
     */
    double centrality_;

    /**
     * @brief A double used when computing betweenness centrality. This value 
     * represents the ratio of shortest paths between 2 other 
     * arbitrary vertices that this vertex lies on.
     */
    double dependency_;
};

/**
 * @brief This comparator is used to compare the distance values of two Vertex*. 
 * This comparator struct is used to construct a priority queue of Vertex* in
 * our implementation of Dijkstra's algorithm. 
 */
struct VertexPointerComparator {
  /**
   * @brief Compares the distance parameters of two Vertex objects. This
   * operator is used by the priority queue in the implementation of
   * Dijkstra's algorithm to find the shortest path between two vertices.
   *
   * @param arg1 one of the Vertex objects with which to compare distances with.
   * @param arg2 the other Vertex object with which to compare distances with.
   *
   * @return true if arg1 has a larger distance than arg2.
   * @return false if arg1 has a smaller distance than arg2.
   */
  bool operator() (Vertex* arg1, Vertex* arg2) {
    return (arg1->getDistance() > arg2->getDistance());
  }
};
