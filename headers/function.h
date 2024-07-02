#ifndef C8_3DVIEWER_V1_0_1_FUNCTION_H
#define C8_3DVIEWER_V1_0_1_FUNCTION_H

#include "struct.h"

int ParsingFile(char *filename, DATA *data_object);
int CountVertexAndFacets(char *filename, DATA *data_object);
void CreateMatrix(DATA *data_object, int *check);
void ParsingFacets(char *line, DATA *data_object, int *status,
                   unsigned int *vertexes_size, unsigned int *vertex_index,
                   int *f, unsigned int *count_block);
void ParsingVertex(DATA *data_object, int i, double number1, double number2,
                   double number3);
void FreeMatrix(MATRIX_3D *matrix3D_object);
void FreePolygon(POLYGON *polygon);

void move_affin(MATRIX_3D *A, double dx, double dy, double dz);
void rotation_by_OX(MATRIX_3D *A, double angle);
void rotation_by_OY(MATRIX_3D *A, double angle);
void rotation_by_OZ(MATRIX_3D *A, double angle);
void scale_affine(MATRIX_3D *A, double sx, double sy, double sz);

#endif  // C8_3DVIEWER_V1_0_1_FUNCTION_H
