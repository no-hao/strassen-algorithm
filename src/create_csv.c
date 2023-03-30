#include "../include/create_csv.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void write_random_matrix(FILE *file, int rows, int cols) {
  for (int j = 0; j < rows; j++) {
    for (int k = 0; k < cols; k++) {
      float rand_num = (float)rand() / (float)RAND_MAX * 9.0f;
      fprintf(file, "%.0f", rand_num);
      if (k < cols - 1) {
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

void create_csv(int n, int pairs) {
  int rows = 1 << n;
  int cols = 1 << n;
  int matrices_per_col = pairs * 2; // Multiply by 2 to account for pairs

  srand(time(NULL));

  char default_filename[256];
  sprintf(default_filename, "matrix-%d-pairs-of-%dx%d.csv", pairs, rows, cols);

  char filename[256];
  sprintf(filename, "./data/%s", default_filename);

  FILE *file = fopen(filename, "w");
  if (!file) {
    fprintf(stderr, "Error: Unable to create the file %s\n", filename);
    exit(1);
  }

  for (int i = 0; i < matrices_per_col; i++) {
    write_random_matrix(file, rows, cols);
    if (i < matrices_per_col - 1) {
      write_separator_line(file, cols);
    }
  }

  fclose(file);
  printf("File %s created successfully.\n", filename);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <n> <pairs>\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);
  int pairs = atoi(argv[2]);

  create_csv(n, pairs);

  return 0;
}
