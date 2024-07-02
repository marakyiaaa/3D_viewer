#include "../headers/function.h"
#include "../headers/include.h"

void move_affin(MATRIX_3D *A, double dx, double dy, double dz) {
  for (int i = 0; i < A->rows; i++) {
    A->matrix[i][0] += dx;
    A->matrix[i][1] += dy;
    A->matrix[i][2] += dz;
  }
}

void rotation_by_OX(MATRIX_3D *A, double angle) {
  for (int i = 0; i < A->rows; i++) {
    double temp_y = A->matrix[i][1];
    double temp_z = A->matrix[i][2];
    A->matrix[i][1] = cos(angle) * temp_y - sin(angle) * temp_z;
    A->matrix[i][2] = sin(angle) * temp_y + cos(angle) * temp_z;
  }
}

void rotation_by_OY(MATRIX_3D *A, double angle) {
  for (int i = 0; i < A->rows; i++) {
    double temp_x = A->matrix[i][0];
    double temp_z = A->matrix[i][2];
    A->matrix[i][0] = temp_x * cos(angle) + temp_z * sin(angle);
    A->matrix[i][2] = temp_x * (-sin(angle)) + temp_z * cos(angle);
  }
}

void rotation_by_OZ(MATRIX_3D *A, double angle) {
  for (int i = 0; i < A->rows; i++) {
    double temp_x = A->matrix[i][0];
    double temp_y = A->matrix[i][1];
    A->matrix[i][0] = temp_x * cos(angle) - temp_y * sin(angle);
    A->matrix[i][1] = temp_x * sin(angle) + temp_y * cos(angle);
  }
}
