#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include "utils/argument_parser.h"
#include "algorithms/betweenness.h"
#include "algorithms/dijkstras.h"
#include "algorithms/bfs.h"
#include "utils/utils.h"
#include "core/graph.h"

using std::priority_queue;
using std::unordered_map;
using std::vector;
using std::string;
using std::pair;
using std::sort;

int main(int argc, char* argv[]) {
  string dataset_filepath, cc_addresses_filepath, dijkstras_outfile;
  string dijkstra_start_vertex;
  bool should_run_bfs, should_run_dijkstra, verbose;
  int num_betweenness_to_print, num_threads;

  ArgumentParser ap;

  ap.add_argument("-f", true, &dataset_filepath, "The path to the dataset to load as a graph.");
  ap.add_argument("-b", false, &should_run_bfs, "Indicates whether or not to run BFS on the graph.");
  ap.add_argument("-d", false, &dijkstras_outfile, "The path to save the resulting shortest path distances resulting from running Dijkstra's algorithm from some vertex. Does nothing if not specified.");
  ap.add_argument("-s", false, &dijkstra_start_vertex, "Indicates the address of the address to perform Dijkstra's algorithm from.");
  ap.add_argument("-c", false, &num_betweenness_to_print, "Indicates the number of nodes to print the betweeness centrality of. Prints the BC of the k nodes with the largest BC.");
  ap.add_argument("-l", false, &cc_addresses_filepath, "The path to save addresses of the largest connected component. Does nothing if not specified.");
  ap.add_argument("-t", false, &num_threads, "The number of threads to use when computing betweenness centrality. Does nothing if not specified or if not running BC.");
  ap.add_argument("-v", false, &verbose, "Whether or not to print thread status when running code on multiple threads.");

  int arg_status = ap.parse(argc, argv);

  // error out if the arguments could not be parsed successfully
  if (arg_status == 1) return 1; 

  Graph* g = Graph::fromFile(dataset_filepath);

  if (g->getVertices().size() == 0) {
    std::cout << BOLDRED << "Could not load graph." << RESET << std::endl;
    return 1;
  }

  Graph* largest_connected_component = NULL;
  std::cout << std::endl;

  // Run BFS if the CLI argument to do so was given
  if (should_run_bfs) {
    run_bfs(g);
    std::cout << std::endl;
  }

  // Find the largest connected component if the CLI argument to do so was given
  if (cc_addresses_filepath != "") {
    vector<Vertex*> cc = find_largest_component(g);

    std::cout << YELLOW << "The largest connected component contains " 
              << cc.size() << " vertices." << RESET << std::endl;

    largest_connected_component = Graph::fromVertexList(cc);

    std::cout << YELLOW << "The new graph produced using the largest connected" 
              << " component consists of " 
              << largest_connected_component->getVertices().size() 
              << " vertices and "
              << largest_connected_component->getEdges().size() << " egdes."
              << RESET << std::endl;

    std::ofstream of;
    of.open(cc_addresses_filepath);
    of << ",from_address,to_address,truncated_value,gas,gas_price" << std::endl;

    for (Edge* e : largest_connected_component->getEdges()) {
      of << "," << *e << std::endl;
    }

    std::cout << GREEN << "Saved the largest connected component as a CSV to " 
              << cc_addresses_filepath << ".\n" << RESET << std::endl;

    of.close();
  }

  if (largest_connected_component != NULL) {
    delete largest_connected_component;
  }

  // Run Dijkstra's algorithm if the CLI argument to do so was given
  if (!dijkstras_outfile.empty()) {
    // try and look for the vertex passed in via the command line to start from  
    Vertex* start = g->getVertex(dijkstra_start_vertex);
    if (start == NULL) {
      if (!dijkstra_start_vertex.empty()) {
        std::cout << YELLOW << "Could not find vertex with address " 
                  << dijkstra_start_vertex << RESET << std::endl;
        
      }
      
      start = g->getVertices().begin()->second;
    } 

    std::cout << YELLOW << "Beginning Dijkstra's single source shortest path "
              << "algorithm from vertex with address " << start->getAddress()
              << "..." << RESET << std::endl;

    clock_t c1 = clock();
    dijkstra(g, start);
    clock_t c2 = clock();

    print_elapsed(c1, c2, "Dijkstra's algorithm");

    std::ofstream of;
    of.open(dijkstras_outfile);
    of << "address,incident_edges,distance" << std::endl;

    for (pair<string, Vertex*> v : g->getVertices()) {
      of << v.second->getAddress() << "," << v.second->getIncidentEdges().size()
         << "," << v.second->getDistance() << std::endl;
    }

    std::cout << GREEN << "Saved all shortest paths to " << dijkstras_outfile 
              << RESET << "\n" << std::endl;
  }

  // Compute betweenness centrality if the CLI argument to do so was given
  if (num_betweenness_to_print > 0) {
    std::cout << YELLOW << "Computing betweenness centrality on graph..." 
              << RESET << std::endl;
    
    clock_t c1 = clock();
    unordered_map<string, double> bc = 
      compute_betweenness_centrality(g, num_threads, verbose);
    clock_t c2 = clock();
    print_elapsed(c1, c2, "Brandes's betweenness centrality algorithm");

    vector<pair<string, double>> bc_heap;
    bc_heap.reserve(bc.size());

    for (pair<string, double> p : bc)
      bc_heap.push_back(p);
    sort(bc_heap.begin(), bc_heap.end(), compare_bc_pair);

    num_betweenness_to_print = 
      std::min(num_betweenness_to_print, static_cast<int>(bc_heap.size()));

    std::cout << YELLOW << "There are " << g->getPathCounter() 
              << " shortest paths in the graph.\n" << RESET << std::endl;

    for (int i = 0; i < num_betweenness_to_print; ++i) {
      Vertex* node = g->getVertex(bc_heap[i].first);
      std::cout << BLUE << "Betweenness Centrality of Address "
                << bc_heap[i].first << " (" << node->getIncidentEdges().size()
                << " incident edges): " << bc_heap[i].second << RESET 
                << std::endl;
    }

    std::ofstream of;
    string base_filepath = dataset_filepath.substr(0, dataset_filepath.size()-4);

    of.open(base_filepath +  + "_betweenness_centrality.csv");
    of << "address,incident_edges,betweenness_centrality" << std::endl;

    for (pair<string, double>& p : bc_heap) {
      Vertex* v = g->getVertex(p.first);
      of << v->getAddress() << "," << v->getIncidentEdges().size()
         << "," << p.second << std::endl;
    }
    
    of.close();

    std::cout << GREEN << "Saved all betweenness centrality of all vertices to "
              << base_filepath << "_betweenness_centrality.csv" << RESET << "\n" 
              << std::endl;

    std::cout << std::endl;
  }

  delete g;
}
