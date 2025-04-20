#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

#include "../../include/config.h"

typedef struct {
  size_t        rows;  ///< Количество строк
  size_t        cols;  ///< Количество столбцов
  MATRIX_TYPE** data;  ///< Двумерный массив данных
} Matrix;

void   destroy_matrix(Matrix* matrix);
Matrix create_matrix(size_t rows, size_t cols);
void   print_matrix(const Matrix* matrix);
Matrix load_matrix_from_file(const char* filename);
int    write_matrix_to_file(const Matrix* matrix, const char* filename);
MATRIX_TYPE determinant(const Matrix* matrix);
Matrix      transpose_matrix(const Matrix* matrix);
int matrix_multiply(const Matrix* A, const Matrix* B, Matrix* result);
int matrix_subtract(const Matrix* A, const Matrix* B, Matrix* result);
int matrix_add(const Matrix* A, const Matrix* B, Matrix* result);

#endif  // MATRIX_H
