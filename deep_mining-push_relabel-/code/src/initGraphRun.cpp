#include <iostream>
#include <chrono>
#include <random>
//#include "../include/test.h"
//#include "../include/test_map.h"
#include "../include/util.h"


void help() {}

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

    std::chrono::system_clock::time_point t_start = std::chrono::system_clock::now();
    output = solver.getMaxFlow(s, t, max_flow_ops);
    std::pair<bool,bool> matrix[solver.getH()][solver.getW()];
    for(int i=0;i<solver.getH();i++) {
        for(int j=0;j<solver.getW();j++) {
	    matrix[i][j].first = false;
	    matrix[i][j].second = false;
	    std::cout << 0 << " ";
	}
	std::cout << "\n";
    }
    for(int i=0;i<solver.capturedCells_.size();i++) {
	std::pair<int,int> cell(solver.capturedCells_[solver.capturedCells_.size()-i-1].first,solver.capturedCells_[solver.capturedCells_.size()-i-1].second);
	matrix[cell.first][cell.second].first = true;
    }
    for(int i=solver.getH()-1;i>0;i--) { // should avoid first line
        for(int j=solver.getW()-1;j>-1;j--) {
            if(matrix[i][j].first and !matrix[i][j].second) {
		matrix[i-1][j].first = true;
		if(j == 0) {
		    matrix[i-1][j+1].first = true;
		}
		else if(j == solver.getW()-1) {
		    matrix[i-1][j-1].first = true;
		}
		else {
		    matrix[i-1][j-1].first = true;
		    matrix[i-1][j+1].first = true;
		}
	    }
        }
    }
    std::cout << "\n";
    for(int i=0;i<solver.getH();i++) {
        for(int j=0;j<solver.getW();j++) {
            if(matrix[i][j].first)
                std::cout << 1 << " ";
            else
                std::cout << 0 << " ";
        }
        std::cout << "\n";
    }
    auto run_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-t_start).count();

    time = static_cast<int>(run_time);
    std::cout << "Max Flow: " << output << "\n"; 
    std::cout << "Time: " << time << "\n";

    /*end of required modifications*/

    return 0;
}
