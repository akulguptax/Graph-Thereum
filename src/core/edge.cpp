#include "core/edge.h"

using std::ostream;

Edge::Edge(Vertex* source, 
           Vertex* destination, 
           double value, 
           uint64_t gas, 
           uint64_t gas_price) 
  : source_(source), destination_(destination), value_(value), 
    gas_(gas), gas_price_(gas_price), was_explored_(false) {}

Vertex* Edge::getAdjacentVertex(Vertex* start) const {
  // If the starting vertex of this edge is one of the endpoints...
  // return the other endpont
  return start == source_ ? destination_ : source_;
}

Vertex* Edge::getSource() const { 
  return source_; 
}

Vertex* Edge::getDestination() const { 
  return destination_; 
}

double Edge::getValue() const { 
  return value_; 
}

uint64_t Edge::getGas() const { 
  return gas_; 
}

uint64_t Edge::getGasPrice() const { 
  return gas_price_; 
}

bool Edge::wasExplored() const { 
  return was_explored_; 
}

void Edge::setExplored(bool explored) { 
  was_explored_ = explored; 
}

ostream& operator<<(ostream& os, const Edge& e) {
  os << e.source_->getAddress() << "," << e.destination_->getAddress() << "," 
     << e.value_ << "," << e.gas_ << "," << e.gas_price_; 
  return os;
}
