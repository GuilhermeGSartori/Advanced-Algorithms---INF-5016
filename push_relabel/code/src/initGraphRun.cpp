#include <iostream>
#include <chrono>
#include <random>
//#include "../include/test.h"
#include "../include/test_map.h"
//#include "../include/util.h"

// Parece que tem como colocar isso no make
#define OPERATIONAL

void help() {}

#ifdef OPERATIONAL
int main(int argc, char** argv) {

    unsigned n, m;
    int output = 0;
    int time;
    std::random_device rd;
    std::mt19937 gen(rd());

    /* adapt expected argc for the problem */
    if(argc != 1) {
        help();
	std::cout << "bad input\n";
        return -1;
    }

    /* specific to problem modifications required*/
    /* - operations, new variables, class name, graph reading function, solver run, output format*/
    Operations max_flow_ops;
    unsigned s, t;
    PushRelabel solver;

    read_graph(std::cin, n, m, s, t, solver);

    //std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point t_start = std::chrono::system_clock::now();
    output = solver.getMaxFlow(s, t, max_flow_ops);
    //auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-t).count();
    auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-t_start).count();

    time = static_cast<int>(run_time);
    std::cout << "n = " << n << ", max flow: " << output;
    std::cout << ", time: " << time << ", pushes: " << max_flow_ops.pushes_;
    std::cout << ", relabel: " << max_flow_ops.relabels_ << "\n"; 

    /*end of required modifications*/

    return 0;
}
#endif //OPERATIONAL



#ifdef TEST
int main() {
    int test_number;
    PushRelabel solver;

    std::cout << "-------------------------------\n";
    std::cout << "Choose a test case to perform:\n\n";
    std::cout << "* 1 -> Graph Data Structure\n";
    std::cout << "* 2 -> Edges Data Verification\n";
    std::cout << "* 3 -> Update Nodes and Edges\n";
    std::cout << "-------------------------------\n";
 
    std::cin >> test_number; // loop for input?
    
    auto test_case = test_map.find(test_number);

    if(test_case != test_map.end()) {
        bool results = test_case->second(solver);
	if(results) {
	    std::cout << "Test passed!\n";
	}
	else
	    std::cout << "Test failed... :(\n";
    }
    else {
        std::cout << "Test case does not exist\n";
    }


    
    return 0;
}
#endif //TEST
