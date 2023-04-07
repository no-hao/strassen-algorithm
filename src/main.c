#include "../include/brute_force.h"
#include "../include/combined.h"
#include "../include/file_utils.h"
#include "../include/matrix.h"
#include "../include/strassen.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 5) {
    fprintf(stderr, "Usage: %s <matrix_csv_file> [-p] [-b|-s|-c]\n", argv[0]);
    return 1;
  }

  const char *file_path = argv[1];
  Options options = {false, false, false, false};

  for (int i = 2; i < argc; ++i) {
    if (strcmp(argv[i], "-p") == 0) {
      options.print_flag = true;
    } else if (strcmp(argv[i], "-b") == 0) {
      options.brute_force_flag = true;
    } else if (strcmp(argv[i], "-s") == 0) {
      options.strassen_flag = true;
    } else if (strcmp(argv[i], "-c") == 0) {
      options.combined_flag = true;
    } else {
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      return 1;
    }
  }

  // If no algorithm flags were provided, set both brute_force_flag and
  // strassen_flag to true (run both algorithms)
  if (!options.brute_force_flag && !options.strassen_flag &&
      !options.combined_flag) {
    options.brute_force_flag = true;
    options.strassen_flag = true;
  }

  MatrixArray input_matrices = read_csv(file_path);

  if (input_matrices.matrices == NULL) {
    fprintf(stderr, "Error reading matrices from file: %s\n", file_path);
    return 1;
  }

  if (options.brute_force_flag) {
    MatrixArray output_matrices_bf =
        multiply_matrix_array(input_matrices, brute_force_algorithm);

    if (options.print_flag) {
      print_results_brute_force(file_path, input_matrices, output_matrices_bf);
    }

    char *output_file_bf = generate_output_file_path(file_path, "BF");
    write_output_to_csv(output_file_bf, output_matrices_bf);
    printf("Output matrices (Brute Force) written to file: %s\n",
           output_file_bf);
    free(output_file_bf);
    free_matrix_array(&output_matrices_bf);
  }

  if (options.strassen_flag) {
    MatrixArray output_matrices_strassen =
        multiply_matrix_array(input_matrices, strassen_algorithm);

    if (options.print_flag) {
      print_results_strassen(file_path, input_matrices,
                             output_matrices_strassen);
    }

    char *output_file_strassen =
        generate_output_file_path(file_path, "Strassen");
    write_output_to_csv(output_file_strassen, output_matrices_strassen);
    printf("Output matrices (Strassen Algorithm) written to file: %s\n",
           output_file_strassen);
    free(output_file_strassen);
    free_matrix_array(&output_matrices_strassen);
  }

  // Implement the combined algorithm here, and conditionally execute it based
  // on the combined_flag
  /* if (options.combined_flag) { */
  /*   MatrixArray output_matrices_combined = */
  /*       multiply_matrix_array(input_matrices, combined_algorithm); */
  /**/
  /*   if (options.print_flag) { */
  /*     print_results_combined(file_path, input_matrices, */
  /*                            output_matrices_combined); */
  /*   } */
  /**/
  /*   // ... rest of the code for combined flag */
  /* } */

  free_matrix_array(&input_matrices);

  return 0;
}
