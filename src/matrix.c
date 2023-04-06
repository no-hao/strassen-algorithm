#include "../include/file_utils.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void add_line_to_matrix(Matrix *matrix, const char *line, int cols, int row) {
  char *saveptr;
  char *token = strtok_r((char *)line, ",", &saveptr);
  int col = 0;
  while (token && col < cols) {
    matrix->data[row][col] = atof(token);
    col++;
    token = strtok_r(NULL, ",", &saveptr);
  }
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

void free_multiple_matrices(int count, ...) {
  va_list args;
  va_start(args, count);

  for (int i = 0; i < count; i++) {
    Matrix *matrix = va_arg(args, Matrix *);
    free_matrix(matrix);
  }

  va_end(args);
}

int get_max_number_width(MatrixArray matrix_array) {
  int max_width = 0;
  for (int m = 0; m < matrix_array.num_matrices; m++) {
    Matrix matrix = matrix_array.matrices[m];
    for (int i = 0; i < matrix.rows; i++) {
      for (int j = 0; j < matrix.cols; j++) {
        int width = snprintf(NULL, 0, "%.0f", matrix.data[i][j]);
        if (width > max_width) {
          max_width = width;
        }
      }
    }
  }
  return max_width;
}

MatrixArray initialize_matrix_array() {
  MatrixArray matrix_array;
  matrix_array.num_matrices = 0;
  matrix_array.matrices = NULL;
  return matrix_array;
}

int is_blank_line(const char *line, int len) {
  for (int i = 0; i < len; i++) {
    if (line[i] != ',' && line[i] != '\n' && line[i] != '\r') {
      return 0;
    }
  }
  return 1;
}

// Function to multiply matrices in pairs inside a MatrixArray using the
// specified function
MatrixArray multiply_matrix_array(MatrixArray input,
                                  Matrix (*multiply_func)(Matrix, Matrix)) {
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
        input.matrices[i], input.matrices[i + 1], multiply_func);
    clock_t end = clock();
    output.elapsed_times[i / 2] = (double)(end - start) / CLOCKS_PER_SEC;
  }

  return output;
}

// Function to multiply a pair of matrices using the specified function
Matrix multiply_matrix_pair(Matrix a, Matrix b,
                            Matrix (*multiply_func)(Matrix, Matrix)) {
  return multiply_func(a, b);
}

void print_elapsed_times(MatrixArray matrix_array) {
  printf("Elapsed times (seconds):\n");
  for (int i = 0; i < matrix_array.num_matrices; i++) {
    printf("Matrix pair %d: %.6f\n", i + 1, matrix_array.elapsed_times[i]);
  }
}

void print_matrices(MatrixArray matrix_array) {
  int max_width = get_max_number_width(matrix_array);

  for (int m = 0; m < matrix_array.num_matrices; m++) {
    printf("Matrix %d:\n", m + 1);
    Matrix matrix = matrix_array.matrices[m];
    for (int i = 0; i < matrix.rows; i++) {
      for (int j = 0; j < matrix.cols; j++) {
        printf("%*.0f ", max_width, matrix.data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
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
