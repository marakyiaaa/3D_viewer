#include "../headers/define.h"
#include "../headers/function.h"
#include "../headers/include.h"

int ParsingFile(char *filename, DATA *data_object) {
  char *line = NULL;
  double number1 = 0.0, number2 = 0.0, number3 = 0.0;
  int i = 0, f = 0, status = TRUE;
  unsigned int vertexes_size = ALLOCATE, vertex_index = 0, count_block = 0;
  size_t len = 0;
  ssize_t read;
  data_object->object_polygon = calloc(1, sizeof(POLYGON));
  if (data_object->object_polygon == NULL) {
    status = FALSE;
  }
  data_object->object_polygon->vertexes = calloc(vertexes_size, sizeof(int));
  data_object->object_polygon->count_facets_in_line =
      calloc(vertexes_size, sizeof(int));
  if (data_object->object_polygon->vertexes == NULL ||
      data_object->object_polygon->count_facets_in_line == NULL) {
    free(data_object->object_polygon->vertexes);
    free(data_object->object_polygon->count_facets_in_line);
    free(data_object->object_polygon);
    status = FALSE;
  }
  FILE *file_object = fopen(filename, "r");
  if (!file_object) {
    free(data_object->object_polygon->vertexes);
    free(data_object->object_polygon->count_facets_in_line);
    free(data_object->object_polygon);
    status = FALSE;
  } else {
    while ((read = getline(&line, &len, file_object)) != -1) {
      if (sscanf(line, "v %lf %lf %lf", &number1, &number2, &number3) == 3) {
        ParsingVertex(data_object, i, number1, number2, number3);
        i++;
        count_block++;
      } else if (line[0] == 'f') {
        ParsingFacets(line, data_object, &status, &vertexes_size, &vertex_index,
                      &f, &count_block);
        f++;
      }
    }
    f++;
    vertex_index++;
    data_object->object_polygon->count_facets_in_line[f] = 0;
    data_object->object_polygon->vertexes[vertex_index] = 0;
  }
  fclose(file_object);
  free(line);
  return status;
}

void ParsingVertex(DATA *data_object, int i, double number1, double number2,
                   double number3) {
  data_object->object_matrix.matrix[i][0] = number1;
  data_object->object_matrix.matrix[i][1] = number2;
  data_object->object_matrix.matrix[i][2] = number3;
}

void ParsingFacets(char *line, DATA *data_object, int *status,
                   unsigned int *vertexes_size, unsigned int *vertex_index,
                   int *f, unsigned int *count_block) {
  int i = 1, l = 0, buffer_size = ALLOCATE_LENGTH;
  char *buffer = calloc(buffer_size, sizeof(char));
  if (buffer == NULL) {
    *status = FALSE;
  }
  while (line[i] != '\0' && line[i] != '\n' && *status) {
    if (line[i - 1] == ' ' && (isdigit(line[i]) || line[i] == '-')) {
      int j = 0;
      while (isdigit(line[i]) || line[i] == '-') {
        if (j >= buffer_size - 1) {
          buffer_size *= 2;
          char *new_buffer = realloc(buffer, buffer_size * sizeof(char));
          buffer = new_buffer;
        }
        buffer[j++] = line[i++];
      }
      l++;
      buffer[j] = '\0';
      if (*vertex_index >= *vertexes_size) {
        *vertexes_size *= 2;
        int *new_vertexes = realloc(data_object->object_polygon->vertexes,
                                    *vertexes_size * sizeof(int));
        data_object->object_polygon->vertexes = new_vertexes;
      }
      if ((data_object->object_polygon->vertexes[(*vertex_index)++] =
               atoi(buffer)) < 0) {
        data_object->object_polygon->vertexes[(*vertex_index) - 1] +=
            *count_block + 1;
      }
    } else {
      i++;
    }
    data_object->object_polygon->count_facets_in_line[*f] = l;
  }
  data_object->object_polygon->numbers_vertex_and_facets = *vertex_index;
  free(buffer);
}

void scale_affine(MATRIX_3D *A, double sx, double sy, double sz) {
  for (int i = 0; i < A->rows; i++) {
    A->matrix[i][0] *= sx;
    A->matrix[i][1] *= sy;
    A->matrix[i][2] *= sz;
  }
}