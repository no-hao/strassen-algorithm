#include "../include/brute_force.h"
#include "../include/file_utils.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <matrix_csv_file> <n>\n", argv[0]);
    return 1;
  }

  const char *file_path = argv[1];
  int n = atoi(argv[2]);

  MatrixArray input_matrices = read_csv(file_path, n);

  if (input_matrices.matrices == NULL) {
    fprintf(stderr, "Error reading matrices from file: %s\n", file_path);
    return 1;
  }

  printf("Matrices read from file %s:\n", file_path);
  print_matrices(input_matrices);
  printf("\n");

  MatrixArray output_matrices =
      multiply_matrix_array(input_matrices, multiply_matrix_pairs_bf);

  printf("Output matrices:\n");
  print_matrices(output_matrices);
  printf("\n");

  // Call the print_elapsed_times function here
  printf("Elapsed times for each pair of matrix multiplications:\n");
  print_elapsed_times(output_matrices);
  printf("\n");

  char *output_file = generate_output_file_path(file_path);
  write_output_to_csv(output_file, output_matrices);
  printf("Output matrices written to file: %s\n", output_file);

  free(output_file);

  free_matrices(&input_matrices);
  free_matrices(&output_matrices);

  return 0;
}
