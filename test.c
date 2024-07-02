#include <check.h>

#include "headers/define.h"
#include "headers/function.h"
#include "headers/include.h"
#include "headers/struct.h"

#define M_PI 3.14159265358979323846264338327950288

START_TEST(test_move_affin) {
  int check = 0;
  DATA data = {3, 0, {NULL, 0, 0}, NULL};
  CreateMatrix(&data, &check);

  for (int i = 0; i < data.object_matrix.rows; ++i) {
    data.object_matrix.matrix[i][0] = i;
    data.object_matrix.matrix[i][1] = i * 2;
    data.object_matrix.matrix[i][2] = i * 3;
  }
  double dx = 1.0, dy = 2.0, dz = 3.0;
  double matrix_res[4][3] = {{1, 2, 3}, {2, 4, 6}, {3, 6, 9}, {4, 8, 12}};

  move_affin(&data.object_matrix, dx, dy, dz);

  for (int i = 0; i < data.object_matrix.rows; i++) {
    for (int j = 0; j < data.object_matrix.cols; j++) {
      ck_assert_double_eq(data.object_matrix.matrix[i][j], matrix_res[i][j]);
    }
  }
  FreeMatrix(&data.object_matrix);
  FreePolygon(data.object_polygon);
}
END_TEST

START_TEST(test_rotation_by_OX) {
  int check = 0;
  DATA data = {3, 0, {NULL, 0, 0}, NULL};
  CreateMatrix(&data, &check);
  for (int i = 0; i < data.object_matrix.rows; ++i) {
    data.object_matrix.matrix[i][0] = i;
    data.object_matrix.matrix[i][1] = i * 2;
    data.object_matrix.matrix[i][2] = i * 3;
  }

  double angle = M_PI / 4;
  double matrix_res[4][3] = {{0, 0, 0},
                             {1, -0.707107, 3.535533},
                             {2, -1.414214, 7.071068},
                             {3, -2.121320, 10.606602}};

  rotation_by_OX(&data.object_matrix, angle);

  for (int i = 0; i < data.object_matrix.rows; i++) {
    for (int j = 0; j < data.object_matrix.cols; j++) {
      ck_assert_double_eq_tol(data.object_matrix.matrix[i][j], matrix_res[i][j],
                              1e-6);
    }
  }
  FreeMatrix(&data.object_matrix);
  FreePolygon(data.object_polygon);
}
END_TEST

START_TEST(test_rotation_by_OY) {
  int check = 0;
  DATA data = {3, 0, {NULL, 0, 0}, NULL};
  CreateMatrix(&data, &check);

  for (int i = 0; i < data.object_matrix.rows; ++i) {
    data.object_matrix.matrix[i][0] = i;
    data.object_matrix.matrix[i][1] = i * 2;
    data.object_matrix.matrix[i][2] = i * 3;
  }

  double angle = M_PI / 4;
  double matrix_res[4][3] = {{0, 0, 0},
                             {2.828427, 2, 1.414214},
                             {5.656854, 4, 2.828427},
                             {8.485281, 6, 4.242641}};

  rotation_by_OY(&data.object_matrix, angle);

  for (int i = 0; i < data.object_matrix.rows; i++) {
    for (int j = 0; j < data.object_matrix.cols; j++) {
      ck_assert_double_eq_tol(data.object_matrix.matrix[i][j], matrix_res[i][j],
                              1e-6);
    }
  }
  FreeMatrix(&data.object_matrix);
  FreePolygon(data.object_polygon);
}
END_TEST

START_TEST(test_rotation_by_OZ) {
  int check = 0;
  DATA data = {3, 0, {NULL, 0, 0}, NULL};
  CreateMatrix(&data, &check);

  for (int i = 0; i < data.object_matrix.rows; ++i) {
    data.object_matrix.matrix[i][0] = i;
    data.object_matrix.matrix[i][1] = i * 2;
    data.object_matrix.matrix[i][2] = i * 3;
  }

  double angle = M_PI / 4;
  double matrix_res[4][3] = {{0, 0, 0},
                             {-0.707107, 2.121320, 3},
                             {-1.414214, 4.242641, 6},
                             {-2.121320, 6.363961, 9}};

  rotation_by_OZ(&data.object_matrix, angle);

  for (int i = 0; i < data.object_matrix.rows; i++) {
    for (int j = 0; j < data.object_matrix.cols; j++) {
      ck_assert_double_eq_tol(data.object_matrix.matrix[i][j], matrix_res[i][j],
                              1e-6);
    }
  }
  FreeMatrix(&data.object_matrix);
  FreePolygon(data.object_polygon);
}
END_TEST

START_TEST(test_scale_affin) {
  DATA data = {3, 0, {NULL, 0, 0}, NULL};
  data.object_matrix.rows = 3;
  data.object_matrix.cols = 3;
  data.object_matrix.matrix =
      (double**)malloc(data.object_matrix.rows * sizeof(double*));
  for (int i = 0; i < data.object_matrix.rows; ++i) {
    data.object_matrix.matrix[i] =
        (double*)malloc(data.object_matrix.cols * sizeof(double));
  }

  for (int i = 0; i < data.object_matrix.rows; ++i) {
    data.object_matrix.matrix[i][0] = i;
    data.object_matrix.matrix[i][1] = i * 2;
    data.object_matrix.matrix[i][2] = i * 3;
  }

  double sx = 2.0, sy = 0.5, sz = 3.0;
  scale_affine(&data.object_matrix, sx, sy, sz);

  double matrix_res[3][3] = {
      {0.0, 0.0, 0.0}, {2.0, 1.0, 9.0}, {4.0, 2.0, 18.0}};

  for (int i = 0; i < data.object_matrix.rows; i++) {
    for (int j = 0; j < data.object_matrix.cols; j++) {
      ck_assert_double_eq(data.object_matrix.matrix[i][j], matrix_res[i][j]);
    }
  }
  FreeMatrix(&data.object_matrix);
  FreePolygon(data.object_polygon);
}
END_TEST

START_TEST(test_CountVertexAndFacets) {
  FILE* tmp_file = fopen("test_file.txt", "w");
  fprintf(tmp_file, "v 1 2 3\n");
  fprintf(tmp_file, "v 4 5 6\n");
  fprintf(tmp_file, "f 1 2 3\n");
  fprintf(tmp_file, "f 4 5 6\n");
  fclose(tmp_file);

  DATA data_object = {0, 0, {NULL, 0, 0}, NULL};
  int result = CountVertexAndFacets("test_file.txt", &data_object);

  ck_assert_int_eq(result, TRUE);
  ck_assert_int_eq(data_object.count_of_vertex, 2);
  ck_assert_int_eq(data_object.count_of_facets, 2);

  FreeMatrix(&data_object.object_matrix);
  FreePolygon(data_object.object_polygon);

  remove("test_file.txt");
}
END_TEST

START_TEST(test_file_not_exists) {
  DATA data_object = {0, 0, {NULL, 0, 0}, NULL};
  int result = CountVertexAndFacets("non_existing_file.obj", &data_object);
  ck_assert_int_eq(result, FALSE);
}
END_TEST

START_TEST(test_empty_file) {
  FILE* tmp_file = fopen("empty_file.obj", "w");

  DATA* data_object = malloc(sizeof(DATA));
  data_object->object_polygon = NULL;
  data_object->count_of_vertex = 0;
  data_object->count_of_facets = 0;

  int result = CountVertexAndFacets("empty_file.obj", data_object);
  CreateMatrix(data_object, &result);
  result = ParsingFile("empty_file.obj", data_object);
  ck_assert_int_eq(result, TRUE);
  ck_assert_int_eq(data_object->count_of_vertex, 0);
  ck_assert_int_eq(data_object->count_of_facets, 0);

  FreeMatrix(&data_object->object_matrix);
  FreePolygon(data_object->object_polygon);
  free(data_object);
  fclose(tmp_file);
  remove("empty_file.obj");
}
END_TEST

START_TEST(test_vertices_and_facets_file) {
  DATA* data_object = malloc(sizeof(DATA));
  data_object->object_polygon = NULL;
  data_object->count_of_vertex = 0;
  data_object->count_of_facets = 0;

  int result = CountVertexAndFacets("from_test.txt", data_object);

  CreateMatrix(data_object, &result);

  result = ParsingFile("from_test.txt", data_object);
  ck_assert_int_eq(result, TRUE);
  ck_assert_int_eq(data_object->count_of_vertex, 4);
  ck_assert_int_eq(data_object->count_of_facets, 3);

  FreeMatrix(&data_object->object_matrix);
  FreePolygon(data_object->object_polygon);
  free(data_object);
}
END_TEST

int main() {
  Suite* suite = suite_create("TEST");
  TCase* tCase = tcase_create("TEST");
  SRunner* sRunner = srunner_create(suite);

  double FAILED = 0;
  suite_add_tcase(suite, tCase);

  tcase_add_test(tCase, test_move_affin);
  tcase_add_test(tCase, test_rotation_by_OX);
  tcase_add_test(tCase, test_rotation_by_OY);
  tcase_add_test(tCase, test_rotation_by_OZ);
  tcase_add_test(tCase, test_scale_affin);
  tcase_add_test(tCase, test_CountVertexAndFacets);
  tcase_add_test(tCase, test_file_not_exists);
  tcase_add_test(tCase, test_empty_file);
  tcase_add_test(tCase, test_vertices_and_facets_file);

  srunner_run_all(sRunner, CK_NORMAL);
  FAILED = srunner_ntests_failed(sRunner);
  srunner_free(sRunner);
  return !FAILED ? EXIT_SUCCESS : EXIT_FAILURE;
}