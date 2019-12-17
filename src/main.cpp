#include <Eigen/Core>
#include <SymEigsSolver.h>  // Also includes <MatOp/DenseSymMatProd.h>
#include <iostream>
#include <Eigen/Dense>
#include <igraph.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>
#include <numeric>
#include <vector>
#include <chrono>
#include "generate_graphs.hpp"
#include "get_eigenvectors.hpp"
#include "convert_laplacian.hpp"
#include "cell_placement.hpp"
#include "simulated_annealing.hpp"
#include "generic_graph_operations.hpp"


using namespace Spectra; using std::accumulate; using std::vector; using std::cout; using std::endl;


int main() {
    
    // Set parameters
    const int NUM_NODES = 20; // Number of cells to be placed
    const int CHIP_SIZE = 5;
    
    // Generate random graph
    vector<vector<double>> netlist_graph = create_random_graph(2, 0, NUM_NODES, 8, 0.2, 0.5);
    
    // Convert adjacency matrix to adjacency list (the netlist)
    vector<vector<int>> netlist = matrix_to_list(NUM_NODES, netlist_graph);
    
    // Convert random graph to laplacian matrix
    compute_laplacian(netlist_graph);
    
    // Generate eignevalues and eigenvectors and return 2nd and 3rd smallest
    vector<double> s_evector(NUM_NODES), t_evector(NUM_NODES); // Holds 2nd and 3rd smallest eigenvectors
    
    //vector<vector<double>> laplacian_graph(NUM_NODES, vector<double>(NUM_NODES, 0));
    get_largest_eigenvectors(netlist_graph, s_evector, t_evector);
    
    // Print adjacency list
    cout << endl;
    print_adj_list(netlist);
    
    // Set initial placement on chip
    vector<vector<double>> chip = initial_placement(s_evector, t_evector, CHIP_SIZE);
    
    // Initial placement is provided as input
    vector<pair<int, int>> coordinate_list = get_cell_coordinates(chip, NUM_NODES);
    
    // Get solution cost vector
    vector<double> cost_vector = comput_solution_cost_vector(coordinate_list, netlist);
    cout << "Initial Cost: " << accumulate(cost_vector.cbegin(), cost_vector.cend(), 0) << endl;
    
    // Print original cell placement to console
    cout << "----- Initial Placement ------" << endl;
    coordlist_to_matrix(coordinate_list, CHIP_SIZE);
    cout << endl;
    
    // Perform simulated Annealing that returns total cost
    int cost = sim_anneal(coordinate_list, cost_vector, netlist, chip);
    
    // Get solution cost vector
    cout << "Final Cost: " << cost << endl;
    
    // Print final cell placement to console
    cout << "----- Final Placement ------" << endl;
    coordlist_to_matrix(coordinate_list, CHIP_SIZE);
    
}




