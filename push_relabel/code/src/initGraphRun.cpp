#include <iostream>
//#include "../include/test.h"
#include "../include/test_map.h"
//#include "../include/util.h"

// Parece que tem como colocar isso no make
#define TEST

void help() {}

#ifdef OPERATIONAL
int main(int argc, char** argv) {

    unsigned n, m;
    int output = 0;

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
    //std::cout << "graph will be read\n";
    read_graph(std::cin, n, m, s, t, solver);
    //std::cout << "graph WAS read\n";
    // output = solver.runMaxFlow(); -> to do
    //check max flow and output -> nao termina se impossivel? ver isso, estudar mais algoritmo
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
