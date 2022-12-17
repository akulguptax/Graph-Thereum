#include "core/graph.h"
#include "utils/utils.h"

#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>

using std::unordered_set;
using std::unordered_map;
using std::stringstream;
using std::make_pair;
using std::ifstream;
using std::fstream;
using std::string;
using std::vector;
using std::stack;
using std::pair;

Graph::Graph() { }

Graph::~Graph() {
  _delete();
}

Graph::Graph(const Graph& other) {
  _copy(other);
}

const Graph& Graph::operator=(const Graph& rhs) {
  if (this != &rhs) {
    _delete();
    _copy(rhs);
  }

  return *this;
}

void Graph::_delete() {
  for (pair<string, Vertex*> v : vertices_) {
    delete v.second;
  }

  for (Edge* e : edges_) {
    delete e;
  }
}

void Graph::_copy(const Graph& other) {
  for (pair<string, Vertex*> v : other.getVertices()) {
    addVertex(v.second->getAddress());
  }

  for (Edge* e : other.getEdges()) {
    addEdge(
      getVertex(e->getSource()->getAddress()),
      getVertex(e->getDestination()->getAddress()),
      e->getValue(), e->getGas(), e->getGasPrice()
    );
  }
}

Edge* Graph::addEdge(Vertex* source, Vertex* destination, double value, 
                     uint64_t gas, uint64_t gas_price) {
  Edge* e = new Edge(source, destination, value, gas, gas_price);
  edges_.push_back(e);

  source->addEdge(e);
  destination->addEdge(e);

  return e;
}

Vertex* Graph::addVertex(const string& address) {
  Vertex* v = new Vertex(address);
  vertices_.insert(make_pair(address, v));

  return v;
}

Vertex* Graph::getVertex(const string& address) const {
  auto it = vertices_.find(address);
  return it == vertices_.end() ? NULL : it->second;
}

bool Graph::containsVertex(const string& address) const {
  return vertices_.find(address) != vertices_.end();
}

const unordered_map<string, Vertex*>& Graph::getVertices() const {
  return vertices_;
}

const vector<Edge*>& Graph::getEdges() const {
  return edges_;
}

Graph* Graph::fromFile(const string& path, bool suppress_print) {
  clock_t c1, c2;
  c1 = clock();
  
  if (!suppress_print) {
    std::cout << "Loading graph..." << std::endl;
  }
  
  Graph* g = new Graph();

  ifstream infile(path);
  string line;

  if (infile.is_open()) {
    getline(infile, line); // skip the first line containing column names

    while (getline(infile, line)) {
      stringstream comma_separated(line);
      string csv_element, to_address, from_address, value, gas, gas_price;

      getline(comma_separated, csv_element, ',');  // discard 0th index element
      getline(comma_separated, from_address, ','); // from_address
      getline(comma_separated, to_address, ',');   // to_address
      getline(comma_separated, value, ',');        // truncated_value
      getline(comma_separated, gas, ',');          // gas
      getline(comma_separated, gas_price, ',');    // gas_price

      Vertex* from = g->getVertex(from_address);
      if (from == NULL) from = g->addVertex(from_address);
      
      Vertex* to = g->getVertex(to_address);
      if (to == NULL) to = g->addVertex(to_address);

      Edge* transaction = g->addEdge(
        from, to, std::stod(value), std::stoull(gas), std::stoull(gas_price)
      );
    }
  }

  if (!suppress_print) {
    std::cout << "Loaded " << g->getEdges().size() << " edges and "
              << g->getVertices().size() << " vertices." << std::endl;

    c2 = clock();
    print_elapsed(c1, c2, "loading graph from csv");
  }

  return g;
}

Graph* Graph::fromVertexList(const vector<Vertex*>& vertices) {
  Graph* g = new Graph();
  
  for (Vertex* v : vertices) {
    // Add each edge in the list of vertices to the new graph
    g->addVertex(v->getAddress());

    // Mark each indicent edge of each vertex in provided graph as unvisited.
    for (Edge* e : v->getIncidentEdges()) {
      e->setExplored(false);
    }
  }

  for (Vertex* v : vertices) {
    for (Edge* e : v->getIncidentEdges()) {
      if (e->wasExplored()) continue;

      // mark edges we add as visited so we do not duplicate edges
      e->setExplored(true); 

      // Check if both the source and destination of the indicent edge are 
      // contained in the provided graph. If not, do not add the edge.
      Vertex* from = g->getVertex(e->getSource()->getAddress());
      if (from == NULL) continue;
      
      Vertex* to = g->getVertex(e->getDestination()->getAddress());
      if (to == NULL) continue;

      // Duplicate the edge and add to the new graph
      g->addEdge(from, to, e->getValue(), e->getGas(), e->getGasPrice());
    }
  }

  for (Vertex* v : vertices) {
    // Mark each indicent edge of each vertex in provided graph as unvisited.
    for (Edge* e : v->getIncidentEdges()) {
      e->setExplored(false);
    }
  }

  return g;
}

void Graph::pushDistanceOrderedVertex(Vertex* v) {
  distance_ordered_vertices_.push(v);
}

Vertex* Graph::popDistanceOrderedVertex() {
  Vertex* v = distance_ordered_vertices_.top();
  distance_ordered_vertices_.pop();
  return v;
}

void Graph::resetDistanceOrderedVertices() {
  distance_ordered_vertices_ = stack<Vertex*>();
}

bool Graph::hasDistanceOrderedVertices() const {
  return !distance_ordered_vertices_.empty();
}
