// file_utils.h
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "matrix.h"
#include <stdio.h>

void write_output_to_csv(const char *filename, MatrixArray output);

void write_matrix_to_file(FILE *file, Matrix matrix);

MatrixArray read_csv(const char *filename, int n);

#endif
