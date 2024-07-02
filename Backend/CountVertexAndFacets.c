#include "../headers/define.h"
#include "../headers/function.h"
#include "../headers/include.h"

int CountVertexAndFacets(char *filename, DATA *data_object) {
  int status = TRUE;
  char *line = NULL;
  size_t len = 0;
  ssize_t read = 0;
  FILE *file_object = fopen(filename, "r");
  if (!file_object) {
    status = FALSE;
    perror("");
  } else {
    while (!feof(file_object) &&
           (read = getline(&line, &len, file_object)) != -1) {
      (void)read;
      if (line[0] == 'v' && line[1] == ' ')
        data_object->count_of_vertex++;
      else if (line[0] == 'f' && line[1] == ' ')
        data_object->count_of_facets++;
    }
    fclose(file_object);
    free(line);
  }
  return status;
}
