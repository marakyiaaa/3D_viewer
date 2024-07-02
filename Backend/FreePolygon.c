#include "../headers/function.h"
#include "../headers/include.h"

void FreePolygon(POLYGON *polygon) {
  if (polygon) {
    if (polygon->vertexes) {
      free(polygon->vertexes);
    }
    if (polygon->count_facets_in_line) {
      free(polygon->count_facets_in_line);
    }
    free(polygon);
  }
}