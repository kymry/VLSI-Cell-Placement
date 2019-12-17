//
//  generic_graph_operations.cpp
//  AVLSI
//
//  Created by kymry burwell on 11/23/18.
//  Copyright © 2018 kymry burwell. All rights reserved.
//

#include "generic_graph_operations.hpp"
#include <vector>
#include <iostream>

using std::cout; using std::vector; using std::pair; using std::endl;

// Convert coordinate list to matrix and print to console (for visualization purposes)
void coordlist_to_matrix(vector<pair<int, int>> &coordinate_list, int size){
    
    vector<vector<int>> matrix(size, vector<int>(size, 0));
    
    for (int i = 1; i < coordinate_list.size(); ++i){
        matrix[coordinate_list[i].first][coordinate_list[i].second] = i;
    }
    
    // Print solution
    for (auto row : matrix){
        for (auto col : row){
            cout << col << " ";
        }
        cout << endl;
    }
}

// Prints the input matrix to standard output
void  print_matrix(vector<vector<double>> &matrix){
    
    for (auto &i: matrix){
        for (auto j : i){
            cout << j << " ";
        }
        cout << endl;
    }
}


// Prints the input adjacency list to standard output
void  print_adj_list(vector<vector<int>> &adj_list){
    
    cout << "----- Netlist -----" << endl; 
    
    for (int i = 1; i < adj_list.size(); ++i){
        cout << i << ": ";
        for (auto j : adj_list[i]){
            cout << j << " ";
        }
        cout << endl;
    }
    
    cout << endl;
}

// Prints the solution cost vector
void print_solution_cost_vector(vector<double> &cost_vector){
    
    // Begin at 1 because cell id's begin at 1 (index 0 is unused)
    for (int i = 1; i < cost_vector.size(); ++i){
        cout << "cost of " << i << " : " << cost_vector[i] << endl;
    }
    
}

// Converts adjacency matrix to adjacency list
vector<vector<int>> matrix_to_list(int num_cells,vector<vector<double>> &adj_matrix){
    
    // Data structure to hold adjacency list
    vector<vector<int>> adj(num_cells + 1);
    
    // Iterate over adjacency matrix and create adjacency list
    for (int i = 0; i < num_cells; ++i){
        for (int j = 0; j < num_cells; ++j){
            int curr_cell = adj_matrix[i][j];
            if (curr_cell != 0){
                adj[i+1].push_back(j+1); // +1 because the cell id naming convenction begins at 1
            }
        }
    }
    
    return adj;
}
