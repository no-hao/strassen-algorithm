#include "../include/brute_force.h"
#include "../include/file_utils.h"
#include "../include/matrix.h"
#include "../include/strassen.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // TODO: make sure we implement crossover_point and appropriate expected
  // arguments.
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <matrix_csv_file> <crossover_point>\n", argv[0]);
    return 1;
  }

  const char *file_path = argv[1];
  // TODO: this will likely be the crossover_point.
  // int crossover_point = atoi(argv[2]);

  MatrixArray input_matrices = read_csv(file_path);

  if (input_matrices.matrices == NULL) {
    fprintf(stderr, "Error reading matrices from file: %s\n", file_path);
    return 1;
  }

  printf("Matrices read from file %s:\n", file_path);
  print_matrices(input_matrices);
  printf("\n");

  MatrixArray output_matrices_bf =
      multiply_matrix_array(input_matrices, multiply_matrix_pairs_bf);

  printf("Output matrices (Brute Force):\n");
  print_matrices(output_matrices_bf);
  printf("\n");

  MatrixArray output_matrices_strassen =
      multiply_matrix_array(input_matrices, strassen_algorithm);

  printf("Output matrices (Strassen Algorithm):\n");
  print_matrices(output_matrices_strassen);
  printf("\n");

  printf(
      "Elapsed times for each pair of matrix multiplications (Brute Force):\n");
  print_elapsed_times(output_matrices_bf);
  printf("\n");

  printf("Elapsed times for each pair of matrix multiplications (Strassen "
         "Algorithm):\n");
  print_elapsed_times(output_matrices_strassen);
  printf("\n");

  char *output_file_bf = generate_output_file_path(file_path, "BF");
  write_output_to_csv(output_file_bf, output_matrices_bf);
  printf("Output matrices (Brute Force) written to file: %s\n", output_file_bf);
  free(output_file_bf);

  char *output_file_strassen = generate_output_file_path(file_path, "Strassen");
  write_output_to_csv(output_file_strassen, output_matrices_strassen);
  printf("Output matrices (Strassen Algorithm) written to file: %s\n",
         output_file_strassen);
  free(output_file_strassen);

  free_matrix_array(&input_matrices);
  free_matrix_array(&output_matrices_bf);
  free_matrix_array(&output_matrices_strassen);

  return 0;
}
