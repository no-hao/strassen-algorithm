// file_utils.h
#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "matrix.h"
#include <stdio.h>

OptionType get_option_type(const char *option_str);

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

void print_matrices(MatrixArray matrix_array);

void print_elapsed_times(MatrixArray matrix_array, const char *method);

void print_results(MatrixArray output_matrices, const char *algorithm_name);

void process_algorithm(MultiplyFunction algorithm_function,
                       MatrixArray input_matrices, Options options,
                       const char *file_path, const char *algorithm_name);

#endif
