
# GraphThereum

The goal of this project was to analyze the recent transactions on the Ethereum blockchain, to better understand the costs/inefficiencies of utilizing such a blockchain network, as well as to prove or disprove the concept of decentralization.

Began by accessing the most recent ~1.4 million transactions on the network as our dataset.

Our first basic algorithm was a basic Breadth First Search/Traversal. Through the execution of this algorithm, which took about 10.7 seconds to run on EWS, we found out that these 1.4 million transactions (more precisely 1365188 edges) occured among only 123496 addresses on the network (vertices). The fact that there were 10 times as many transactions as there were individuals involved in the transaction was neither particularly surprising nor concerning. It is reasonable to expect that an individual who uses the network would use it more than once to transact. Additionally, the number wasn't extremely high, which would be concerning from a decentralization perspective. 

Here is the terminal output from running the BFS:


![Alt text](img/bfs_proof.PNG?raw=true "Title")

However, admittedly, this simple average heuristic is limited in the amount it can inform us about the decentralization (or lack thereof) of this network. For that, we will use the betweeness centrality algorithm, whose results you will see later.

After that, ran dijkstra's algorithm in a rather interesting way. We identified the largest connected component and treated it as a graph in isolation. Chose an arbitrary start vertex and then used dijsktra's algorithm to calculate the minimum Gas Cost from every other vertex to that vertex. This serves to estimate the cost of a direct transaction between the fixed start address and every other person on this connected component of the blockchain. As visible from the two visualizations below, the vast majority of the estimated transactions were concentrated between about 1.6M and 1.9M units. This is highlighted in the standard histogram, as well as in the violin chart. 

With a fairly small standard deviation, we can be fairly confident that most direct transactions would cost about 1.75M Wei. 

![Alt text](img/dijkstra_visualization_histogram.PNG?raw=true "Estimated Gas Costs - Histogram")
![Alt text](img/dijkstra_visualization.PNG?raw=true "Estimated Gas Costs - Violin Chart")

Ran some basic statistics on the output file to understand the quantitative breakdown:

* Average - 1.753 M 
* StdDev - 0.169 M
* Median - 1.710 M
* Mode - 1.671 M
* Min - 0.021 M
* Max - 6.423 M


After all of this, arrived at arguably the most exciting and impactful part of this project. Again, for context, one of the core concepts, technically, economically, and even philosophically, of the blockchain movement is decentralization. Set out to use Betweenness Centrality to identify how truly decentralized the Ethereum blockchain network is. The first image provides proof of successfully running the Betweeness Centrality Algorithm based on the terminal output.

![Alt text](img/bc_proof.png?raw=true "Successful Run of Betweenness Centrality")

Another interesting visualization is the relationship between the betweeness centrality of an address on chain and the number of transactions it is involved in (the # of incident edges). 

![Alt text](img/betweenness_centrality_analysis.png?raw=true "Betweenness Centrality as a Function of Incident Edges")

In the above visualization, the color of each point represents the number of vertices falling at that point, where a more yellow color means more vertices are represented by that dot. You can see that the vast majority of vertices are then concentrated on the bottom left quadrant of the graph, with only a few transactions and a very low betweenness centrality (BC). As we move to the top right, we see darker points (representing fewer vertices) that are more sparse. In particular, the top right corner has two points comprised of vertices that have a BC that is about 6 orders in magnitude larger than the BC of the majority. That is sligthly concerning from a decentralization perspective, but not so dominant that it is a serious concern. 

Another useful, and even simpler, way of understanding the decentralization aspect is to view the BC as a percentage. What you notice in the following picture is that the BC percentages (centrality_pct, calculated as BC/total number of shortest paths) are all so close to 0 that they could not even be represented in c++ (python output displayed), meaning that no single actor or address on the blockchain plays a non-negligible role in the overall network - hooray for decentralization!


![Alt text](img/bc_proportion.png?raw=true "Betweenness Centrality Scores as Percentages, all Negligible")

Using these powerful graph algorithms, learned an immense amount both about the technical implementation of these algorithms and even more excitingly, about the Ethereum blockchain. It was very interesting to understand the transaction cost structure, as well as to validate that the network was indeed highly decentralized.
