CC = g++
CFLAGS = -Wall
SRC_DIR = ../src
INC_DIR = ../include


max_flow: $(SRC_DIR)/graph.o $(SRC_DIR)/push_relabel.o $(SRC_DIR)/test.o $(SRC_DIR)/util.o $(SRC_DIR)/initGraphRun.o
	  $(CC) -o max_flow $(SRC_DIR)/util.o $(SRC_DIR)/graph.o $(SRC_DIR)/initGraphRun.o $(SRC_DIR)/push_relabel.o $(SRC_DIR)/test.o

$(SRC_DIR)/graph.o: $(SRC_DIR)/graph.cpp $(INC_SRC)/graph.h
		    $(CC) -c $(SRC_DIR)/graph.cpp

$(SRC_DIR)/push_relabel.o: $(SRC_DIR)/push_relabel.cpp $(INC_SRC)/push_relabel.h
		    $(CC) -c $(SRC_DIR)/push_relabel.cpp

$(SRC_DIR)/test.o: $(SRC_DIR)/test.cpp $(INC_SRC)/test.h
		    $(CC) -c $(SRC_DIR)/test.cpp

$(SRC_DIR)/util.o: $(SRC_DIR)/util.cpp $(INC_SRC)/util.h $(INC_SRC)/push_relabel.h
		    $(CC) -c $(SRC_DIR)/util.cpp

$(SRC_DIR)/initGraphRun.o: $(SRC_DIR)/initGraphRun.cpp $(INC_SRC)/util.h $(INC_SRC)/test.h
		    $(CC) -c $(SRC_DIR)/initGraphRun.cpp
