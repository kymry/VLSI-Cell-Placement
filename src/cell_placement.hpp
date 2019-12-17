//
//  cell_placement.hpp
//  AVLSI
//
//  Created by kymry burwell on 11/11/18.
//  Copyright © 2018 kymry burwell. All rights reserved.
//

#ifndef cell_placement_hpp
#define cell_placement_hpp

#include <stdio.h>
#include <vector>

using std::vector;

// Places cell in closest available grid location (only called when grid location is already occupied during initial cell placement)
void find_alternate_placement(vector<vector<double>> &chip, int x_fin, int y_fin, int cell_name, int chip_size);

// Constructs the initial cell placement of the netlist on the chip
vector<vector<double>> initial_placement(vector<double> &second_evector, vector<double> &third_evector, const int CHIP_SIZE);

#endif /* cell_placement_hpp */
