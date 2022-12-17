#include "algorithms/betweenness.h"
#include "algorithms/dijkstras.h"
#include "utils/utils.h"
#include <vector>

using std::unordered_map;
using std::make_pair;
using std::advance;
using std::promise;
using std::future;
using std::vector;
using std::string;
using std::thread;
using std::pair;
using std::move;

std::unordered_map<std::string, double> compute_betweenness_centrality(
    Graph* graph, int num_threads, bool verbose) {
  if (num_threads <= 1) {
    return compute_betweenness_centrality_sequential(graph);
  } else {
    return compute_betweenness_centrality_parallel(graph, num_threads, verbose);
  }
}

unordered_map<string, double> compute_betweenness_centrality_sequential(Graph* graph) {
  unordered_map<string, double> betweenness_centrality;

  for (pair<string, Vertex*> vertexPair : graph->getVertices()) {
    betweenness_centrality.insert({vertexPair.second->getAddress(), 0.0});
  }
  
  for (pair<string, Vertex*> vertexPair : graph->getVertices()) {
    // single-shortest-path
    dijkstra(graph, vertexPair.second);

    // accumulation
    while (graph->hasDistanceOrderedVertices()) {
      Vertex* w = graph->popDistanceOrderedVertex();

      for (Vertex* v : w->getCentralityParents()) {
        if (w->getCentrality() == 0) continue;

        v->incrementDependency((v->getCentrality() / w->getCentrality()) * (1 + w->getDependency()));
      }

      // only add centrality to paths that do not start/end at the starting vertex
      if (w != vertexPair.second) {
        // divide by because its an undirected graph so each edge is counted
        // twice (since algo is for directed graphs)
        betweenness_centrality[w->getAddress()] += w->getDependency() / 2.0;
      }
    }
  }

  return betweenness_centrality;
}

unordered_map<string, double> compute_betweenness_centrality_parallel(
    Graph* graph, int num_threads, bool verbose) {
  vector<pair<thread, future<unordered_map<string, double>>>> thread_group;

  for (int thread_idx = 0; thread_idx < num_threads; ++thread_idx) {
    Graph* copied_graph = new Graph(*graph);
    promise<unordered_map<string, double>> thread_result;
    auto completable_future = thread_result.get_future();


    thread next_thread(&compute_betweenness_centrality_parallel_helper,
                       std::move(thread_result), copied_graph,
                       thread_idx, num_threads, verbose);

    thread_group.emplace_back(move(next_thread), move(completable_future));
  }

  unordered_map<string, double> betweenness_centrality;

  // Go through each thread, get the resulting confusion matrix, and aggregate
  for (pair<thread, future<unordered_map<string, double>>>& t : thread_group) {
    // Adapted from https://stackoverflow.com/a/57134334
    thread next_thread = std::move(t.first);
    future<unordered_map<string, double>> result = std::move(t.second);

    // retrieve result from promise
    unordered_map<string, double> partial_centrality = result.get();

    // Aggregate the results
    for (pair<string, double> p : partial_centrality) {
      betweenness_centrality[p.first] += p.second;
    }

    next_thread.join(); // Close the thread
  }

  return betweenness_centrality;
}

void compute_betweenness_centrality_parallel_helper(
    std::promise<std::unordered_map<std::string, double>> thread_result,
    Graph* graph, int thread_index, int num_threads, bool verbose) {
  unordered_map<string, double> betweenness_centrality;

  for (pair<string, Vertex*> vertexPair : graph->getVertices()) {
    betweenness_centrality.insert({vertexPair.second->getAddress(), 0.0});
  }

  auto it = graph->getVertices().begin();
  auto end = graph->getVertices().end();

  // increment the iterator to start at the offset this thread operates on
  int counter = 0;
  while (it != end && counter < thread_index) {
    ++it;
    ++counter;
  }

  int iteration = -1;

  while (it != end) {
    if (verbose && ++iteration % 100 == 0) {
      std::cout << YELLOW << "Thread " << thread_index << " at iteration "
                << iteration << "." << RESET << std::endl;
    }

    // single-shortest-path
    dijkstra(graph, it->second);

    // accumulation
    while (graph->hasDistanceOrderedVertices()) {
      Vertex* w = graph->popDistanceOrderedVertex();

      for (Vertex* v : w->getCentralityParents()) {
        graph->incrementPathCounter();
        if (w->getCentrality() == 0) continue;

        v->incrementDependency((v->getCentrality() / w->getCentrality()) * (1 + w->getDependency()));
      }

      // only add centrality to paths that do not start/end at the starting vertex
      if (w != it->second) {
        // divide by because its an undirected graph so each edge is counted
        // twice (since algo is for directed graphs)
        betweenness_centrality[w->getAddress()] += w->getDependency() / 2.0;
      }
    }

    // increment the iterator by the number of threads to avoid overlapping 
    // computation between multiple threads
    counter = 0;
    while (it != end && counter < num_threads) {
      ++it;
      ++counter;
    }
  }

  delete graph;
  thread_result.set_value(betweenness_centrality);
}
