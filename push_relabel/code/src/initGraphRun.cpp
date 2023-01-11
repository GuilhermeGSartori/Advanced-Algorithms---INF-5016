#include <iostream>
#include "../include/push_relabel.h"
#include "../include/util.h"
#include "../include/test.h"

// Parece que tem como colocar isso no make
#define OPERATIONAL

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
    std::cout << "graph will be read\n";
    read_graph(std::cin, n, m, s, t, solver);
    std::cout << "graph WAS read\n";
    // output = solver.runMaxFlow(); -> to do
    //check max flow and output -> nao termina se impossivel? ver isso, estudar mais algoritmo
    /*end of required modifications*/

    return 0;
}
#endif //OPERATIONAL


// pode ser generico, so pega testes de map e ja era, igual para todos projetos
#ifdef TEST
int main() {
    return 0;
}
#endif //TEST
