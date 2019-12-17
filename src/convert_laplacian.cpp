#include "convert_laplacian.hpp"
#include <vector>
#include <iostream>

using std::vector; using std::cout; using std:: endl;

// Convert graph to laplacian. Q = D - A
void compute_laplacian(vector<vector<double>> &random_graph){
    
    // For each row compute q = d - a
    for (int i = 0 ; i != static_cast<int>(random_graph.size()); ++i){
        
        // Reference to row
        vector<double> &row = random_graph[i];
        double degree = 0;
        
        // Calculate degree and invert weights
        for (double &col : row){
            degree += col;
            if (col != 0) {col = -col;}
        }
        
        // Set degree
        row[i] = degree;
    }
}




