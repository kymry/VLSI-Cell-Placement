
DIR= -I/usr/local/include -I/usr/local/Cellar/igraph/0.7.1_6/include/igraph \
	-I/usr/local/include/spectra-0.6.2/include \
	-I/usr/local/include/eigen-eigen-5a0156e40feb

LIBS= -llibigraph
LIB_PATH= -L/usr/local/Cellar/igraph/0.7.1_6/lib

OBJFILES= cell_placement.o convert_laplacian.o generate_graphs.o \
	generic_graph_operations.o get_eigenvectors.o simulated_annealing.o main.o

placement: $(OBJFILES)
	g++ -std=c++11 -Wall -O3 $(DIR) $(LIB_PATH) -o placement $(OBJFILES) $(LIBS)

cell_placement.o: cell_placement.cpp
	g++ -std=c++11 -Wall -O3 $(DIR) -c cell_placement.cpp

convert_laplacian.o: convert_laplacian.cpp
	g++ -std=c++11 -Wall -O3 $(DIR) -c convert_laplacian.cpp

generate_graphs.o: generate_graphs.cpp
	g++ -std=c++11 -Wall -O3 $(DIR) $(LIB_PATH) -c generate_graphs.cpp $(LIBS)

generic_graph_operations.o: generic_graph_operations.cpp
	g++ -std=c++11 -Wall -O3 $(DIR) -c generic_graph_operations.cpp

get_eigenvectors.o: get_eigenvectors.cpp
	g++ -std=c++11 -Wall -O3 $(DIR) -c get_eigenvectors.cpp

simulated_annealing.o: simulated_annealing.cpp
	g++ -std=c++11 -Wall -O3 $(DIR) -c simulated_annealing.cpp

main.o: main.cpp
	g++ -std=c++11 -Wall -O3 $(DIR) -c main.cpp
