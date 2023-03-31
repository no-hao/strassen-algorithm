#include "../include/matrix.h"
#include <stdio.h>
#include <stdlib.h>

Matrix strassen_multiply(Matrix a, Matrix b) {
  // Check if matrices can be multiplied
  if (a.cols != b.rows) {
    printf("Error: Matrices cannot be multiplied (dimension mismatch).\n");
    exit(EXIT_FAILURE);
  }

  // Base case: if matrix dimensions are 1x1, return the product of the two
  // elements
  if (a.rows == 1 && a.cols == 1 && b.rows == 1 && b.cols == 1) {
    Matrix result = {1, 1, NULL};
    float **data = (float **)malloc(sizeof(float *));
    data[0] = (float *)malloc(sizeof(float));
    data[0][0] = a.data[0][0] * b.data[0][0];
    result.data = data;
    return result;
  }

  // Divide the input matrices into submatrices
  Matrix a11, a12, a21, a22, b11, b12, b21, b22;
  subdivide_matrix(a, &a11, &a12, &a21, &a22);
  subdivide_matrix(b, &b11, &b12, &b21, &b22);

  // Calculate seven products of submatrices using the Strassen algorithm
  Matrix p1 = strassen_multiply(a11, subtract_matrices(b12, b22));
  Matrix p2 = strassen_multiply(add_matrices(a11, a12), b22);
  Matrix p3 = strassen_multiply(add_matrices(a21, a22), b11);
  Matrix p4 = strassen_multiply(a22, subtract_matrices(b21, b11));
  Matrix p5 = strassen_multiply(add_matrices(a11, a22), add_matrices(b11, b22));
  Matrix p6 =
      strassen_multiply(subtract_matrices(a12, a22), add_matrices(b21, b22));
  Matrix p7 =
      strassen_multiply(subtract_matrices(a11, a21), add_matrices(b11, b12));

  // Calculate the four resulting submatrices
  Matrix c11 = add_matrices(subtract_matrices(add_matrices(p5, p4), p2), p6);
  Matrix c12 = add_matrices(p1, p2);
  Matrix c21 = add_matrices(p3, p4);
  Matrix c22 = add_matrices(subtract_matrices(add_matrices(p5, p1), p3), p7);

  // Combine the resulting submatrices into the final product matrix
  Matrix result = combine_matrices(c11, c12, c21, c22);

  // Free the allocated memory for intermediate matrices
  free_multiple_matrices(16, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &c11, &c12,
                         &c21, &c22, &a11, &a12, &a21, &a22, &b11, &b12, &b21,
                         &b22);

  return result;
}
