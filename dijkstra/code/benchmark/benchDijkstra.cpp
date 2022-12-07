#include "../include/dijkstra.h"
#include "../include/kHeap.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>



// bench results struct for one graph (40 runs will be executed for a single graph)
struct BenchResults {
	
	int n;
	int m;

	int dijkstraTime[40];
	int heapInserts[40];
	int heapUpdates[40];
	int heapDeletes[40];
};


void read_dimacs(std::istream& in, unsigned& n, unsigned& m, Graph& problemGraph) {
	
	std::string line="", dummy;
	while(line.substr(0,4) != "p sp")
		getline(in,line);

	std::stringstream linestr;
	linestr.str(line);
	linestr >> dummy >> dummy >> n >> m;
	problemGraph.setNumberOfNodes(n);
	problemGraph.allocateVector();
	unsigned i=0;
	while(i<m) {
		getline(in,line);
		if(line.substr(0,2) == "a ") {
			std::stringstream arc(line);
			unsigned u,v,w;
			char ac;
			arc >> ac >> u >> v >> w;
			problemGraph.addEdge(u-1, v-1, w);
			i++;
		}
	}
}


BenchResults benchIter(std::istream& in, int toTest) {

	BenchResults results;
	unsigned n, m;
	int k = 2, thisGraphRunNum = 0;
	std::random_device rd;
	std::mt19937 gen(rd()); // magic?
	Operations dummy;
	Graph problemGraph;
	
	read_dimacs(in, n, m, problemGraph);
	if(toTest) {
		Dijkstra dummySolver;
		if(803 == dummySolver.run(problemGraph, 0, 1, k, dummy))
			std::cout << "dijkstra test passed!\n";
	}

	
	std::uniform_int_distribution<int> distr(0, n-1);

	results.n = n; // nodes
	results.m = m; // arcs

	for(int i=0; i<4; i++) { // will test with 4 types of heap binHeap(i=0), triHeap(i=1), quadHeap(i=2), pentaHeap(i=3)
		for(int j=0; j<10; j++) { // 10 tests instances repeated on the same env (graph and k value) but changing the initial and target conditions
			Dijkstra solver;
			int init = distr(gen); // random number (0 to n-1)
			int target = distr(gen); //random number (0 to n-1)
			Operations ops;

			std::cout << "run number: " << thisGraphRunNum << " with inital node: " << init << " and targer node: " << target << "\n";
			// missing mem count capabilities
			std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
			solver.run(problemGraph, init, target, k, ops);
			auto runTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-t).count();
			
			results.dijkstraTime[thisGraphRunNum] = static_cast<int>(runTime);
			results.heapInserts[thisGraphRunNum] = ops.inserts;
			results.heapUpdates[thisGraphRunNum] = ops.updates;
			results.heapDeletes[thisGraphRunNum] = ops.deletes;
			
			thisGraphRunNum++;
		}
		k++;
	}

	return results; //graph is killed and the results are stored... 10*4
}

// saves results in a CSV file
void resultsToFile(std::ofstream& file, BenchResults results) {

	int k = 2;
	int run = 0;

	for(int i=0; i<4; i++) {
		for(int j=0; j<10; j++) {
			file << std::to_string(run + 1) << ", " << std::to_string(k) << ", " << std::to_string(results.n) << ", " << std::to_string(results.m) << ", ";
			file << std::to_string(results.dijkstraTime[run]) << ", " << std::to_string(results.heapInserts[run]) << ", ";
			file << std::to_string(results.heapUpdates[run]) << ", " << std::to_string(results.heapDeletes[run]) << ", \n";
			run++;
		}
		file << "MEDIA COM K = " << std::to_string(k) << ", \n";
		k++;
	}
}

int main() {

	BenchResults results;
	std::ofstream resultsFile;
	// change the name of the file and comment the iterations to execute one per process run.
	// TO-DO: create function that receives the graph you want to bench -> autoupdate file name and benchIter
	resultsFile.open("results/NY_bench_results.csv");
	
	std::ifstream NYGraph("graphs/USA-road-d.NY.gr"); // 264346 nodes - 733846 arcs
	std::ifstream COLGraph("graphs/USA-road-d.COL.gr"); // 435666 nodes - 1057066 arcs
	std::ifstream FLAGraph("graphs/USA-road-d.FLA.gr"); // 1070376 nodes - 2712798 arcs
	std::ifstream EGraph("graphs/USA-road-d.E.gr"); // 3598623 nodes - 8778114 arcs
	std::ifstream WGraph("graphs/USA-road-d.W.gr"); // 6262104 nodes - 15248146 arcs
	std::ifstream USAGraph("graphs/USA-road-d.USA.gr"); // 23947347 nodes - 58333344 arcs
							    // all these files need to be downloaded again since they are way to big for github

	resultsFile << "RUN, K, N, M, TIME(ms), INSERTS, UPDATES, DELETES, \n";
	
	std::cout << "\nNEW YORK\n\n";
	resultsFile << "NEW YORK with random initial and target nodes, \n";	
	results = benchIter(NYGraph, true);
	resultsToFile(resultsFile, results);

	/*std::cout << "\n\n\nCOLORADO\n\n";
	resultsFile << "COLORADO with random initial and target nodes, \n";
	results = benchIter(COLGraph, false);
	resultsToFile(resultsFile, results);*/

	/*std::cout << "\n\n\nFLORIDA\n\n";
	resultsFile << "FLORIDA with random initial and target nodes, \n";
	results = benchIter(FLAGraph, false);
	resultsToFile(resultsFile, results);*/

	/*std::cout << "\n\n\nEAST\n\n";
	resultsFile << "EAST USA with random initial and target nodes, \n";
	results = benchIter(EGraph, false);
	resultsToFile(resultsFile, results);*/
	
	/*std::cout << "\n\n\nWEST\n\n";
       	resultsFile << "WEST USA with random initial and target nodes, \n";	
	results = benchIter(WGraph, false);
	resultsToFile(resultsFile, results);*/

	/*std::cout << "\n\n\nUSA\n\n";
	resultsFile << "USA with random initial and tarfet nodes, \n";
	results = benchIter(USAGraph, false);
	resultsToFile(resultsFile, results);*/

	resultsFile.close();
	return 0;
}
