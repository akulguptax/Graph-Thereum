#include "core/vertex.h"

using std::string;
using std::vector;

Vertex::Vertex(const string& address)
  : address_(address), incident_edges_(), was_explored_(false),
    distance_(0xFFFFFFFFFFFFFFFF), parent_(NULL), centrality_parents_(),
    centrality_(0.0), dependency_(0.0) { }

void Vertex::addEdge(Edge* e) {
  incident_edges_.push_back(e);
}

const vector<Edge*>& Vertex::getIncidentEdges() const {
  return incident_edges_;
}

const string& Vertex::getAddress() const {
  return address_;
}

bool Vertex::wasExplored() const { 
  return was_explored_; 
}

void Vertex::setExplored(bool was_explored) { 
  was_explored_ = was_explored; 
}

void Vertex::setDistance(uint64_t distance) {
  distance_ = distance;
}

uint64_t Vertex::getDistance() const {
  return distance_;
};

Vertex* Vertex::getParent() const {
  return parent_;
}

void Vertex::setParent(Vertex* parent) {
  parent_ = parent; 
}

void Vertex::addCentralityParent(Vertex* parent) {
  centrality_parents_.push_back(parent);
}

void Vertex::incrementCentrality(double amount) {
  centrality_ += amount;
}

double Vertex::getCentrality() const {
  return centrality_;
}

void Vertex::incrementDependency(double amount) {
  dependency_ += amount;
}

double Vertex::getDependency() const {
  return dependency_;
}

const vector<Vertex*>& Vertex::getCentralityParents() const {
  return centrality_parents_;
}

void Vertex::reset() {
  dependency_ = 0.0;
  centrality_ = 0.0;
  parent_ = NULL;
  centrality_parents_.clear();
  was_explored_ = false;
  distance_ = 0xFFFFFFFFFFFFFFFF;
}
