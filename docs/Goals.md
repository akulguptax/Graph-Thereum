# CS 225 Data Structures

# Final Project Proposal (neilk3-akulg3-amaanmk2-amantk2)

1. **Leading Question** <br /> With the rapidly growing market capitalizations of cryptocurrencies and the promise of Web3, decentralized applications, and the future of decentralized computation on blockchain networks, we seek to analyze and understand the pros and cons of the first popular Turing Complete blockchain - Ethereum and the Ethereum Virtual Machine. We will examine several facets of the network, particularly testing the decentralization narrative by identifying individuals that seem to be disproportionately central to the transactions passing through the network. We will examine the shape of the flow of currency through the system to understand how tightly connected any two individuals are by networks of payments. 

2. **Dataset Acquistion and Processing** <br /> Our publicly accessible dataset will be Ethereum Network Transactions gathered from [Kaggle](https://www.kaggle.com/bigquery/ethereum-blockchain?select=transactions). Based on this dataset we hope to learn whether the market is dominated by certain individuals and if so, determine the extent to which such individuals dominate the market. We also hope to be able to predict the gas required for certain transactions between two individuals. If such transactions have taken place before, we can simply return the historical data. The interesting information will be to predict gas prices for transactions between individuals who are linked in the network through mutual neighbors, but not by a single prior transaction. In graph terminology, the two individuals are not neighbors, but there is a path between them. The dataset consists of a very large list of historical transactions. Each transaction logs the address of the sender and receiver, the amount transferred in the transaction, and information about the gas price and gas amount required for the transaction. We intend to weight our graph using the gas required for each transaction. We plan to normalize the gas required for transactions using the price of gas as the price fluctuates for different transactions. 

3. **Graph Algorithms** <br /> The graph algorithms that we plan to implement include Betweenness Centrality and Dijkstra’s algorithm. We also plan to implement breadth first traversal. Note that m is the number of edges, and n is the number of vertices. <br />
* For Betweenness Centrality:<br />
  * Betweenness Centrality is an algorithm to measure how essential each node is within a network. It takes in any network, in our case a weighted network, and uses the shortest paths between all pairs of nodes to identify the nodes that occur most frequently in these shortest paths. The algorithm is based on Brandes' approximate algorithm. The implementation requires O(n + m) space and runs in O(nm + n^2 log(n)) time since our graph contains weighted edges. 
  * The inputs required would be the from_address and to_address, which represents the source node and the destination node, respectively and the receipt_cumalitive that represents the weight of an edge.
* Dijkstra’s Algorithm:
  * Dijkstra’s algorithm, given a source node, finds the shortest path to every node. We will be using this to find the shortest weighted path between two nodes. In terms of our dataset, this tells us how expensive it would be for person 1 (node 1) to transact with person 2 directly (node 2). 
  * The information we need from the dataset is from the ‘transactions’ dataset where we need the from_address, to_address, and receipt_cumalitive. The from and to address represents the 2 nodes, and the receipt_cumalitive represents the weight. That is, the total amount of gas used in the transaction.
  * The output will be a list of shortest paths to every node in the graph from a single start node, and the function will return only the shortest path to the second node that we are looking for (arbitrarily chosen). We will plot and do very basic statistical analysis on that created list. 
  * Runtime for Dijkstra’s algorithm is O(m*logn)<br />
 
* Breadth First Traversal: <br />
  * We will be implementing a breadth first traversal to find a certain node given a starting point. 
  * Our target runtime is O(m + n) since that is the runtime for BFS. This is because we have to look through every vertex and edge to find a specific node. 
  * The inputs required would be the from_address and to_address, which represents the source node and the destination node, respectively. 

4. **Timeline** <br /> Our timeline breakdown consists of four main benchmarks consisting of our Data work, Implementation of BFS, Dijkstra's Algorithm, and Betweenness Centrality. The breakdown of the benchmarks including expected dates is illustrated below. 

* A. Data & Setup (11/11) <br />
  * Data acquisition <br /> 
  * Data processing <br />
  * Setup Project Files/Structure <br />
* B. Traversal (BFS) (11/18) <br />
  * Implement BFS <br />
  * BFS Tests <br />
* C. Dijkstra’s (12/02) <br />
  * Implement Dijkstra’s Algorithm <br />
  * Dijkstra’s Algorithm Tests <br />
* D. Betweenness Centrality (12/09) <br />
  * Betweenness Centrality algorithm Implementation <br />
  * Tests for Betweenness Centrality <br />
