///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "ices_types.hpp"

namespace ices {
  // Solve the iceberg avoiding problem for the given grid, using an exhaustive
  // optimization algorithm.
  //
  // This algorithm is expected to run in exponential time, so the grid's
  // width+height must be small enough to fit in a 64-bit int; this is enforced
  // with an assertion.
  //
  // The grid must be non-empty.
  unsigned int iceberg_avoiding_exhaustive(const grid& setting) {
      
    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);

  // Compute the path length, and check that it is legal.
  const size_t steps = setting.rows() + setting.columns() - 2; // len
  assert(steps < 64);
  unsigned int count_paths = 0; // Counter for the legal paths
  char candidate[steps];
  int bit;
  int count = 0; // Path counter

  for(int i = 0; i <= pow(2.0, steps); i++){
    candidate[count] = 's'; // Start point
    count++;
    for (unsigned int j = 0; j <= steps -1; j++) {
      // Creating the candidate path
      bit = (i >> j) & 1;
      // Checks the bit location and validity
      if (bit == 1){
        candidate[count] = 'r';
      }
      else{
        candidate[count] = 'd';
      }
      count++;
    }
    int row = 0;
    int col = 0;
    bool valid = true;
    // Checks if the path to the right
    for(int k = 1; k < count; k++){
      if(candidate[k] == 'r'){
        if(setting.may_step(row, col+1) == true){
          col++;
        }
        else{
          valid = false;
          break;
        }
      }
      // Checks the path downward
      else if(candidate[k] == 'd'){
        if(setting.may_step(row+1, col) == true){
          row++;
        }
        else{
          valid = false;
          break;
        }
      }
    }
    // If the path si valid, increment count
    if(valid == true){
      count_paths++;
    }
    count = 0;
  }

    return count_paths;
  }

  // Solve the iceberg avoiding problem for the given grid, using a dynamic
  // programming algorithm.
  //
  // The grid must be non-empty.
  unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);

    const int DIM=100;
    std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));
    
    grid G(setting);

    A[0][0] = 1;

    int from_above;
    int from_left;
      
    
    // Loops through the rows and columns of the grid
    for(int i = 0; i < G.rows(); i++){
      for(int j = 0; j < G.columns(); j++){
        
        // Checks if its the inital position to prevent later code from setting it to zero
        if(i==0 && j==0){
          A[i][j] = 1;
        }

        // If there is an iceberg set A to zero
        else if (G.get(i,j) == 1){
          A[i][j] = 0;
        }

        else{
          from_above = from_left = 0;
          
          // Check if the above grid position has a path
          if(i>0 && A[i-1][j]!=0){
            from_above = A[i-1][j];
          }

          // Checks if the left grid position has a path
          if(j>0 && A[i][j-1]!=0){
            from_left = A[i][j-1];
          }

          // Set A to zero if no path from the left or above
          if(from_left == 0 && from_above == 0){
            A[i][j] = 0;
          }
          // Set A to however many paths there are from the left or above
          else{
            A[i][j] = from_above + from_left;
            
          }
        }
      }
    }

    return A[setting.rows()-1][setting.columns()-1];
  }

}
