#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
  int rows;
  int cols;
  float **data;
} Matrix;

typedef struct {
  int num_matrices;
  Matrix *matrices;
  double *elapsed_times;
} MatrixArray;

Matrix *add_matrix(Matrix *matrices, int index, int rows, int cols);

Matrix add_matrices(Matrix a, Matrix b);

Matrix combine_matrices(Matrix a, Matrix b, Matrix c, Matrix d);

void free_matrix(Matrix *matrix);

void free_matrix_array(MatrixArray *matrix_array);

void free_multiple_matrices(int count, ...);

MatrixArray initialize_matrix_array();

Matrix subtract_matrices(Matrix a, Matrix b);

MatrixArray multiply_matrix_array(MatrixArray input,
                                  Matrix (*multiply_func)(Matrix, Matrix));

Matrix multiply_matrix_pair(Matrix a, Matrix b,
                            Matrix (*multiply_func)(Matrix, Matrix));

void subdivide_matrix(Matrix matrix, Matrix *a, Matrix *b, Matrix *c,
                      Matrix *d);

void free_strassen_matrices(Matrix *p1, Matrix *p2, Matrix *p3, Matrix *p4,
                            Matrix *p5, Matrix *p6, Matrix *p7, Matrix *c11,
                            Matrix *c12, Matrix *c21, Matrix *c22, Matrix *a11,
                            Matrix *a12, Matrix *a21, Matrix *a22, Matrix *b11,
                            Matrix *b12, Matrix *b21, Matrix *b22);

#endif // MATRIX_H
