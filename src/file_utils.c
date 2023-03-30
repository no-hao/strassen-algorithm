#include "../include/file_utils.h"
#include <string.h>

char *generate_output_file_path(const char *input_file_path) {
  const char *output_dir = "./output/";
  const char *suffix = "-OUTPUT.csv";
  const char *filename_start = strrchr(input_file_path, '/');
  const char *extension = ".csv";

  if (filename_start == NULL) {
    filename_start = input_file_path;
  } else {
    filename_start++;
  }

  size_t filename_length = strlen(filename_start) - strlen(extension);
  char *filename_no_ext = (char *)malloc(filename_length + 1);
  strncpy(filename_no_ext, filename_start, filename_length);
  filename_no_ext[filename_length] = '\0';

  char *output_file_path = (char *)malloc(
      strlen(output_dir) + strlen(filename_no_ext) + strlen(suffix) + 1);
  strcpy(output_file_path, output_dir);
  strcat(output_file_path, filename_no_ext);
  strcat(output_file_path, suffix);

  free(filename_no_ext);
  return output_file_path;
}

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

void write_separator_line(FILE *file, int cols) {
  for (int l = 0; l < cols - 1; l++) {
    fprintf(file, ",");
  }
  fprintf(file, "\n");
}

void write_output_to_csv(const char *output_file, MatrixArray output_matrices) {
  FILE *file = fopen(output_file, "w");
  if (file == NULL) {
    fprintf(stderr, "Error opening output file: %s\n", output_file);
    return;
  }

  for (int k = 0; k < output_matrices.num_matrices; k++) {
    Matrix *matrix = &output_matrices.matrices[k];
    for (int i = 0; i < matrix->rows; i++) {
      for (int j = 0; j < matrix->cols; j++) {
        fprintf(file, "%.0f", matrix->data[i][j]);
        if (j < matrix->cols - 1) {
          fprintf(file, ",");
        } else {
          fprintf(file, "\n");
        }
      }
    }
    if (k < output_matrices.num_matrices - 1) {
      write_separator_line(file, matrix->cols);
    }
  }

  fclose(file);
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
      matrix_array.num_matrices++;
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
