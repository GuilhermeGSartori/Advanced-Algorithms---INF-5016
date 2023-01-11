#include <iostream>
#include "../include/push_relabel.h"
#include "../include/util.h"
#include "../include/test.h"

// Parece que tem como colocar isso no make
#define OPERATIONAL

void help() {}

bool validateInput(int argc, char argv**, std::istream& in) { // file pointer? or full file? open multiples?
    if(argc == 1) { // argv not required, input will be passed as cin with ' < '
	char starter_c = std::cin.peek();
        if(starter_c != 'c' and starter_c != 'p')
	    return false;
	else
	    return true;
    }
    return false;
}

#ifdef OPERATIONAL
int main(int argc, char argv**) {

    unsigned n, m;
    int output = 0;
    if(!validateInput(argc, argv, std::cin)) {
        help();
        return -1;
    }

    /* specific to problem modifications required*/
    /* - Input validation, operations, new variables, class name, graph reading function, solver run, output format*/
    Operations max_flow_ops;
    unsigned s, t;
    PushRelabel solver;
    read_graph(std::cin, n, m, s, t, solver);
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
