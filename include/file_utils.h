// file_utils.h
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "matrix.h"
#include <stdio.h>

char *generate_output_file_path(const char *input_file_path);

void write_matrix_to_file(FILE *file, Matrix matrix);

void write_separator_line(FILE *file, int cols);

void write_output_to_csv(const char *output_file, MatrixArray output_matrices);

MatrixArray read_csv(const char *filename, int n);

#endif
