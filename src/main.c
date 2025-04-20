#include <stdio.h>
#include <stdlib.h>

#include "../matrix/Matrix.h"
#include "../output/Output.h"

int main() {
  int res = 0;  // Общий флаг успеха операций

  // 1. Загрузка матриц
  Matrix A = load_matrix_from_file("data/main_data/Matrix_A.txt");
  Matrix B = load_matrix_from_file("data/main_data/Matrix_B.txt");
  Matrix C = load_matrix_from_file("data/main_data/Matrix_C.txt");
  Matrix D = load_matrix_from_file("data/main_data/Matrix_D.txt");

  if (!A.data || !B.data || !C.data || !D.data) {
    res = 1;
    fprintf(stderr, "Ошибка загрузки матриц.\n");
  }

  // 2. Транспонирование D
  Matrix D_transpose = {0};
  if (res == 0) {
    D_transpose = transpose_matrix(&D);
    if (!D_transpose.data) {
      res = 1;
      fprintf(stderr, "Ошибка транспонирования B.\n");
    }
  }

  // 3. Умножение B * C
  Matrix BC = {0};
  if (res == 0) {
    BC = create_matrix(B.rows, C.cols);
    if (!BC.data) {
      res = 1;
      fprintf(stderr, "Ошибка создания матрицы AB.\n");
    }
  }

  if (res == 0) {
    if (matrix_multiply(&B, &C, &BC) != 0) {
      res = 1;
      fprintf(stderr, "Ошибка умножения матриц.\n");
    }
  }

  // 4. Вычитание A - BC
  Matrix A_minus_BC = {0};
  if (res == 0) {
    A_minus_BC = create_matrix(A.rows, A.cols);
    if (!A_minus_BC.data) {
      res = 1;
      fprintf(stderr, "Ошибка создания матрицы AB_minus_C.\n");
    }
  }

  if (res == 0) {
    if (matrix_subtract(&A, &BC, &A_minus_BC) != 0) {
      res = 1;
      fprintf(stderr, "Ошибка вычитания матриц.\n");
    }
  }

  // 5. Сложение с D транспонированной (A_minus_BC + D^T)
  Matrix result = {0};
  if (res == 0) {
    result = create_matrix(A_minus_BC.rows, A_minus_BC.cols);
    if (!result.data) {
      res = 1;
      fprintf(stderr, "Ошибка создания результирующей матрицы.\n");
    }
  }

  if (res == 0) {
    if (matrix_add(&A_minus_BC, &D_transpose, &result) != 0) {
      res = 1;
      fprintf(stderr, "Ошибка сложения матриц.\n");
    }
  }

  // 6. Вывод и сохранение результата
  if (res == 0) {
    printf("Результат выражения A×B^T−C+D:\n");
    print_matrix(&result);
    if (write_matrix_to_file(&result, "data/output/result.txt") != 0) {
      res = 1;
      fprintf(stderr, "Ошибка сохранения результата.\n");
    } else {
      printf("Результат сохранен в data/output/result.txt\n");
    }
  }

  // Освобождение памяти
  destroy_matrix(&A);
  destroy_matrix(&B);
  destroy_matrix(&C);
  destroy_matrix(&D);
  destroy_matrix(&D_transpose);
  destroy_matrix(&BC);
  destroy_matrix(&A_minus_BC);
  destroy_matrix(&result);

  return res;
}