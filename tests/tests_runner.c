#include <CUnit/Basic.h>
#include <stdio.h>

// Объявления тестовых функций
void register_matrix_tests(void);
void test_file_operations(void);

int main(void) {
  // Инициализация реестра тестов
  if (CU_initialize_registry() != CUE_SUCCESS) {
    return CU_get_error();
  }

  // Регистрация всех тестовых сьют
  register_matrix_tests();

  // Сьют для интеграционных тестов
  CU_pSuite integrationSuite = CU_add_suite("Integration", NULL, NULL);
  if (integrationSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Запуск тестов
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
