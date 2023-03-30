#include "../include/file_utils.h"

void write_matrix_to_file(FILE *file, Matrix matrix) {
  for (int j = 0; j < matrix.rows; j++) {
    for (int k = 0; k < matrix.cols; k++) {
      fprintf(file, "%.0f", matrix.data[j][k]);
      if (k < matrix.cols - 1) {
        fprintf(file, ",");
      }
    }
    fprintf(file, "\n");
  }
}

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

// Function to write the output matrices to a CSV file
void write_output_to_csv(const char *filename, MatrixArray output) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error: unable to create output file.\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < output.count; i++) {
    write_matrix_to_file(file, output.matrices[i]);
    if (i < output.count - 1) {
      fprintf(file, "\n");
    }
  }

  fclose(file);
}
