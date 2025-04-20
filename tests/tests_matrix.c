#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/matrix/Matrix.h"

void test_matrix_creation(void) {
  Matrix m = create_matrix(2, 3);
  CU_ASSERT_PTR_NOT_NULL(m.data);
  CU_ASSERT_EQUAL(m.rows, 2);
  CU_ASSERT_EQUAL(m.cols, 3);
  destroy_matrix(&m);

  Matrix invalid = create_matrix(-1, 0);
  CU_ASSERT_PTR_NULL(invalid.data);
  CU_ASSERT_EQUAL(invalid.rows, 0);
  CU_ASSERT_EQUAL(invalid.cols, 0);
}

void test_matrix_addition(void) {
  Matrix a = create_matrix(2, 2);
  Matrix b = create_matrix(2, 2);

  // Заполняем матрицы тестовыми данными
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      a.data[i][j] = i + j;
      b.data[i][j] = (i + j) * 2;
    }
  }

  Matrix result     = create_matrix(2, 2);
  int    add_result = matrix_add(&a, &b, &result);

  CU_ASSERT_EQUAL(add_result, 0);
  CU_ASSERT_DOUBLE_EQUAL(result.data[0][0], 0.0, 0.001);
  CU_ASSERT_DOUBLE_EQUAL(result.data[1][1], 6.0, 0.001);

  destroy_matrix(&a);
  destroy_matrix(&b);
  destroy_matrix(&result);

  // Тест с несовместимыми размерами
  Matrix c              = create_matrix(3, 2);
  Matrix invalid_add    = create_matrix(2, 2);
  int    invalid_result = matrix_add(&a, &c, &invalid_add);
  CU_ASSERT_NOT_EQUAL(invalid_result, 0);

  destroy_matrix(&c);
  destroy_matrix(&invalid_add);
}

void test_matrix_multiplication(void) {
  Matrix a = create_matrix(2, 3);
  Matrix b = create_matrix(3, 2);

  // Заполняем матрицы тестовыми данными
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      a.data[i][j] = i + j;
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      b.data[i][j] = i * j;
    }
  }

  Matrix result     = create_matrix(2, 2);
  int    mul_result = matrix_multiply(&a, &b, &result);

  CU_ASSERT_EQUAL(mul_result, 0);
  CU_ASSERT_DOUBLE_EQUAL(result.data[0][0], 0.0, 0.001);
  CU_ASSERT_DOUBLE_EQUAL(result.data[1][1], 8.0, 0.001);

  destroy_matrix(&a);
  destroy_matrix(&b);
  destroy_matrix(&result);

  // Тест с несовместимыми размерами
  Matrix c              = create_matrix(2, 2);
  Matrix invalid_mul    = create_matrix(2, 2);
  int    invalid_result = matrix_multiply(&a, &c, &invalid_mul);
  CU_ASSERT_NOT_EQUAL(invalid_result, 0);

  destroy_matrix(&c);
  destroy_matrix(&invalid_mul);
}

void test_matrix_transpose(void) {
  Matrix m = create_matrix(2, 3);

  // Заполняем матрицу тестовыми данными
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      m.data[i][j] = i * 3 + j;
    }
  }

  Matrix transposed = transpose_matrix(&m);

  CU_ASSERT_EQUAL(transposed.rows, 3);
  CU_ASSERT_EQUAL(transposed.cols, 2);
  CU_ASSERT_DOUBLE_EQUAL(transposed.data[0][0], 0.0, 0.001);
  CU_ASSERT_DOUBLE_EQUAL(transposed.data[1][1], 4.0, 0.001);

  destroy_matrix(&m);
  destroy_matrix(&transposed);
}

void test_null_safety(void) {
  // Проверка обработки NULL указателей
  Matrix result = create_matrix(1, 1);

  CU_ASSERT_NOT_EQUAL(matrix_add(NULL, NULL, &result), 0);
  CU_ASSERT_NOT_EQUAL(matrix_subtract(NULL, NULL, &result), 0);
  CU_ASSERT_NOT_EQUAL(matrix_multiply(NULL, NULL, &result), 0);

  Matrix transposed = transpose_matrix(NULL);
  CU_ASSERT_PTR_NULL(transposed.data);

  destroy_matrix(&result);

  // Проверка с частично валидными аргументами
  Matrix m = create_matrix(1, 1);
  CU_ASSERT_NOT_EQUAL(matrix_add(&m, NULL, &result), 0);
  CU_ASSERT_NOT_EQUAL(matrix_multiply(NULL, &m, &result), 0);

  destroy_matrix(&m);
}

void register_matrix_tests(void) {
  CU_pSuite suite = CU_add_suite("Matrix Tests", NULL, NULL);
  CU_add_test(suite, "Matrix Creation", test_matrix_creation);
  CU_add_test(suite, "Matrix Addition", test_matrix_addition);
  CU_add_test(suite, "Matrix Multiplication", test_matrix_multiplication);
  CU_add_test(suite, "Matrix Transpose", test_matrix_transpose);
  CU_add_test(suite, "NULL Safety", test_null_safety);
}
