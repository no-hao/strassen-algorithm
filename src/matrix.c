#include "../include/file_utils.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

MatrixArray initialize_matrix_array() {
  MatrixArray matrix_array;
  matrix_array.num_matrices = 0;
  matrix_array.matrices = NULL;
  return matrix_array;
}

Matrix *add_matrix(Matrix *matrices, int index, int rows, int cols) {
  matrices = realloc(matrices, (index + 1) * sizeof(Matrix));
  Matrix *current = &matrices[index];
  current->rows = rows;
  current->cols = cols;
  current->data = (float **)malloc(rows * sizeof(float *));
  for (int i = 0; i < rows; i++) {
    current->data[i] = (float *)malloc(cols * sizeof(float));
  }
  return matrices;
}

Matrix add_matrices(Matrix a, Matrix b) {
  if (a.rows != b.rows || a.cols != b.cols) {
    printf("Error: Matrices cannot be added (dimension mismatch).\n");
    exit(EXIT_FAILURE);
  }

  Matrix result;
  result.rows = a.rows;
  result.cols = a.cols;
  result.data = (float **)malloc(result.rows * sizeof(float *));
  for (int i = 0; i < result.rows; i++) {
    float *a_row = a.data[i];
    float *b_row = b.data[i];
    float *res_row = (float *)malloc(result.cols * sizeof(float));
    result.data[i] = res_row;

    for (int j = 0; j < result.cols; j++) {
      *(res_row + j) = *(a_row + j) + *(b_row + j);
    }
  }

  return result;
}

Matrix subtract_matrices(Matrix a, Matrix b) {
  if (a.rows != b.rows || a.cols != b.cols) {
    printf("Error: Matrices cannot be subtracted (dimension mismatch).\n");
    exit(EXIT_FAILURE);
  }

  Matrix result;
  result.rows = a.rows;
  result.cols = a.cols;
  result.data = (float **)malloc(result.rows * sizeof(float *));
  for (int i = 0; i < result.rows; i++) {
    float *a_row = a.data[i];
    float *b_row = b.data[i];
    float *res_row = (float *)malloc(result.cols * sizeof(float));
    result.data[i] = res_row;

    for (int j = 0; j < result.cols; j++) {
      *(res_row + j) = *(a_row + j) - *(b_row + j);
    }
  }

  return result;
}

// Function to multiply a pair of matrices using the specified function
Matrix multiply_matrix_pair(Matrix a, Matrix b,
                            MultiplyFunction multiply_function) {
  return multiply_function.multiply_func(a, b, multiply_function.extra_data);
}

// Function to multiply matrices in pairs inside a MatrixArray using the
// specified function
MatrixArray multiply_matrix_array(MatrixArray input,
                                  MultiplyFunction multiply_function) {
  if (input.num_matrices % 2 != 0) {
    printf(
        "Error: odd number of matrices in input. Cannot multiply in pairs.\n");
    exit(EXIT_FAILURE);
  }

  MatrixArray output;
  output.num_matrices = input.num_matrices / 2;
  output.matrices = (Matrix *)malloc(output.num_matrices * sizeof(Matrix));
  output.elapsed_times = (double *)malloc(output.num_matrices * sizeof(double));

  for (int i = 0; i < input.num_matrices; i += 2) {
    clock_t start = clock();
    output.matrices[i / 2] = multiply_matrix_pair(
        input.matrices[i], input.matrices[i + 1], multiply_function);
    clock_t end = clock();
    output.elapsed_times[i / 2] = (double)(end - start) / CLOCKS_PER_SEC;
  }

  return output;
}

void subdivide_matrix(Matrix matrix, Matrix *a, Matrix *b, Matrix *c,
                      Matrix *d) {
  int half_rows = matrix.rows / 2;
  int half_cols = matrix.cols / 2;

  *a = (Matrix){half_rows, half_cols, NULL};
  *b = (Matrix){half_rows, half_cols, NULL};
  *c = (Matrix){half_rows, half_cols, NULL};
  *d = (Matrix){half_rows, half_cols, NULL};

  a->data = (float **)malloc(half_rows * sizeof(float *));
  b->data = (float **)malloc(half_rows * sizeof(float *));
  c->data = (float **)malloc(half_rows * sizeof(float *));
  d->data = (float **)malloc(half_rows * sizeof(float *));

  for (int i = 0; i < half_rows; i++) {
    a->data[i] = (float *)malloc(half_cols * sizeof(float));
    b->data[i] = (float *)malloc(half_cols * sizeof(float));
    c->data[i] = (float *)malloc(half_cols * sizeof(float));
    d->data[i] = (float *)malloc(half_cols * sizeof(float));

    for (int j = 0; j < half_cols; j++) {
      a->data[i][j] = matrix.data[i][j];
      b->data[i][j] = matrix.data[i][j + half_cols];
      c->data[i][j] = matrix.data[i + half_rows][j];
      d->data[i][j] = matrix.data[i + half_rows][j + half_cols];
    }
  }
}

Matrix combine_matrices(Matrix a, Matrix b, Matrix c, Matrix d) {
  if (a.rows != b.rows || a.rows != c.rows || a.rows != d.rows ||
      a.cols != b.cols || a.cols != c.cols || a.cols != d.cols) {
    printf("Error: Matrices cannot be combined (dimension mismatch).\n");
    exit(EXIT_FAILURE);
  }

  Matrix result;
  result.rows = a.rows * 2;
  result.cols = a.cols * 2;
  result.data = (float **)malloc(result.rows * sizeof(float *));

  int second_half_offset = a.cols;

  for (int i = 0; i < result.rows; i++) {
    float *res_row = (float *)malloc(result.cols * sizeof(float));
    result.data[i] = res_row;

    float *src_row = (i < a.rows) ? a.data[i] : c.data[i - a.rows];

    for (int j = 0; j < a.cols; j++) {
      *(res_row + j) = *(src_row + j);
    }

    src_row = (i < b.rows) ? b.data[i] : d.data[i - b.rows];

    for (int j = 0; j < b.cols; j++) {
      *(res_row + j + second_half_offset) = *(src_row + j);
    }
  }

  return result;
}

MatrixArray create_empty_matrix_array() {
  MatrixArray empty_array;
  empty_array.num_matrices = 0;
  empty_array.matrices = NULL;
  return empty_array;
}

void free_matrix(Matrix *matrix) {
  if (matrix->data) {
    for (int i = 0; i < matrix->rows; i++) {
      if (matrix->data[i]) {
        free(matrix->data[i]);
      }
    }
    free(matrix->data);
  }
  matrix->data = NULL;
  matrix->rows = 0;
  matrix->cols = 0;
}

void free_matrix_array(MatrixArray *matrix_array) {
  if (matrix_array->matrices) {
    for (int m = 0; m < matrix_array->num_matrices; m++) {
      Matrix *matrix = &matrix_array->matrices[m];
      if (matrix->data) {
        for (int i = 0; i < matrix->rows; i++) {
          if (matrix->data[i]) {
            free(matrix->data[i]);
          }
        }
        free(matrix->data);
      }
    }
    free(matrix_array->matrices);
    matrix_array->matrices = NULL;
  }
  matrix_array->num_matrices = 0;
}

void free_strassen_matrices(Matrix *p1, Matrix *p2, Matrix *p3, Matrix *p4,
                            Matrix *p5, Matrix *p6, Matrix *p7, Matrix *c11,
                            Matrix *c12, Matrix *c21, Matrix *c22, Matrix *a11,
                            Matrix *a12, Matrix *a21, Matrix *a22, Matrix *b11,
                            Matrix *b12, Matrix *b21, Matrix *b22) {
  free_matrix(p1);
  free_matrix(p2);
  free_matrix(p3);
  free_matrix(p4);
  free_matrix(p5);
  free_matrix(p6);
  free_matrix(p7);
  free_matrix(c11);
  free_matrix(c12);
  free_matrix(c21);
  free_matrix(c22);
  free_matrix(a11);
  free_matrix(a12);
  free_matrix(a21);
  free_matrix(a22);
  free_matrix(b11);
  free_matrix(b12);
  free_matrix(b21);
  free_matrix(b22);
}
