#ifndef get_eigenvectors_hpp
#define get_eigenvectors_hpp

#include <stdio.h>
#include <vector>

using std::vector;

// Compute second and third largest eigenvalues and return their eigenvectors
void get_largest_eigenvectors(vector<vector<double>> &input_graph, vector<double> &second_largest_evector, vector<double> &third_largest_evector);

#endif /* get_eigenvectors_hpp */
