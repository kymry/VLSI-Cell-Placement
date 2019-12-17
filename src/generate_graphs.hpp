#ifndef generate_graphs_hpp
#define generate_graphs_hpp

#include <stdio.h>
#include <igraph.h>
#include <vector>
#include <iostream>

using std::vector; using std::string; using std::pair;

/*   Generate random graph
     type = 0: regular lattice graph
     type = 1: k-regular
     type = 2: geometric
     type = 3: Erdos Renyi
     size: size of graph (nxn) default is 4x4 = 16 nodes
     num_nodes: number of vertice in graph
     k_ize: k-regular parameter
     radius: radius in geometric graph
     p: probability of node connected in erdos renyi
 */
vector<vector<double>> create_random_graph(int type = 0, int size = 16, igraph_integer_t num_nodes = 0, igraph_integer_t k_size = 0, igraph_real_t radius = 0.2, igraph_real_t p = 0.5 );

// Prints the edgelist of a graph (to the console or a file)
void print_igraph_edgelist(igraph_vector_t *edge_list, string outpute_filename = "", bool flag = 1);

// Intake netlist from file (netlist must be in edge list format)
vector<vector<double>> read_edgelist(string inpute_filename, int num_nodes);

#endif /* generate_graphs_hpp */
