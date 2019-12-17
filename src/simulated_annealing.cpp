//
//  simulated_annealing.cpp
//  AVLSI
//
//  Created by kymry burwell on 11/13/18.
//  Copyright © 2018 kymry burwell. All rights reserved.
//

#include "simulated_annealing.hpp"
#include "generate_graphs.hpp"
#include "generic_graph_operations.hpp"
#include <vector>
#include <utility>
#include <cmath>
#include <numeric>
#include <random>
#include <math.h>
#include <iostream>
#include <unordered_set>

using std::vector; using std::pair; using std::make_pair; using std::abs; using std::accumulate; using std::cout; using std::endl;
using std::unordered_set;

// Generates cell coordinate list. Outputs coordinates of placement of all cells. <(x1,y1),(x2,y2)...(xn,yn)>
vector<pair<int, int>> get_cell_coordinates(vector<vector<double>> &placement_matrix, int num_cells){
    
    // Data structure to hold list of cell coordinates (size is n+1 as 0th index isn't used)
    auto chip_size = placement_matrix.size();
    vector<pair<int, int>> coordinate_list(num_cells + 1, make_pair(0, 0));
    
    // Iterate over matrix and extract cell coordinates
    for (int i = 0; i < chip_size; ++i){
        for (int j = 0; j < chip_size; ++j){
            int curr_cell = placement_matrix[i][j];
            if (curr_cell != 0) { // If a cell is occupying chip location
                coordinate_list[curr_cell].first = i; // X-coordinate
                coordinate_list[curr_cell].second = j; // Y-coordinate
            }
        }
    }
    
    return coordinate_list;
}

// Computes the initial solution cost
vector<double> comput_solution_cost_vector(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist){
    
    // Data structure to hold the total cost of each cell (manhattan distance to each connected cells)
    vector<double> cell_cost(netlist.size());
    
    // Iterate over netlist and compute manhattan distance to all adjacent cells
    for (int i = 1; i < netlist.size(); ++i){
        int x = coordinate_list[i].first;
        int y = coordinate_list[i].second;
        
        for (auto j : netlist[i]){
            int x_p = coordinate_list[j].first;
            int y_p = coordinate_list[j].second;
            cell_cost[i] += abs(x - x_p);
            cell_cost[i] += abs(y - y_p);
        }
    }
    
    return cell_cost;
}

// Peforms a cell swap and computes new cost
double try_swap1(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int b, double curr_cost, vector<double> &cost_vector){
    
    // Make swap
    pair<int, int> temp_a = coordinate_list[a];
    coordinate_list[a] = coordinate_list[b];
    coordinate_list[b] = temp_a;
    
    // Hold new cost
    double cost = (curr_cost - cost_vector[a]) - cost_vector[b];
    double cost_a = 0, cost_b = 0;

    
    // For all adjacent cells of first cell to be swapped
    for (auto i : netlist[a]){
        int x = coordinate_list[i].first;
        int y = coordinate_list[i].second;
        cost_a += abs(x - coordinate_list[a].first);
        cost_a += abs(y - coordinate_list[a].second);
    }

    // For all adjacent cells of second cell to be swapped
    for (auto i : netlist[b]){
        int x = coordinate_list[i].first;
        int y = coordinate_list[i].second;
        cost_b += abs(x - coordinate_list[b].first);
        cost_b += abs(y - coordinate_list[b].second);
    }
    
    // Update cell costs
    cost_vector[a] = cost_a;
    cost_vector[b] = cost_b;
    
    // Update new cost
    cost = cost + cost_a + cost_b;
    
    return cost;
}

// Peforms a cell swap and computes new cost (Does NOT update the coordinate list. DOES change the cost vector)
double try_swap(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int b, double curr_cost, vector<double> &cost_vector){
    
    // Hold new costs
    double cost = curr_cost - cost_vector[a] - cost_vector[b];
    cost_vector[a] = 0;
    cost_vector[b] = 0;
    
    // For all adjacent cells of first cell to be swapped
    for (auto i : netlist[a]){
        int x = coordinate_list[i].first;
        int y = coordinate_list[i].second;
        cost_vector[a] += abs(x - coordinate_list[b].first);
        cost_vector[a] += abs(y - coordinate_list[b].second);
    }
    
    // For all adjacent cells of second cell to be swapped
    for (auto i : netlist[b]){
        int x = coordinate_list[i].first;
        int y = coordinate_list[i].second;
        cost_vector[b] += abs(x - coordinate_list[a].first);
        cost_vector[b] += abs(y - coordinate_list[a].second);
    }

    // Update new cost
    cost = cost + cost_vector[a] + cost_vector[b];
    
    return cost;
}

// Perform Swap (DOES update the coordinate list)
void swap(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int b, vector<double> &cost_vector){
    
    int ax_old = coordinate_list[a].first, ax_new = coordinate_list[b].first;
    int ay_old = coordinate_list[a].second, ay_new = coordinate_list[b].second;
    
    // Make swap
    pair<int, int> temp_a = coordinate_list[a];
    coordinate_list[a] = coordinate_list[b];
    coordinate_list[b] = temp_a;
    
    // Update cost vector for adjacent edges to a
    for (auto cell : netlist[a]){
        int old_cost = abs(ax_old - coordinate_list[cell].first) + abs(ay_old - coordinate_list[cell].second);
        int new_cost = abs(ax_new - coordinate_list[cell].first) + abs(ay_new - coordinate_list[cell].second);
        cost_vector[cell] += (new_cost - old_cost);
    }
    
    // Update cost vector for adjacent edges to b
    for (auto cell : netlist[b]){
        int new_cost = abs(ax_old - coordinate_list[cell].first) + abs(ay_old - coordinate_list[cell].second);
        int old_cost = abs(ax_new - coordinate_list[cell].first) + abs(ay_new - coordinate_list[cell].second);
        cost_vector[cell] += (new_cost - old_cost);
    }
    
}

// Perform move (DOES update the coordinate list)
void move(vector<pair<int, int>> &coordinate_list, vector<vector<int>> &netlist, int a, int index, vector<double> &cost_vector, vector<pair<int, int>> &open_spaces){
    
    int ax_old = coordinate_list[a].first, ax_new = open_spaces[index].first;
    int ay_old = coordinate_list[a].second, ay_new = open_spaces[index].second;
    
    // Make move
    pair<int, int> temp = coordinate_list[a];
    coordinate_list[a] = open_spaces[index];
    open_spaces[index] = temp;
    
    // Update cost vector for adjacent edges to a
    for (auto cell : netlist[a]){
        int old_cost = abs(ax_old - coordinate_list[cell].first) + abs(ay_old - coordinate_list[cell].second);
        int new_cost = abs(ax_new - coordinate_list[cell].first) + abs(ay_new - coordinate_list[cell].second);
        cost_vector[cell] += (new_cost - old_cost);
    }
    
}


// Try a cell move
double try_move(vector<pair<int, int>> &coordinate_list, vector<pair<int, int>> &open_spaces, vector<vector<int>> &netlist, double curr_cost, vector<double> &cost_vector, int index, int a){
    
    // Decrement old cost and set to 0
    curr_cost -= cost_vector[a];
    cost_vector[a] = 0;
    
    // For all adjacent cells of first cell to be swapped
    for (auto i : netlist[a]){
        int x = coordinate_list[i].first;
        int y = coordinate_list[i].second;
        cost_vector[a] += abs(x - open_spaces[index].first);
        cost_vector[a] += abs(y - open_spaces[index].second);
    }
    
    // Update total cost
    curr_cost += cost_vector[a];
    
    return curr_cost;
}

// Generate open chip location list (generate a list of all free chp locations that a cell can be placed in)
vector<pair<int, int>> get_open_spaces(vector<vector<double>> &chip){
    
    vector<pair<int, int>> open_spaces;
    
    for (int i = 0; i < chip.size(); ++i){
        for (int j = 0; j < chip.size(); ++j){
            if (chip[i][j] == 0){
                open_spaces.push_back(make_pair(i, j));
            }
        }
    }
    return open_spaces;
}

// Selects pair of cells uniformely at random
void select_pair(int size, int &a, int &b){
    
    // Get two items at random from uniform distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, size-1);
    a = dis(gen);
    b = dis(gen);
    
    // In case of repeated numbers
    while (a == b){
        b = dis(gen);
    }
    
}

// Perform simulated Annealing and returns total cost
int sim_anneal(vector<pair<int, int>> &coordinate_list, vector<double> &cost_vector, vector<vector<int>> &netlist, vector<vector<double>> &chip){
    
    // Random number generator used for accepting higher cost move
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    double T = 1000, epsilon = 0.01, alpha = 0.95; // Initial temperature and cut-off temperature, and reduction factor
    double trial_cost, delta_cost; // Iteration counter, rejection counter, and cost
    double curr_cost = accumulate(cost_vector.cbegin(), cost_vector.cend(), 0); // Cost of initial placement
    const double e = 2.71828182;
    vector<pair<int, int>> open_spaces = get_open_spaces(chip); // Available spaces to places cells on chip
    
    // Random number generator used for selecting index of cell move
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, open_spaces.size()-1);
    
    // Perform annealing while Temperature is above cut-off limit epsilon
    while (T > epsilon){
    
        int i = 0, reject = 0;
        // While stopping criterion is not met
        while (i < (coordinate_list.size()*4) && reject < coordinate_list.size()){
             i += 2;
            
            /***** Try swapping two cells *****/
            // Select random pair of cells
            int a, b;
            select_pair((int)coordinate_list.size(), a, b);
            
            // Try swapping cells (Save original cost in case swap is not accepted)
            double a_original_cost = cost_vector[a];
            double b_original_cost = cost_vector[b];
            trial_cost = try_swap(coordinate_list, netlist, a, b, curr_cost, cost_vector);
            delta_cost = trial_cost - curr_cost;
            
            // If cost improved, accept and make the swap
            if (delta_cost < 0){
                //curr_cost = trial_cost;
                swap(coordinate_list, netlist, a, b, cost_vector);
                curr_cost = accumulate(cost_vector.begin(), cost_vector.end(), 0);
            }
            // Otherwise make swap with some probability depending on temperature
            else {
                double r = distribution(generator);
                if (r < pow(e, -(delta_cost/T))) {
                    //curr_cost = trial_cost;
                    swap(coordinate_list, netlist, a, b, cost_vector);
                    curr_cost = accumulate(cost_vector.begin(), cost_vector.end(), 0);
                } else {
                    reject += 1;
                    cost_vector[a] = a_original_cost;
                    cost_vector[b] = b_original_cost;
                }
            }
            
            /***** Try moving of single cell to open location*****/
            // If there are open spaces, attempt a single cell move
            
            if (open_spaces.size() == 0)
                continue;
            
            // Select a new a (b will be unused)
            select_pair((int)coordinate_list.size(), a, b);
            int index = dis(gen);
            a_original_cost = cost_vector[a];
            trial_cost = try_move(coordinate_list, open_spaces, netlist, curr_cost, cost_vector, index, a);
            delta_cost = trial_cost - curr_cost;
            
            // If cost improved, accept and make the swap
            if (delta_cost < 0){
                move(coordinate_list, netlist, a, index, cost_vector, open_spaces);
                curr_cost = accumulate(cost_vector.begin(), cost_vector.end(), 0);
            }
            // Otherwise make swap with some probability depending on temperature
            else {
                double r = distribution(generator);
                if (r < pow(e, -(delta_cost/T))) {
                    //curr_cost = trial_cost;
                    move(coordinate_list, netlist, a, index, cost_vector, open_spaces);
                    curr_cost = accumulate(cost_vector.begin(), cost_vector.end(), 0);
                } else {
                    reject += 1;
                    cost_vector[a] = a_original_cost;
                }
            }
        }
        // Update the temperature
        T *= alpha;
    }
    return curr_cost;
}




















