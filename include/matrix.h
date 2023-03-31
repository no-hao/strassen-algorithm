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

MatrixArray initialize_matrix_array();

int is_blank_line(const char *line, int len);

Matrix *add_matrix(Matrix *matrices, int index, int rows, int cols);

void add_line_to_matrix(Matrix *matrix, const char *line, int cols, int row);

int get_max_number_width(MatrixArray matrix_array);

void print_matrices(MatrixArray matrix_array);

void free_matrices(MatrixArray *matrix_array);

Matrix multiply_matrix_pair(Matrix a, Matrix b,
                            Matrix (*multiply_func)(Matrix, Matrix));

MatrixArray multiply_matrix_array(MatrixArray input,
                                  Matrix (*multiply_func)(Matrix, Matrix));

void print_elapsed_times(MatrixArray matrix_array);

#endif // MATRIX_H
