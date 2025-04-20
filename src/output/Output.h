#ifndef OUTPUT_H
#define OUTPUT_H

void output_print_matrix(int rows, int cols, const double* data);

int     output_write_matrix_to_file(int rows, int cols, const double* data,
                                    const char* filename);
double* output_load_matrix_from_file(int* rows, int* cols,
                                     const char* filename);

#endif  // OUTPUT_H