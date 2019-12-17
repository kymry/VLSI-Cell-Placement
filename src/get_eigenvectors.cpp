#include <Eigen/Core>
#include <GenEigsSolver.h>
#include <SymEigsSolver.h>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <igraph.h>
#include <unistd.h>
#include <sys/times.h>
#include <math.h>
#include <vector>
#include "get_eigenvectors.hpp"

using namespace Spectra; using std::cout; using std::vector;

// Compute second and third largest eigenvalues and return their eigenvectors
void get_largest_eigenvectors(vector<vector<double>> &input_graph, vector<double> &second_largest_evector, vector<double> &third_largest_evector){
    
     // Maps laplacian matrix to Eigen Matrix data type (square matrix)
    const int MATRIX_SIZE = static_cast<int>(input_graph.size());
    Eigen::MatrixXd eigen_matrix(MATRIX_SIZE,MATRIX_SIZE);
    
    for (auto i = 0; i != MATRIX_SIZE; ++i)
        for (auto j = 0; j != MATRIX_SIZE; ++j){
            eigen_matrix(i,j) = input_graph[i][j];
     }
    
    // Construct matrix operation object using the Eigen matrix wrapper class DenseSymMatProd
    DenseSymMatProd<double> op(eigen_matrix);
    
    /* Construct eigenvector solver object and return specified number of eigenvalues
            LARGEST_MAGN : retrieve eigenvalues with largest magnitude
            SMALLEST_MAGN : retrieve eignevalues with smallest magnitude
            eigs(&op, nev, nvc) : nev = number of eigenvalues to return. nvc = controls convegence speed (should be >= 2*nev) */
    SymEigsSolver< double, SMALLEST_MAGN, DenseSymMatProd<double> > eigs(&op, 3, 6);
    
    /* Initialize and compute eigenvalues - Returns number of converged eigenvalues
            init() : Initializes the solver by providing a random initial residual vector
            comput(maxit, Scalar, sort_rule) : maxit = max number of iterations, sort_rule = sorting rule for return eigenvalues */
    eigs.init();
    int nconv = eigs.compute();
    
    // Retrieve results
    Eigen::VectorXd evalues;
    Eigen::MatrixXd evectors;
    if(eigs.info() == SUCCESSFUL){
        evalues = eigs.eigenvalues(); // Returns a vector containing the eignevalues
        evectors = eigs.eigenvectors(); // Returns a matrix containing the eignevectors
    }
    
    // Convert results to C++ vector type
    for (int i = 0; i != MATRIX_SIZE; ++i){
        second_largest_evector[i] = evectors(i,1);
        third_largest_evector[i] = evectors(i,2);
    }
    
    //std::cout << "Eigenvalues found:\n" << evalues << std::endl;
    //std::cout << "Eigenvectors found:\n" << evectors << std::endl;
    //std::cout << std::endl;
}

