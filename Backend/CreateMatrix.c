#include "../headers/function.h"
#include "../headers/include.h"

void CreateMatrix(DATA *data_object, int *check) {
  data_object->object_matrix.rows = data_object->count_of_vertex + 1;
  data_object->object_matrix.cols = 3;
  data_object->object_matrix.matrix =
      (double **)calloc(data_object->object_matrix.rows, sizeof(double *));
  if (data_object->object_matrix.matrix == NULL) {
    (*check)++;
  } else {
    for (int i = 0; i < data_object->object_matrix.rows; ++i) {
      data_object->object_matrix.matrix[i] =
          (double *)calloc(data_object->object_matrix.cols, sizeof(double));
    }
  }
}

void FreeMatrix(MATRIX_3D *matrix3D_object) {
  if (matrix3D_object && matrix3D_object->matrix) {
    for (int i = 0; i < matrix3D_object->rows; ++i) {
      free(matrix3D_object->matrix[i]);
    }
    free(matrix3D_object->matrix);
    matrix3D_object->matrix = NULL;
    matrix3D_object->rows = 0;
    matrix3D_object->cols = 0;
  }
}
