#include "../include/brute_force.h"
#include "../include/file_utils.h"
#include "../include/matrix.h"
#include "../include/strassen.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s <matrix_csv_file> <crossover_point> [-p]\n",
            argv[0]);
    return 1;
  }

  const char *file_path = argv[1];
  int print_flag = (argc == 4) && strcmp(argv[3], "-p") == 0;

  MatrixArray input_matrices = read_csv(file_path);

  if (input_matrices.matrices == NULL) {
    fprintf(stderr, "Error reading matrices from file: %s\n", file_path);
    return 1;
  }

  MatrixArray output_matrices_bf =
      multiply_matrix_array(input_matrices, multiply_matrix_pairs_bf);

  MatrixArray output_matrices_strassen =
      multiply_matrix_array(input_matrices, strassen_algorithm);

  if (print_flag) {
    print_results(file_path, input_matrices, output_matrices_bf,
                  output_matrices_strassen);
  }

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
