#include <check.h>

#include "viewer.h"

// непустые файлы

START_TEST(simple_test_1) {
  data_t data = {0, 0, 0, NULL, NULL};
  value_t value;
  char *path = "./pictures/cat.txt";
  init_data(&data);
  init_CentralCoords(&value);
  int res = ObjParser(path, &data, &value);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(data.count_of_vertexes, 1136);
  ck_assert_int_eq(data.count_of_facets, 2082);
  free(data.vertexes);
  free(data.polygons);
}
END_TEST

START_TEST(simple_test_2) {
  data_t data = {0, 0, 0, NULL, NULL};
  value_t value;
  char *path = "./pictures/cub.txt";
  init_data(&data);
  init_CentralCoords(&value);
  int res = ObjParser(path, &data, &value);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(data.count_of_vertexes, 8);
  ck_assert_int_eq(data.count_of_facets, 12);
  free(data.vertexes);
  free(data.polygons);
}
END_TEST

START_TEST(simple_test_3) {
  data_t data = {0, 0, 0, NULL, NULL};
  value_t value;
  char *path = "./pictures/eyeball.txt";
  init_data(&data);
  init_CentralCoords(&value);
  int res = ObjParser(path, &data, &value);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(data.count_of_vertexes, 9667);
  ck_assert_int_eq(data.count_of_facets, 9344);
  free(data.vertexes);
  free(data.polygons);
}
END_TEST

START_TEST(simple_test_4) {
  data_t data = {0, 0, 0, NULL, NULL};
  value_t value;
  char *path = "./pictures/octopus.txt";
  init_data(&data);
  init_CentralCoords(&value);
  int res = ObjParser(path, &data, &value);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(data.count_of_vertexes, 6424);
  ck_assert_int_eq(data.count_of_facets, 6354);
  free(data.vertexes);
  free(data.polygons);
}
END_TEST

// несуществующий файл

START_TEST(simple_test_5) {
  data_t data = {0, 0, 0, NULL, NULL};
  value_t value;
  char *path = "./pictures/dont_exist.txt";
  init_data(&data);
  init_CentralCoords(&value);
  int res = ObjParser(path, &data, &value);
  ck_assert_int_eq(res, 0);
  free(data.vertexes);
  free(data.polygons);
}
END_TEST

// пустой файл

START_TEST(simple_test_6) {
  data_t data = {0, 0, 0, NULL, NULL};
  value_t value;
  char *path = "./pictures/elephant.txt";
  init_data(&data);
  init_CentralCoords(&value);
  int res = ObjParser(path, &data, &value);
  ck_assert_int_eq(res, 0);
  free(data.vertexes);
  free(data.polygons);
}
END_TEST

int main(void) {
  int number_failed;
  Suite *s = suite_create("viewer");
  TCase *viewer_h = tcase_create("viewer_h");
  SRunner *sr = srunner_create(s);

  suite_add_tcase(s, viewer_h);

  tcase_add_test(viewer_h, simple_test_1);
  tcase_add_test(viewer_h, simple_test_2);
  tcase_add_test(viewer_h, simple_test_3);
  tcase_add_test(viewer_h, simple_test_4);
  tcase_add_test(viewer_h, simple_test_5);
  tcase_add_test(viewer_h, simple_test_6);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (!number_failed) ? EXIT_SUCCESS : EXIT_FAILURE;
}