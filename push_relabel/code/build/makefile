CC = g++
CFLAGS = -Wall
SRC_DIR = ../src
INC_DIR = ../include


max_flow: $(SRC_DIR)/graph.o $(SRC_DIR)/push_relabel.o $(SRC_DIR)/test.o $(SRC_DIR)/util.o $(SRC_DIR)/initGraphRun.o
	  $(CC) -o max_flow util.o test.o graph.o initGraphRun.o push_relabel.o

$(SRC_DIR)/graph.o: $(SRC_DIR)/graph.cpp $(INC_DIR)/graph.h
		    $(CC) -c $(SRC_DIR)/graph.cpp

$(SRC_DIR)/push_relabel.o: $(SRC_DIR)/push_relabel.cpp $(INC_DIR)/push_relabel.h
		    $(CC) -c $(SRC_DIR)/push_relabel.cpp

$(SRC_DIR)/test.o: $(SRC_DIR)/test.cpp $(INC_DIR)/test.h
		    $(CC) -c $(SRC_DIR)/test.cpp

$(SRC_DIR)/util.o: $(SRC_DIR)/util.cpp $(INC_DIR)/util.h $(INC_DIR)/push_relabel.h
		    $(CC) -c $(SRC_DIR)/util.cpp

$(SRC_DIR)/initGraphRun.o: $(SRC_DIR)/initGraphRun.cpp $(INC_DIR)/util.h $(INC_DIR)/test.h
		    $(CC) -c $(SRC_DIR)/initGraphRun.cpp
