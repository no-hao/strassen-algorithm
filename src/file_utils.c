#include "../include/file_utils.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

OptionType get_option_type(const char *option_str) {
  if (strcmp(option_str, "-pr") == 0) {
    return PR;
  } else if (strcmp(option_str, "-pt") == 0) {
    return PT;
  } else if (strcmp(option_str, "-b") == 0) {
    return B;
  } else if (strcmp(option_str, "-s") == 0) {
    return S;
  } else if (strcmp(option_str, "-c") == 0) {
    return C;
  } else {
    return INVALID;
  }
}

char *generate_output_file_path(const char *input_file_path,
                                const char *method) {
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

  char *output_file_path =
      (char *)malloc(strlen(output_dir) + strlen(filename_no_ext) +
                     strlen(method) + strlen(suffix) + 1);

  strcpy(output_file_path, output_dir);
  strcat(output_file_path, filename_no_ext);
  strcat(output_file_path, "-");
  strcat(output_file_path, method);
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
    write_matrix_to_file(file, *matrix);
    if (k < output_matrices.num_matrices - 1) {
      write_separator_line(file, matrix->cols);
    }
  }

  fclose(file);
}

// Include your MatrixArray, Matrix and other custom data structure definitions
// here
MatrixArray read_csv(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    fprintf(stderr, "Error: Unable to open the file %s\n", filename);
    exit(1);
  }

  MatrixArray matrix_array = initialize_matrix_array();
  int current_matrix = -1;
  int row = 0;
  int cols = -1;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  bool create_new_matrix = true;

  while ((read = getline(&line, &len, file)) != -1) {
    if (is_blank_line(line, read)) {
      create_new_matrix = true;
      continue;
    }

    if (create_new_matrix) {
      create_new_matrix = false;
      current_matrix++;
      matrix_array.num_matrices++;

      if (cols == -1) {
        cols = count_columns(line);
      }

      matrix_array.matrices = realloc(
          matrix_array.matrices, matrix_array.num_matrices * sizeof(Matrix));
      if (!matrix_array.matrices) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
      }
      matrix_array.matrices[current_matrix] = create_matrix(cols, cols);
      row = 0;
    }

    add_line_to_matrix(&matrix_array.matrices[current_matrix], line, cols, row);
    row++;
  }

  fclose(file);
  if (line) {
    free(line);
  }

  return matrix_array;
}

void add_line_to_matrix(Matrix *matrix, const char *line, int cols, int row) {
  char *saveptr;
  char *token = strtok_r((char *)line, ",", &saveptr);
  int col = 0;
  while (token && col < cols) {
    matrix->data[row][col] = atof(token);
    col++;
    token = strtok_r(NULL, ",", &saveptr);
  }
}

int is_blank_line(const char *line, int len) {
  for (int i = 0; i < len; i++) {
    if (line[i] != ',' && line[i] != '\n' && line[i] != '\r') {
      return 0;
    }
  }
  return 1;
}

int count_columns(const char *line) {
  int cols = 1;
  for (int i = 0; line[i] != '\0'; i++) {
    if (line[i] == ',') {
      cols++;
    }
  }
  return cols;
}

Matrix create_matrix(int rows, int cols) {
  Matrix matrix;
  matrix.rows = rows;
  matrix.cols = cols;
  matrix.data = malloc(rows * sizeof(double *));
  for (int i = 0; i < rows; i++) {
    matrix.data[i] = calloc(cols, sizeof(double));
  }
  return matrix;
}

int get_max_number_width(MatrixArray matrix_array) {
  int max_width = 0;
  for (int m = 0; m < matrix_array.num_matrices; m++) {
    Matrix matrix = matrix_array.matrices[m];
    for (int i = 0; i < matrix.rows; i++) {
      for (int j = 0; j < matrix.cols; j++) {
        int width = snprintf(NULL, 0, "%.0f", matrix.data[i][j]);
        if (width > max_width) {
          max_width = width;
        }
      }
    }
  }
  return max_width;
}

void print_matrices(MatrixArray matrix_array) {
  int max_width = get_max_number_width(matrix_array);

  for (int m = 0; m < matrix_array.num_matrices; m++) {
    printf("Matrix %d:\n", m + 1);
    Matrix matrix = matrix_array.matrices[m];
    for (int i = 0; i < matrix.rows; i++) {
      for (int j = 0; j < matrix.cols; j++) {
        printf("%*.0f ", max_width, matrix.data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }
}

void process_algorithm(MultiplyFunction algorithm_function,
                       MatrixArray input_matrices, Options options,
                       const char *file_path, const char *algorithm_name) {
  MatrixArray output_matrices =
      multiply_matrix_array(input_matrices, algorithm_function);

  if (options.print_results_flag) {
    print_results(output_matrices, algorithm_name);
  }

  if (options.print_time_flag) {
    print_elapsed_times(output_matrices, algorithm_name);
  }

  char *output_file = generate_output_file_path(file_path, algorithm_name);
  write_output_to_csv(output_file, output_matrices);
  printf("Output matrices (%s) written to file: %s\n", algorithm_name,
         output_file);
  free(output_file);
  free_matrix_array(&output_matrices);
}

void print_elapsed_times(MatrixArray matrix_array, const char *method) {
  printf("Elapsed times for each pair of matrix multiplications (%s):\n",
         method);

  for (int i = 0; i < matrix_array.num_matrices; i++) {
    printf("Matrix pair %d: %.6f\n", i + 1, matrix_array.elapsed_times[i]);
  }

  printf("\n");
}

void print_results(MatrixArray output_matrices, const char *algorithm_name) {
  printf("\nOutput matrices (%s):\n", algorithm_name);
  print_matrices(output_matrices);
  printf("\n");
}
