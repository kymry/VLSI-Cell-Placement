#include <iostream>
#include <fstream>
#include <igraph.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>
#include <vector>
#include "generate_graphs.hpp"

using std::vector; using std::cout; using std::endl; using std::string; using std::ofstream; using std::pair;
using std::ifstream;

// Print edge list
void print_igraph_edgelist(igraph_vector_t *edge_list, string outpute_filename, bool flag) {
    
    // Print to console
    if (flag){
        for (int i = 0; i < igraph_vector_size(edge_list); i+=2){
         cout  << VECTOR(*edge_list)[i] << " -- " << VECTOR(*edge_list)[i+1] << endl;
         }
    } else {
        // Print to file
        ofstream outStream(outpute_filename, ofstream::app);
        for (int i = 0; i < igraph_vector_size(edge_list); i+=2){
            outStream  << VECTOR(*edge_list)[i] << " -- " << VECTOR(*edge_list)[i+1] << endl;
        }
    }
}

// Generate random graph
vector<vector<double>> create_random_graph(int type, int size, igraph_integer_t num_nodes, igraph_integer_t k_size, igraph_real_t radius, igraph_real_t p) {
    
    // Instantiate graph objects
    igraph_t graph;
    igraph_vector_t dimvector;
    igraph_vector_t edge_list;
    
    // igraph_vector_t must be accessed by the VECTOR() function
    igraph_vector_init(&dimvector, 2); // 2 dimenions
    VECTOR(dimvector)[0] = size;
    VECTOR(dimvector)[1] = size;
    
    /*  Generate graph - type depends on user input
        type = 0 : square lattice
        type = 1 : k-regular (k_size = degree)
        type = 2 : geometric random graph (radius = connection factor for vertices)
        type = 3 : completely random graph using the erodos-renyi model*/
    if (type == 0){
        igraph_lattice(&graph, &dimvector, 0.1, IGRAPH_UNDIRECTED, 0, 0);
    }
    else if (type == 1){
        igraph_k_regular_game(&graph, num_nodes, k_size, IGRAPH_UNDIRECTED, 0);
    }
    else if (type == 2){
        igraph_grg_game(&graph, num_nodes, radius, 0, 0, 0);
    }
    else {
        igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNP, num_nodes, p, 0, 0);
    }
    
    // Get edge list
    const int num_edges = igraph_ecount(&graph);
    igraph_vector_init(&edge_list, 2*num_edges);
    igraph_get_edgelist(&graph, &edge_list, 0);
    
    // Remove graph from memory
    igraph_destroy(&graph);
    
    // Convert edgelist to standard c++ vector matrix for processing
    int num_vertices;
    if (type == 0){
        num_vertices = size*size;
    } else {
        num_vertices = num_nodes;
    }
    vector<vector<double>> output_graph(num_vertices, vector<double>(num_vertices));
    
    for (int i = 0; i < num_edges*2; i+=2){
        int v1 = VECTOR(edge_list)[i]; // first vertex in edge
        int v2 = VECTOR(edge_list)[i+1]; // second vertex in edge
        output_graph[v1][v2] = 1;
        output_graph[v2][v1] = 1;
    }
    
    return output_graph;
}

// Intake netlist from file (netlist must be in edge list format)
vector<vector<double>> read_edgelist(string inpute_filename, int num_nodes){
    
    // Initial matrix to hold input
    vector<vector<double>> adjacency_matrix(num_nodes, vector<double>(num_nodes));
    
    // Open stream connection
    ifstream input(inpute_filename);
    
    // Process all elements in file
    int a, b;
    while (input >> a >> b){
        adjacency_matrix[a][b] = 1;
        adjacency_matrix[b][a] = 1;
    }
    
    return adjacency_matrix;
}





