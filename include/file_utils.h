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

void add_line_to_matrix(Matrix *matrix, const char *line, int cols, int row);

int is_blank_line(const char *line, int len);

int get_max_number_width(MatrixArray matrix_array);

int count_columns(const char *line);

Matrix create_matrix(int rows, int cols);

void print_elapsed_times(MatrixArray matrix_array, const char *method);

void print_matrices(MatrixArray matrix_array);

void print_results_brute_force(const char *file_path,
                               MatrixArray output_matrices_bf);

void print_results_strassen(const char *file_path,
                            MatrixArray output_matrices_strassen);

void print_results_combined(const char *file_path,
                            MatrixArray output_matrices_combined);

#endif
