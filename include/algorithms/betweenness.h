#pragma once

#include <iostream>
#include <future>
#include <map>

#include "core/vertex.h"
#include "core/graph.h"
#include "core/edge.h"

/**
 * @brief Run Brandes' algorithm to compute betweenness centrality upon the 
 * given graph. This function will run on a single thread if the number of 
 * threads specified is less than or equal to 1. The function will spawn 
 * num_threads threads otherwise. The verbose flag determines whether to print 
 * out the status of each thread of the function spawns after increments of 
 * work. If the function spawns multiple threads, the main thread will remain 
 * idle and wait for the other threads to complete their work. Then, the main
 * thread will join the results of each thread and return the final result.
 * 
 * If this function runs with a single thread, it will call Dijkstra's algorithm
 * as used in Brandes' algorithm once for each vertex in the graph sequentially.
 * If this function runs with multiple threads, each spawned thread will call
 * Dijkstra's algorithm on a subset of the set of vertices in the given graph.
 * The number of calls to Dijkstra's algorithm will be roughly equal among 
 * threads. In order to avoid data races when accessing and modifying the 
 * internal state of the graph, the function makes a copy of the graph for each
 * thread spawned. These graph copies are deleted after the thread using them 
 * finishes.
 * 
 * @param graph a Graph* to run Brandes' betweenness centrality algorithm on.
 * @return a std::unordered_map<std::string, double> relating blockchain 
 * addresses to the betweenness centrality of the vertex they represent in the
 * blockchain graph.
 */
std::unordered_map<std::string, double> 
compute_betweenness_centrality(
    Graph* graph, int num_threads=1, bool verbose=false);

/**
 * @brief Run Brandes' algorithm to compute betweenness centrality upon the 
 * given graph. This function will run the algorithm using a single thread.
 * It will call Dijkstra's algorithm, as used in Brandes' algorithm, once for 
 * each vertex in the graph. Between each call to Dijkstra's algorithm, the 
 * function will accumulate the results of computing shortest paths from some
 * vertex before resetting the graph state and continuing.
 * 
 * @param graph a Graph* to run Brandes' betweenness centrality algorithm on.
 * @return a std::unordered_map<std::string, double> relating blockchain 
 * addresses to the betweenness centrality of the vertex they represent in the
 * blockchain graph.
 */
std::unordered_map<std::string, double> 
compute_betweenness_centrality_sequential(Graph* graph);

/**
 * @brief Run Brandes' algorithm to compute betweenness centrality upon the 
 * given graph. This function will spawn num_threads threads. The verbose flag 
 * determines whether to print out the status of each thread of the function 
 * spawns after increments of work. The main thread will remain idle and wait 
 * for spawned other threads to complete their work. Then, the main thread will 
 * join the results of each thread and return the final result.
 * 
 * Each spawned thread runs the compute_betweenness_centrality_parallel_helper
 * function to run Dijkstra's algorithm and accumulate the results of the run.
 * Each spawned thread will call Dijkstra's algorithm on a subset of the set of 
 * vertices in the given graph. The number of calls to Dijkstra's algorithm will 
 * be roughly equal among threads. The subsets of vertices that each spawned 
 * thread operates upon are disjoint, so the result is equal to that when 
 * computing betweenness centrality sequentially. In order to avoid data races 
 * when accessing and modifying the internal state of the graph, the function 
 * makes a copy of the graph for each thread spawned. These graph copies are 
 * deleted after the thread using them finishes. 
 *
 * @param graph a Graph* to run Brandes' betweenness centrality algorithm on.
 * @return a std::unordered_map<std::string, double> relating blockchain 
 * addresses to the betweenness centrality of the vertex they represent in the
 * blockchain graph.
 */
std::unordered_map<std::string, double> 
compute_betweenness_centrality_parallel(
    Graph* graph, int num_threads=4, bool verbose=false);

/**
 * @brief This function will call Dijkstra's algorithm on a subset of the set of 
 * vertices in the given graph and accumulate the results. Once the function 
 * finishes executing, it will put the results in a promise that is extracted
 * in the thread that spawned a new thread and called this function.
 *
 * @param thread_result a promise that will be sent to the spawning thread once
 * this function finishes executing.
 * @param graph a Graph* which which to run betweenness centrality on by running
 * Dijkstra's algorithm on a subset of vertices. 
 * @param thread_index the index of the thread in which this function is running 
 * in relation to the other spawned threads. This field is used for splitting up
 * the vertices in the graph into disjoint sets. 
 * @param num_threads the number of threads that were spawned by the function 
 * that spawned and called this thread. This field is used for splitting up
 * the vertices in the graph into disjoint sets. 
 */
void compute_betweenness_centrality_parallel_helper(
    std::promise<std::unordered_map<std::string, double>> thread_result,
    Graph* graph, int thread_index, int num_threads, bool verbose);
