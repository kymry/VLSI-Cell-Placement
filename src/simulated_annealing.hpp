//
//  simulated_annealing.hpp
//  AVLSI
//
//  Created by kymry burwell on 11/13/18.
//  Copyright © 2018 kymry burwell. All rights reserved.
//

#ifndef simulated_annealing_hpp
#define simulated_annealing_hpp

#include <stdio.h>
#include <vector>
#include <utility>

using std::vector; using std::pair;

// Generates cell coordinate list. Outputs coordinates of placement of all cells. <(x1,y1),(x2,y2)...(xn,yn)> 
vector<pair<int, int>> get_cell_coordinates(vector<vector<double>> &placement_matrix, int num_cells);

// Computes the initial solution cost vector
vector<double> comput_solution_cost_vector(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist);

// Peforms a cell swap and computes new cost
double try_swap1(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int b, double curr_cost, vector<double> &cost_vector);

// Peforms a cell swap and computes new cost (Does NOT the coordinate lit. DOES change the cost vector)
double try_swap(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int b, double curr_cost, vector<double> &cost_vector);

// Perform Swap (DOES update the coordinate list)
void swap(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int b,  vector<double> &cost_vector);

// Perform move (DOES update the coordinate list)
void move(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int index, vector<double> &cost_vector, vector<pair<int, int>> &open_spaces);

// Try a cell move
double try_move(vector<pair<int, int>> &coordinate_list, vector<pair<int, int>> &open_spaces, vector<vector<int>> &netlist, double curr_cost, vector<double> &cost_vector, int index, int a);

// Selects pair of cells
void select_pair(int size, int &a, int &b);

// Generate open chip location list (generate a list of all free chp locations that a cell can be placed in)
vector<pair<int, int>> get_open_spaces(vector<vector<double>> &chip);

// Perform simulated Annealing that returns total cost
int sim_anneal(vector<pair<int, int>> &coordinate_list, vector<double> &cost_vector, vector<vector<int>> &netlist, vector<vector<double>> &chip);

#endif /* simulated_annealing_hpp */
