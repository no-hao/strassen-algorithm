#include "../include/file_utils.h"
#include <stdlib.h>
#include <string.h>

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

void add_line_to_matrix(Matrix *matrix, const char *line, int cols, int row) {
  char *token = strtok((char *)line, ",");
  int col = 0;
  while (token && col < cols) {
    matrix->data[row][col] = atof(token);
    col++;
    token = strtok(NULL, ",");
  }
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

void free_matrices(MatrixArray *matrix_array) {
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

// Function to multiply a pair of matrices using the specified function
Matrix multiply_matrix_pair(Matrix a, Matrix b,
                            Matrix (*multiply_func)(Matrix, Matrix)) {
  return multiply_func(a, b);
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

  for (int i = 0; i < input.num_matrices; i += 2) {
    output.matrices[i / 2] = multiply_matrix_pair(
        input.matrices[i], input.matrices[i + 1], multiply_func);
  }

  return output;
}
