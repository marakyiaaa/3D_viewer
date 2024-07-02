#ifndef C8_3DVIEWER_V1_0_1_STRUCT_H
#define C8_3DVIEWER_V1_0_1_STRUCT_H

typedef struct {
  double **matrix;
  int rows;
  int cols;
} MATRIX_3D;

typedef struct {
  int *vertexes;
  int numbers_vertex_and_facets;
  int *count_facets_in_line;
} POLYGON;

typedef struct {
  unsigned int count_of_vertex;
  unsigned int count_of_facets;
  MATRIX_3D object_matrix;
  POLYGON *object_polygon;
} DATA;

#endif  // C8_3DVIEWER_V1_0_1_STRUCT_H
