#include "algorithms/bfs.h"
#include "utils/utils.h"

#include <iostream>
#include <queue>

using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::cout;
using std::endl;

int bfs(Graph* g) {
  // Set all vertices as unexplored to start
  for (pair<string, Vertex*> p : g->getVertices()) {
    p.second->reset();
  }

  // Set all edges as unexplored to start
  for (Edge* e : g->getEdges()) {
    e->setExplored(false);
  }

  int num_connected_components = 0;

  // Run BFS on each connected component of the graph
  for (pair<string, Vertex*> p : g->getVertices()) {
    if (!p.second->wasExplored()) {
      bfs(g, p.second);
      ++num_connected_components;
    }
  }

  return num_connected_components;
}

int bfs(Graph* g, Vertex* start) {
  queue<Vertex*> q;
  start->setExplored(true);
  q.push(start);

  while (!q.empty()) {
    Vertex* v = q.front();
    q.pop();

    for (Edge* indicent : v->getIncidentEdges()) {
      indicent->setExplored(true);
      Vertex* adjacent = indicent->getAdjacentVertex(v);
      if (!adjacent->wasExplored()) {
        // set indicent as discovery edge
        adjacent->setExplored(true);
        q.push(adjacent);
      } /*else if (indicent->wasExplored()) {
        // set indicent as cross edge
      } */
    }
  }

  // this bfs traverses a single connected component 
  // return to keep this interface consistent
  return 1; 
}

void run_bfs(Graph* g) {
  //////////////////////////////////////////////////////////////////////////////
  ///         Ensure all vertices HAVE NOT been explored BEFORE BFS          ///
  //////////////////////////////////////////////////////////////////////////////
  bool vertices_explored = false;
  for (pair<string, Vertex*> p : g->getVertices()) {
    vertices_explored |= p.second->wasExplored();
  }

  if (!vertices_explored)
    cout << GREEN << "No vertices have been explored." << RESET << endl;
  else
    cout << BOLDRED << "One or more vertices has been explored." << RESET << endl;

  //////////////////////////////////////////////////////////////////////////////
  ///           Ensure all edges HAVE NOT been explored BEFORE BFS           ///
  //////////////////////////////////////////////////////////////////////////////
  bool edges_explored = false;
  for (Edge* e : g->getEdges()) {
    edges_explored |= e->wasExplored();
  }

  if (!edges_explored)
    cout << GREEN << "No edges have been explored." << RESET << endl;
  else
    cout << BOLDRED << "One or more edges has been explored." << RESET << endl;

  //////////////////////////////////////////////////////////////////////////////
  ///                            Run BFS on graph                            ///
  //////////////////////////////////////////////////////////////////////////////
  cout << "\nRunning BFS..." << endl;
  clock_t c1, c2;
  c1 = clock();
  int num_connected_components = bfs(g);
  c2 = clock();
  print_elapsed(c1, c2, "BFS");
  cout << "There are " << num_connected_components
       << " connected components in the graph.\n" << endl;

  //////////////////////////////////////////////////////////////////////////////
  ///            Ensure all vertices HAVE been explored AFTER BFS            ///
  //////////////////////////////////////////////////////////////////////////////
  vertices_explored = true;
  for (pair<string, Vertex*> p : g->getVertices()) {
    vertices_explored &= p.second->wasExplored();
  }

  if (vertices_explored)
    cout << GREEN << "All vertices have been explored." << RESET << endl;
  else
    cout << BOLDRED << "One or more vertices was not explored." << RESET << endl;

  //////////////////////////////////////////////////////////////////////////////
  ///             Ensure all edges HAVE been explored AFTER BFS              ///
  //////////////////////////////////////////////////////////////////////////////
  edges_explored = true;
  for (Edge* e : g->getEdges()) {
    edges_explored &= e->wasExplored();
  }

  if (edges_explored)
    cout << GREEN << "All edges have been explored." << RESET << endl;
  else
    cout << BOLDRED << "One or more edges was not explored." << RESET << endl;
}

vector<Vertex*> find_largest_component(Graph* g) {
  // Set all vertices as unexplored to start
  for (pair<string, Vertex*> p : g->getVertices()) {
    p.second->reset();
  }

  // Set all edges as unexplored to start
  for (Edge* e : g->getEdges()) {
    e->setExplored(false);
  }

  vector<Vertex*> largest_connected_component;

  // Run BFS on each connected component of the graph
  for (pair<string, Vertex*> p : g->getVertices()) {
    if (!p.second->wasExplored()) {
      vector<Vertex*> result = find_largest_component(g, p.second);

      if (result.size() > largest_connected_component.size()) {
        largest_connected_component = result;
      }
    }
  }

  // Set all vertices as unexplored to reset
  for (pair<string, Vertex*> p : g->getVertices()) {
    p.second->setExplored(false);
  }

  // Set all edges as unexplored to reset
  for (Edge* e : g->getEdges()) {
    e->setExplored(false);
  }

  return largest_connected_component;
}

vector<Vertex*> find_largest_component(Graph* g, Vertex* start) {
  queue<Vertex*> q;
  start->setExplored(true);
  q.push(start);

  vector<Vertex*> result;
  result.push_back(start);

  while (!q.empty()) {
    Vertex* v = q.front();
    q.pop();

    for (Edge* indicent : v->getIncidentEdges()) {
      indicent->setExplored(true);
      Vertex* adjacent = indicent->getAdjacentVertex(v);
      if (!adjacent->wasExplored()) {
        result.push_back(adjacent);
        adjacent->setExplored(true);
        q.push(adjacent);
      }
    }
  }

  return result;  
}
