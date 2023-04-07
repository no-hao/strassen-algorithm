#include "../include/brute_force.h"
#include <stdio.h>
#include <stdlib.h>

// Function to multiply a pair of matrices
Matrix brute_force_algorithm(Matrix a, Matrix b, void *params) {
  (void)params;

  if (a.cols != b.rows) {
    printf("Error: incompatible matrices for multiplication.\n");
    exit(EXIT_FAILURE);
  }

  Matrix result;
  result.rows = a.rows;
  result.cols = b.cols;
  result.data = (float **)malloc(result.rows * sizeof(float *));

  for (int i = 0; i < result.rows; i++) {
    result.data[i] = (float *)malloc(result.cols * sizeof(float));
    for (int j = 0; j < result.cols; j++) {
      result.data[i][j] = 0;
      for (int k = 0; k < a.cols; k++) {
        result.data[i][j] += a.data[i][k] * b.data[k][j];
      }
    }
  }

  return result;
}
