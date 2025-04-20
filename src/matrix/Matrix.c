#include "Matrix.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../output/Output.h"

void destroy_matrix(Matrix* matrix) {
  if (matrix->data != NULL && matrix != NULL) {
    for (size_t row_index = 0; row_index < matrix->rows; row_index++) {
      free(matrix->data[row_index]);
    }
    free(matrix->data);
    matrix->data = NULL;
    matrix->rows = 0;
    matrix->cols = 0;
  }
}

Matrix create_matrix(size_t rows, size_t cols) {
  Matrix matrix             = {0};
  bool   allocation_success = true;

  if (rows > 0 && cols > 0) {

    matrix.data = (MATRIX_TYPE**)malloc(rows * sizeof(MATRIX_TYPE*));
    allocation_success = (matrix.data != NULL);

    if (allocation_success) {
      matrix.rows = rows;
      matrix.cols = cols;

      for (size_t row_index = 0; row_index < rows && allocation_success;
           row_index++) {
        matrix.data[row_index] =
            (MATRIX_TYPE*)malloc(cols * sizeof(MATRIX_TYPE));
        allocation_success = (matrix.data[row_index] != NULL);
      }

      if (!allocation_success) {
        for (size_t cleanup_row = 0; cleanup_row < matrix.rows;
             cleanup_row++) {
          if (matrix.data[cleanup_row] != NULL) {
            free(matrix.data[cleanup_row]);
          }
        }
        free(matrix.data);
        matrix = (Matrix){0};
      }
    }
  }

  return matrix;
}

void print_matrix(const Matrix* matrix) {
  double* data = NULL;

  // Проверка входных данных
  if (matrix && matrix->data) {
    data = malloc(matrix->rows * matrix->cols * sizeof(double));

    if (data) {
      for (size_t row_index = 0; row_index < matrix->rows; row_index++) {
        for (size_t col_index = 0; col_index < matrix->cols; col_index++) {
          data[row_index * matrix->cols + col_index] =
              matrix->data[row_index][col_index];
        }
      }

      // Вызов функции вывода
      output_print_matrix(matrix->rows, matrix->cols, data);
    }
  }

  if (data) {
    free(data);
  }
}

Matrix load_matrix_from_file(const char* filename) {
  Matrix result = {0, 0, NULL};  // Инициализация пустой матрицы
  double* file_data = NULL;
  int is_successful = 0;  // Флаг успешного выполнения

  // Проверка входных данных
  const int is_input_valid = (filename != NULL);

  if (is_input_valid) {
    // Загрузка данных из файла
    int rows = 0, cols = 0;
    file_data = output_load_matrix_from_file(&rows, &cols, filename);
    const int is_data_loaded = (file_data != NULL);

    if (is_data_loaded) {
      // Создание матрицы
      result                      = create_matrix(rows, cols);
      const int is_matrix_created = (result.data != NULL);

      if (is_matrix_created) {
        // Копирование данных в матрицу
        for (size_t row_index = 0; row_index < (size_t)rows; row_index++) {
          for (size_t col_index = 0; col_index < (size_t)cols; col_index++) {
            result.data[row_index][col_index] =
                file_data[row_index * cols + col_index];
          }
        }
        is_successful = 1;  // Все операции выполнены успешно
      }
    }
  }

  // Освобождение временных данных
  if (file_data != NULL) {
    free(file_data);
  }

  // Очистка в случае ошибки
  if (!is_successful && result.data != NULL) {
    destroy_matrix(&result);
    result.rows = 0;
    result.cols = 0;
    result.data = NULL;
  }

  return result;
}

int write_matrix_to_file(const Matrix* matrix, const char* filename) {
  double* linear_data = NULL;
  int success = 0;  // По умолчанию считаем операцию успешной

  // Проверка входных данных
  const int is_input_valid =
      (matrix != NULL) && (matrix->data != NULL) && (filename != NULL);

  if (is_input_valid) {
    // Выделение памяти
    linear_data = malloc(matrix->rows * matrix->cols * sizeof(double));
    const int is_memory_allocated = (linear_data != NULL);

    if (is_memory_allocated) {
      // Копирование данных в линейный массив
      for (size_t row_index = 0; row_index < matrix->rows; row_index++) {
        for (size_t col_index = 0; col_index < matrix->cols; col_index++) {
          linear_data[row_index * matrix->cols + col_index] =
              matrix->data[row_index][col_index];
        }
      }

      // Запись в файл
      success = output_write_matrix_to_file(matrix->rows, matrix->cols,
                                            linear_data, filename);
    }
  }

  // Освобождение памяти
  if (linear_data != NULL) {
    free(linear_data);
  }

  return success;
}

MATRIX_TYPE matrix_determinant(const Matrix* matrix) {
  MATRIX_TYPE det      = 0;
  bool        is_valid = false;

  // Проверка входной матрицы
  if (matrix != NULL && matrix->data != NULL) {
    const size_t n = matrix->rows;
    is_valid       = (n == matrix->cols) && (n > 0);

    if (is_valid) {
      if (n == 1) {
        det = matrix->data[0][0];  // Для матрицы 1x1 - сам элемент
      } else if (n == 2) {
        // Для матрицы 2x2: ad - bc
        det = matrix->data[0][0] * matrix->data[1][1] -
              matrix->data[0][1] * matrix->data[1][0];
      } else {
        // Рекурсивный случай для матриц NxN (N > 2)
        for (size_t col_index = 0; col_index < n; col_index++) {
          Matrix submatrix         = {0};
          bool   submatrix_created = false;

          // Создание подматрицы (минора)
          submatrix = create_matrix(n - 1, n - 1);
          if (submatrix.data != NULL) {
            submatrix_created = true;

            // Заполнение подматрицы (исключаем текущую колонку)
            for (size_t row_index = 1; row_index < n; row_index++) {
              size_t subcol = 0;
              for (size_t k = 0; k < n; k++) {
                if (k != col_index) {
                  submatrix.data[row_index - 1][subcol++] =
                      matrix->data[row_index][k];
                }
              }
            }

            // Рекурсивный вызов для минора
            MATRIX_TYPE minor_det = matrix_determinant(&submatrix);
            det += (col_index % 2 == 0 ? 1 : -1) *
                   matrix->data[0][col_index] * minor_det;
          }

          // Освобождение памяти подматрицы
          if (submatrix_created) {
            destroy_matrix(&submatrix);
          }
        }
      }
    }
  }

  return det;  // При return возвращается либо 0 (при ошибке) либо значение
               // детерминанта
}

Matrix transpose_matrix(const Matrix* matrix) {
  Matrix mat         = {0};
  int    input_valid = 0;

  // Проверка входных данных
  input_valid =
      (matrix != NULL) && (matrix->rows > 0) && (matrix->cols > 0);

  if (input_valid) {
    mat = create_matrix(matrix->cols, matrix->rows);
    if (mat.data != NULL) {
      for (size_t row_index = 0; row_index < matrix->rows; row_index++) {
        for (size_t col_index = 0; col_index < matrix->cols; col_index++) {
          mat.data[col_index][row_index] =
              matrix->data[row_index][col_index];
        }
      }
    }
  }

  return mat;  // При return возвращается либо транспонированная матрица
               // либо нулевая матрица (при ошибке)
}

int matrix_multiply(const Matrix* A, const Matrix* B, Matrix* result) {
  int status = 1;  // По умолчанию состояние ошибки

  // Проверка входных параметров
  if (A != NULL && B != NULL && result != NULL && A->data != NULL &&
      B->data != NULL && result->data != NULL) {

    // Проверка совместимости размеров матриц
    if (A->cols == B->rows && result->rows == A->rows &&
        result->cols == B->cols) {

      // Выполнение умножения матриц
      for (size_t row_index = 0; row_index < A->rows; row_index++) {
        for (size_t col_index = 0; col_index < B->cols; col_index++) {
          MATRIX_TYPE sum = 0;
          for (size_t k = 0; k < A->cols; k++) {
            sum += A->data[row_index][k] * B->data[k][col_index];
          }
          result->data[row_index][col_index] = sum;
        }
      }
      status = 0;  // Успешное выполнение
    }
  }

  return status;
}

int matrix_subtract(const Matrix* A, const Matrix* B, Matrix* result) {
  int    status     = -1;  // Флаг ошибок
  size_t rows_match = 0;   // Флаг совпадения строк
  size_t cols_match = 0;  // Флаг совпадения столбцов
  int    pointers_valid = 0;  // Флаг для указателей

  // Проверка указателей
  pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);
  if (!pointers_valid) {
    status = -1;
  } else {
    // Проверка размеров
    rows_match = (A->rows == B->rows);
    cols_match = (A->cols == B->cols);
    if (!rows_match || !cols_match)
      status = -1;
    else {
      // Выполнение вычитания
      for (size_t row_index = 0; row_index < A->rows; row_index++) {
        for (size_t col_index = 0; col_index < A->cols; col_index++) {
          result->data[row_index][col_index] =
              A->data[row_index][col_index] -
              B->data[row_index][col_index];
        }
      }
      status = 0;  // Успешное завершение
    }
  }

  return status;
}

int matrix_add(const Matrix* A, const Matrix* B, Matrix* result) {
  int    status     = -1;  // Флаг ошибок
  size_t rows_match = 0;   // Флаг совпадения строк
  size_t cols_match = 0;  // Флаг совпадения столбцов
  int    pointers_valid = 0;  // Флаг для указателей

  // Проверка указателей
  pointers_valid = (A != NULL) && (B != NULL) && (result != NULL);
  if (!pointers_valid) {
    status = -1;
  } else {
    // Проверка размеров
    rows_match = (A->rows == B->rows);
    cols_match = (A->cols == B->cols);
    if (!rows_match || !cols_match)
      status = -1;
    else {
      // Выполнение вычитания
      for (size_t row_index = 0; row_index < A->rows; row_index++) {
        for (size_t col_index = 0; col_index < A->cols; col_index++) {
          result->data[row_index][col_index] =
              A->data[row_index][col_index] +
              B->data[row_index][col_index];
        }
      }
      status = 0;  // Успешное завершение
    }
  }

  return status;
}
