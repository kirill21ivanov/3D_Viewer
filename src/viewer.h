#ifndef SRC_VIEWER_H
#define SRC_VIEWER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 255
#define NUM "0123456789"

typedef struct data {
  unsigned int count_of_vertexes;
  unsigned int count_of_facets;
  unsigned int count_of_vertexes_in_facets;
  double *vertexes;
  unsigned int *polygons;
} data_t;

typedef struct value {
  double Xmax;
  double Xmin;
  double Ymax;
  double Ymin;
  double Zmax;
  double Zmin;
} value_t;

typedef enum { FALSE, TRUE } ret_val;

// парсер
void init_data(data_t *ObjData);
void init_CentralCoords(value_t *coords);
int ObjParser(char *path, data_t *ObjData, value_t *coords);
int count_of_spaces_in_f(char *str);
int FillArrays(char *file, data_t *ObjData, value_t *coords);
void find_center_coords(value_t *coords, double x, double y, double z);
int first_facete(int *first_value, char *str, int f, int *flag);
int next_facete(char *str, int f, int *copy_num, int *next_value);
void normallize_position(data_t *ObjData, value_t *coords,
                         double *normallize_coef, double *scale);
double max_coef(double x, double y, double z);
void central_positions(data_t *ObjData, value_t *coords, double x, double y,
                       double z);
void set_figure_in_center(data_t *ObjData, double *normallize_coef,
                          double *scale);
int calculate_edges(data_t *ObjData);

// аффинные преобразования
void move_coords(data_t *ObjData, double x, double y, double z);
void rotateViewer(data_t *ObjData, double degree, char coordinate);

#endif  //  SRC_VIEWER_H
