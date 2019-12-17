//
//  cell_placement.cpp
//  AVLSI
//
//  Created by kymry burwell on 11/11/18.
//  Copyright © 2018 kymry burwell. All rights reserved.
//

#include "cell_placement.hpp"
#include <iostream>
#include <vector>
#include <cmath>

using std::vector; using std::floor; using std::cout; using std::endl;

// Places cell in closest available grid location (only called when grid location is already occupied during initial cell placement)
void find_alternate_placement(vector<vector<double>> &chip, int x, int y, int cell_id, int chip_size){
    
    // Determine distance to grid edges
    int right = chip_size - x - 1;
    int up = chip_size - y - 1;
    int left = -x;
    int down = -y;
    
    // Iterate over all posibilities until open grip location is found (in counter-clockwise spiral increasing by 1)
    for (int i = 1; i <= chip_size; i++){
        
        // upper
        if (up >= i){
            for (int j = -i; j <= i; ++j){
                if (right >= j && left <= j && chip[y+i][x+j] == 0){
                        chip[y+i][x+j] = cell_id;
                        return;
                }
            }
        }
        // lower
        if (down <= -i){
            for (int j = -i; j <= i; ++j){
                if (right >= j && left <= j && chip[y-i][x+j] == 0) {
                    chip[y-i][x+j] = cell_id;
                    return;
                }
            }
        }
        // left
        if (left <= -i){
            for (int j = -i+1; j <= i-1; ++j){
                if (up >= j && down <= j && chip[y+j][x-i] == 0) {
                    chip[y+j][x-i] = cell_id;
                    return;
                }
            }
        }
        // right
        if (right >= i){
            for (int j = -i+1; j <= i-1; ++j){
                if (up >= j && down <= j && chip[y+j][x+i] == 0) {
                    chip[y+j][x+i] = cell_id;
                    return;
                }
            }
        }
    }
}

// Constructs the initial cell placement of the netlist on the chip
vector<vector<double>> initial_placement(vector<double> &second_evector, vector<double> &third_evector, const int CHIP_SIZE){
    
    // Get eigenvector sizes
    const int NETLIST_SIZE = static_cast<int>(second_evector.size());
    
    // Get middle and end x-coordinates of chip
    const int CHIP_MID = floor(CHIP_SIZE/2), CHIP_END = CHIP_SIZE;
    
    // Initialize the matrix that acts as the chip (NxN square)
    vector<vector<double>> chip(CHIP_SIZE, vector<double>(CHIP_SIZE, 0));
    
    // Place cells starting from arbitrary order (i.e. did not order eigenvectors prior to placement)
    for (int i = 0; i < NETLIST_SIZE; ++i){
        
        // Get initial cell coordinates
        double x_int = second_evector[i], y_int = third_evector[i];
        int x_fin, y_fin;
        
        // For x coordinate (if x_int is negative, place on left side of chip, else place on right side)
        if (x_int >= 0 && x_int <= 0.5) { x_fin = (int)floor(x_int * CHIP_END + CHIP_MID); }
        else if (x_int > 0.5) { x_fin = (int)floor(x_int * CHIP_END); }
        else { x_fin = (int)floor(-x_int * CHIP_MID); }
        
        // For y coordinate (if y_int is negative, place on left side of chip, else place on right side)
        if (y_int >= 0 && y_int <= 0.5) { y_fin = floor(y_int * CHIP_END + CHIP_MID); }
        else if (y_int >= 0.5) { y_fin = floor(y_int * CHIP_END); }
        else { y_fin = floor(-y_int * CHIP_MID); }
        
        //  Attempt to place cell (cell name is row number starting at 1)
        if (chip[y_fin][x_fin] == 0){
            chip[y_fin][x_fin] = i+1;
        } else {
            // If grid position already occupied, find alternate placement (i+1 for cell id )
            find_alternate_placement(chip, x_fin, y_fin, i+1, CHIP_SIZE);
        }
    }    
    return chip;
}

