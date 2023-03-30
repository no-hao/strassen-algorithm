#include "../include/matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix read_csv(const char *filename, int n) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error: Unable to open the file %s\n", filename);
    exit(1);
  }

  int rows = (int)pow(2, n);
  int cols = rows;

  // Allocate memory for the matrix
  Matrix m;
  m.rows = rows;
  m.cols = cols;
  m.data = (float **)malloc(rows * sizeof(float *));
  for (int i = 0; i < rows; i++) {
    m.data[i] = (float *)malloc(cols * sizeof(float));
  }

  // Read the file and store the values in the matrix
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  int row = 0;
  while ((read = getline(&line, &len, file)) != -1 && row < rows) {
    char *token = strtok(line, ",");
    int col = 0;
    while (token && col < cols) {
      m.data[row][col] = atof(token);
      col++;
      token = strtok(NULL, ",");
    }
    row++;
  }

  fclose(file);
  if (line) {
    free(line);
  }
  return m;
}

void print_matrix(Matrix matrix) {
  for (int i = 0; i < matrix.rows; i++) {
    for (int j = 0; j < matrix.cols; j++) {
      printf("%.0f ", matrix.data[i][j]);
    }
    printf("\n");
  }
}

void free_matrix(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    free(m->data[i]);
  }
  free(m->data);
  m->data = NULL;
  m->rows = 0;
  m->cols = 0;
}
