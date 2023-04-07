#include "../include/brute_force.h"
#include "../include/combined.h"
#include "../include/file_utils.h"
#include "../include/matrix.h"
#include "../include/strassen.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr,
            "Usage: %s <matrix_csv_file> [-pr] [-pt] [-b|-s|-c "
            "<crossover_point>]\n",
            argv[0]);
    return 1;
  }

  const char *file_path = argv[1];
  Options options = {false, false, false, false, false};
  int crossover_point = -1;

  for (int i = 2; i < argc; ++i) {
    OptionType option_type = get_option_type(argv[i]);

    switch (option_type) {
    case PR:
      options.print_results_flag = true;
      break;
    case PT:
      options.print_time_flag = true;
      break;
    case B:
      options.brute_force_flag = true;
      break;
    case S:
      options.strassen_flag = true;
      break;
    case C:
      options.combined_flag = true;
      if (i + 1 < argc) {
        crossover_point = atoi(argv[i + 1]);
        i++;
      }
      break;
    default:
      fprintf(stderr, "Invalid option: %s\n", argv[i]);
      return 1;
    }
  }

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
    MultiplyFunction bf_function = {brute_force_algorithm, NULL};
    process_algorithm(bf_function, input_matrices, options, file_path,
                      "Brute Force");
  }

  if (options.strassen_flag) {
    MultiplyFunction sf_function = {strassen_algorithm, NULL};
    process_algorithm(sf_function, input_matrices, options, file_path,
                      "Strassen Algorithm");
  }

  if (options.combined_flag) {
    MultiplyFunction combined_function = {combined_algorithm, &crossover_point};
    process_algorithm(combined_function, input_matrices, options, file_path,
                      "Combined Algorithm");
  }

  free_matrix_array(&input_matrices);

  return 0;
}
