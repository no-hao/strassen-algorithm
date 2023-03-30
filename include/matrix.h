#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int rows;
  int cols;
  float **data;
} Matrix;

typedef struct {
  int count;
  Matrix *matrices;
} MatrixArray;

void add_line_to_matrix(Matrix *matrix, const char *line, int cols, int row);

MatrixArray multiply_matrix_array(MatrixArray input,
                                  Matrix (*multiply_func)(Matrix, Matrix));

Matrix multiply_matrix_pair(Matrix a, Matrix b,
                            Matrix (*multiply_func)(Matrix, Matrix));

Matrix *add_matrix(Matrix *matrices, int index, int rows, int cols);

MatrixArray read_csv(const char *filename, int n);

void free_matrices(MatrixArray *matrix_array);

void print_matrices(MatrixArray matrix_array);

int is_blank_line(const char *line, int len);

MatrixArray initialize_matrix_array();

#endif // MATRIX_H
