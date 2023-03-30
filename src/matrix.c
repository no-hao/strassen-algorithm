#include "../include/matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MatrixArray read_csv(const char *filename, int n) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error: Unable to open the file %s\n", filename);
    exit(1);
  }

  int rows = 1 << n;
  int cols = rows;

  MatrixArray matrix_array = initialize_matrix_array();

  int current_matrix = -1;
  int row = 0;

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  int create_new_matrix = 1;

  while ((read = getline(&line, &len, file)) != -1) {
    if (is_blank_line(line, read)) {
      if (create_new_matrix == 0) {
        break;
      }
      continue;
    }

    create_new_matrix = 1;

    if (row == 0) {
      create_new_matrix = 0;
      current_matrix++;
      matrix_array.count++;
      matrix_array.matrices =
          add_matrix(matrix_array.matrices, current_matrix, rows, cols);
    }

    add_line_to_matrix(&matrix_array.matrices[current_matrix], line, cols, row);

    row++;
    if (row == rows) {
      row = 0;
    }
  }

  fclose(file);
  if (line) {
    free(line);
  }

  return matrix_array;
}

MatrixArray initialize_matrix_array() {
  MatrixArray matrix_array;
  matrix_array.count = 0;
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

void print_matrices(MatrixArray matrix_array) {
  for (int m = 0; m < matrix_array.count; m++) {
    printf("Matrix %d:\n", m + 1);
    Matrix matrix = matrix_array.matrices[m];
    for (int i = 0; i < matrix.rows; i++) {
      for (int j = 0; j < matrix.cols; j++) {
        printf("%.0f ", matrix.data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
}

void free_matrices(MatrixArray *matrix_array) {
  if (matrix_array->matrices) {
    for (int m = 0; m < matrix_array->count; m++) {
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
  matrix_array->count = 0;
}
