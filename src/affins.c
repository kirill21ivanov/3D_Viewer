#include "viewer.h"

void move_coords(data_t *ObjData, double x, double y, double z) {
  for (unsigned int i = 0; i < ObjData->count_of_vertexes * 3; i += 3) {
    ObjData->vertexes[i] += x;
    ObjData->vertexes[i + 1] += y;
    ObjData->vertexes[i + 2] += z;
  }
}

void rotateViewer(data_t *ObjData, double degree, char coordinate) {
  double radian = degree * M_PI / 180;
  for (unsigned int index = 0; index < ObjData->count_of_vertexes * 3;
       index += 3) {
    double x = ObjData->vertexes[index];
    double y = ObjData->vertexes[index + 1];
    double z = ObjData->vertexes[index + 2];
    switch (coordinate) {
      case 'x':
        ObjData->vertexes[index + 1] = y * cos(radian) - sin(radian) * z;
        ObjData->vertexes[index + 2] = y * sin(radian) + cos(radian) * z;
        break;
      case 'y':
        ObjData->vertexes[index] = x * cos(radian) + sin(radian) * z;
        ObjData->vertexes[index + 2] = -x * sin(radian) + cos(radian) * z;
        break;
      default:;
    }
  }
}