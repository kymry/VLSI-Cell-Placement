//
//  generic_graph_operations.hpp
//  AVLSI
//
//  Created by kymry burwell on 11/23/18.
//  Copyright © 2018 kymry burwell. All rights reserved.
//

#ifndef generic_graph_operations_hpp
#define generic_graph_operations_hpp

#include <stdio.h>
#include <vector>

using std::pair; using std::vector;

// Convert coordinate list to matrix and print to console (for visualization purposes)
void coordlist_to_matrix(vector<pair<int, int>> &coordinate_list, int size);

// Print matrix to console
void  print_matrix(vector<vector<double>> &matrix);

// Print adjacency list to console
void  print_adj_list(vector<vector<int>> &adj_list);

// Print solution cost vector
void print_solution_cost_vector(vector<double> &cost_vector);

// Converts adjacency matrix to adjacency list
vector<vector<int>> matrix_to_list(int num_cells,vector<vector<double>> &adj_matrix);

#endif /* generic_graph_operations_hpp */
