# GraphThereum

Learn more about our project by [watching our walkthrough!](https://youtu.be/r1-JWJ-rE6Q)

### 1. Location of all major code, data, and results

* **Source Code**: All core code is located in the `/src` and `/include` folders, with the former containing the .cpp files and the latter containing .h. The `/tests` directory includes the test suites.
* **Data**: All data is located in the `/data` directory. This directory contains the Python Notebook used to actually query and download the dataset from BigQuery, as well as a few short subsets of the data, for simple testing purposes (mini_data.csv and tiny_data.csv). Because we did not push the data to GitHub, we set up a `make data` command that will automatically run all necessary scripts and freshly download the data in a new environment, that doesn't already have the data. 
* **Results**: Results can be found in the `/data` directory, labeled with the function from which they arose. 

### 2. Executable Instructions:
* `make all`: Run all of `make data`, `make project`, and `make test`.
* `make data`: Download the necessary data files from Azure Cloud.
* `make project`: Compile the source code and form the project executable.
* `make test`: Compile the source code and test code and form the test executable.

We have provided an interface for customizing how the executable is run using command line arguments. These arguments can be mixed and matched in any order and in any combination. Also note that the `-f` flag is the only required flag as it is used to specify the file to load the graph from. 
* `-f <filepath>`: Load a graph into memory using the CSV file at the given filepath. 
* `-b`: Perform a BFS on the graph and print the status of the search.
* `-l <filepath>`: Run a modified BFS on the graph to find the largest connected component and save the graph representing the largest connected component to the CSV file specified in the given path. If the file does not exist, it will be created. Otherwise, it will be overritten.
* `-d <filepath>`: Run Dijkstra's algorithm on the graph and save the results of the algorithm to the CSV file at the given filepath. If the file does not exist, it will be created. Otherwise, it will be overritten.
* `-s <string>`: If the user elected to run Dijkstra's algorithm, this parameter can be used to specifiy the address of the vertex to begin all shortest paths at. If the address does not map to a vertex in the graph or if this parameter is not specified, an arbitrary vertex will be selected as the start.
* `-c <int>`: Run Brandes' betweenness centrality (BC) algorithm and print out the BC of each of the `<int>` vertices with the highest BC. The BC of each vertex will be saved to a CSV file with `_betweenness_centrality.csv` appended to the path of the dataset that was loaded to create the graph in memory.
* `-t <int>`: If the user elected to run Brandes' algorithm, this parameter specifies the number of threads to use when running the algorithm. If the number specified is less than or equal to 1, the algorithm will run on a single thread. If the number is larger than 1, the algorithm will run on the number of threads specified. Note that due to measures taken to prevent data races, the amount of memory used is directly proportional to the number of threads as each thread receives its own copy of the graph loaded in memory at the start of the program. 
* `-v`: If the user elected to run Brandes' algorithm with multiple threads and this this argument is specified, the program will print out the status of each thread after chunks of work have been completed by each thread.

Here are some cool example variations of our executable we think you should run...
* `./project -f data/data.csv -b -l data/largest_cc.csv`: Load the large dataset from `data/data.csv`, perform a BFS to visit all vertices and edges, and find the largest connected component and save it to `data/largest_cc.csv`.
* `./project -f data/10mb_data.csv -c 10 -t 7`: Load the moderately sized dataset and compute the betweenness centrality of each node by distrubuting work between 15 threads before printing out the betweenness centrality of the 10 vertices with the largest centrality. Note that this takes a few minutes to run on EWS. You can pass `15` to the `-t` flag if you are running locally. 
* `./project -f data/data.csv -d data/dijkstra_outfile.csv -s 0x1c39ba39e4735cb65978d4db400ddd70a72dc750`: Run Dijkstra's algorithm on the large dataset from `data/data.csv` starting from the vertex associated with the address `0x1c39ba39e4735cb65978d4db400ddd70a72dc750` and save the resulting shortest path distances to `data/dijkstra_outfile.csv`.

If you do not specify the `-f` required command line argument, the program will exit after printing the following warning listing all of the required arguments:
```
Missing required argument -f: [STRING] The path to the dataset to load as a graph.
```

If you do not pass a value to string, integer, or double type CLI arguments, you will see a warning listing the argument and the description similar to this for each invalid argument: 
```
Invalid argument -d: [STRING] The path to save the resulting shortest path distances resulting from running Dijkstra's algorithm from some vertex. Does nothing if not specified.
```

You will see a similar warning to the one above if you pass an argument of a mismatching type, like a string to an integer argument. The program will immediately exit on either case. 

You will see a warning like the one below if you pass an argument in the command line that does not match any of the argument rules defined in the `ArgumentParser` declared in `main.cpp`. The only difference between this warning and the others above is that the program will not exit immediately when the program receives unknown flags as command line arguments. 
```
Unknown flag: -q
```
### 3. Test Instructions

* `make data` - note that this only needs to be run once as it simply downloads our datasets locally. 
*  `make test`
*  `./test` will run all tests. Our tests are tagged with one of `"[bfs]"`, `"[dijkstras]"`, and `"[betweenness]"`, so you can run the subset of the tests with `./test "[bfs]"` if you wish.

Tests Overview:
* BFS: The tests include simple and complex BFS on graphs with varying connected components, checking that the entire graph was visited.
* Dijkstras: The tests include verifying the short path on a simple graph as well as verifying the shortest path on graph with equal edge weights and multiple shortest paths. The tests verify both the correct path and the distance of the path.
* Betweeness Centrality: The tests include simple betweenness centrality on a graph with equal edge weights as well as on a graph with varying edge weights. 
