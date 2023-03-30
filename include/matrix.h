#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int rows;
  int cols;
  float **data;
} Matrix;

Matrix read_csv(const char *filename, int n);

void print_matrix(Matrix matrix);

void free_matrix(Matrix *m);

#endif // MATRIX_H
