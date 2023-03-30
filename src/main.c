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

  Matrix m = read_csv(file_path, n);

  if (m.data == NULL) {
    fprintf(stderr, "Error reading matrix from file: %s\n", file_path);
    return 1;
  }

  printf("Matrix read from file %s:\n", file_path);
  print_matrix(m);
  printf("\n");

  free_matrix(&m);

  return 0;
}
