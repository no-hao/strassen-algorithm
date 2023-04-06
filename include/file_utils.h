// file_utils.h
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "matrix.h"
#include <stdio.h>

char *generate_output_file_path(const char *input_file_path,
                                const char *method);

void write_matrix_to_file(FILE *file, Matrix matrix);

void write_separator_line(FILE *file, int cols);

void write_output_to_csv(const char *output_file, MatrixArray output_matrices);

MatrixArray read_csv(const char *filename);

int count_columns(const char *line);

Matrix create_matrix(int rows, int cols);

void print_results(const char *file_path, MatrixArray input_matrices,
                   MatrixArray output_matrices_bf,
                   MatrixArray output_matrices_strassen);

#endif
