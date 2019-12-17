#ifndef convert_laplacian_hpp
#define convert_laplacian_hpp

#include <stdio.h>
#include <vector>

using std::vector;

// Convert graph to laplacian. Q = D - A
void compute_laplacian(vector<vector<double>> &random_graph);

#endif /* convert_laplacian_hpp */
