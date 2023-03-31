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

void add_line_to_matrix(Matrix *matrix, const char *line, int cols, int row);

Matrix *add_matrix(Matrix *matrices, int index, int rows, int cols);

Matrix add_matrices(Matrix a, Matrix b);

Matrix combine_matrices(Matrix a, Matrix b, Matrix c, Matrix d);

void free_matrix(Matrix *matrix);

void free_matrix_array(MatrixArray *matrix_array);

void free_multiple_matrices(int count, ...);

int get_max_number_width(MatrixArray matrix_array);

MatrixArray initialize_matrix_array();

int is_blank_line(const char *line, int len);

MatrixArray multiply_matrix_array(MatrixArray input,
                                  Matrix (*multiply_func)(Matrix, Matrix));

Matrix multiply_matrix_pair(Matrix a, Matrix b,
                            Matrix (*multiply_func)(Matrix, Matrix));

void print_elapsed_times(MatrixArray matrix_array);

void print_matrices(MatrixArray matrix_array);

void subdivide_matrix(Matrix matrix, Matrix *a, Matrix *b, Matrix *c,
                      Matrix *d);

Matrix subtract_matrices(Matrix a, Matrix b);

#endif // MATRIX_H
