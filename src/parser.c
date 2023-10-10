#include "viewer.h"

void init_data(data_t *data) {
  if (data != NULL) {
    if (data->vertexes != NULL) {
      free(data->vertexes);
      data->vertexes = NULL;
    }
    if (data->polygons != NULL) {
      free(data->polygons);
      data->polygons = NULL;
    }
    data->count_of_vertexes_in_facets = 0;
    data->count_of_vertexes = 0;
    data->count_of_facets = 0;
  }
}
void init_CentralCoords(value_t *coords) {
  coords->Xmax = 0;
  coords->Xmin = 0;
  coords->Ymax = 0;
  coords->Ymin = 0;
  coords->Zmax = 0;
  coords->Zmin = 0;
}

int ObjParser(char *path, data_t *ObjData, value_t *coords) {
  int res = TRUE;
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    return FALSE;
  } else {
    char *str = calloc(MAX, sizeof(char));
    while (fgets(str, MAX, file) != NULL) {
      if (!strncmp(str, "v ", 2)) {
        ObjData->count_of_vertexes++;
      } else if (!strncmp(str, "f ", 2)) {
        ObjData->count_of_facets++;
        ObjData->count_of_vertexes_in_facets += count_of_spaces_in_f(str);
      }
    }
    res = (ObjData->count_of_vertexes < 3) ? FALSE
                                           : FillArrays(path, ObjData, coords);
    free(str);
  }
  fclose(file);
  return res;
}

int count_of_spaces_in_f(char *str) {
  int spaces = 0;
  for (; *str != '\n' && *str != '\0'; str++) {
    if (*str == ' ') spaces++;
  }
  return spaces;
}

int FillArrays(char *path, data_t *ObjData, value_t *coords) {
  int res = TRUE, i = 0, j = 0;
  ObjData->vertexes = calloc(ObjData->count_of_vertexes * 3, sizeof(double));
  ObjData->polygons =
      calloc(ObjData->count_of_vertexes_in_facets * 2, sizeof(unsigned int));
  if (ObjData->vertexes == NULL || ObjData->polygons == NULL)
    return FALSE;
  else {
    FILE *file = fopen(path, "r");
    char *str = calloc(MAX, sizeof(char));
    while (fgets(str, MAX, file)) {
      if (!strncmp(str, "v ", 2)) {
        double x, y, z;
        sscanf(str, "v %lf %lf %lf", &x, &y, &z);
        ObjData->vertexes[i++] = x;
        ObjData->vertexes[i++] = y;
        ObjData->vertexes[i++] = z;
        find_center_coords(coords, x, y, z);
      } else if (!strncmp(str, "f ", 2)) {
        int len = strlen(str);
        int first_value = 0, next_value = 0, flag = 0;
        for (int f = 1; f < len; f++) {
          if (str[f] == ' ' && strchr(NUM, str[f + 1])) {
            f++;
            int copy_num = 0;
            ObjData->polygons[j++] =
                (!flag) ? first_facete(&first_value, str, f, &flag)
                        : next_facete(str, f, &copy_num, &next_value);
            if (copy_num) {
              ObjData->polygons[j++] = next_value;
            }
          }
        }
        ObjData->polygons[j++] = first_value;
      }
    }
    free(str);
    fclose(file);
  }
  return res;
}

void find_center_coords(value_t *coords, double x, double y, double z) {
  coords->Xmax = fmax(x, coords->Xmax);
  coords->Ymax = fmax(y, coords->Ymax);
  coords->Zmax = fmax(z, coords->Zmax);
  coords->Xmin = fmin(x, coords->Xmin);
  coords->Ymin = fmin(y, coords->Ymin);
  coords->Zmin = fmin(z, coords->Zmin);
}

int first_facete(int *first_value, char *str, int f, int *flag) {
  int val = 0;
  val = atof(str + f) - 1;
  *first_value = val;
  *flag = 1;
  return val;
}

int next_facete(char *str, int f, int *copy_num, int *next_value) {
  *next_value = atof(str + f) - 1;
  *copy_num = 1;
  return *next_value;
}

void normallize_position(data_t *ObjData, value_t *coords,
                         double *normallize_coef, double *scale) {
  double x_coef = coords->Xmax - coords->Xmin;
  double y_coef = coords->Ymax - coords->Ymin;
  double z_coef = coords->Zmax - coords->Zmin;

  *normallize_coef = max_coef(x_coef, y_coef, z_coef);
  central_positions(ObjData, coords, x_coef, y_coef, z_coef);
  set_figure_in_center(ObjData, normallize_coef, scale);
}

double max_coef(double x, double y, double z) {
  double max_value;
  max_value = fmax(x, y);
  max_value = max_value > z ? max_value : z;
  return max_value;
}

void central_positions(data_t *ObjData, value_t *coords, double x, double y,
                       double z) {
  double central_x = coords->Xmin + x / 2;
  double central_y = coords->Ymin + y / 2;
  double central_z = coords->Zmin + z / 2;

  for (unsigned int i = 0; i < (ObjData->count_of_vertexes * 3); i += 3) {
    ObjData->vertexes[i] -= central_x;
    ObjData->vertexes[i + 1] -= central_y;
    ObjData->vertexes[i + 2] -= central_z;
  }
}

void set_figure_in_center(data_t *ObjData, double *normallize_coef,
                          double *scale) {
  double value = 0.5;
  *scale = (value - (value * (-0.7))) / *normallize_coef;
  for (unsigned int i = 0; i < (ObjData->count_of_vertexes * 3); i += 3) {
    ObjData->vertexes[i] *= *scale;
    ObjData->vertexes[i + 1] *= *scale;
    ObjData->vertexes[i + 2] *= *scale;
  }
}

// формула Эйлера для подсчета ребер
int calculate_edges(data_t *ObjData) {
  int res = 0;
  res = ObjData->count_of_vertexes + ObjData->count_of_facets - 2;
  return res;
}
