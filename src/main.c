#include "../include/matrix.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <matrix_csv_file> <n>\n", argv[0]);
    return 1;
  }

  const char *file_path = argv[1];
  int n = atoi(argv[2]);

  MatrixArray matrix_array = read_csv(file_path, n);

  if (matrix_array.matrices == NULL) {
    fprintf(stderr, "Error reading matrices from file: %s\n", file_path);
    return 1;
  }

  printf("Matrices read from file %s:\n", file_path);
  print_matrices(matrix_array);
  printf("\n");

  free_matrices(&matrix_array);

  return 0;
}
