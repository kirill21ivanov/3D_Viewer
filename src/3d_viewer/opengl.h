#ifndef OPENGL_H
#define OPENGL_H

#define GL_SILENCE_DEPRECATION

#include <QImage>
#include <QMessageBox>
#include <QOpenGLWidget>
#include <QSettings>
#include <QWidget>
#include <QtOpenGL>

#include "qgifimage.h"

extern "C" {
#include "../viewer.h"
}

class OpenGL : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit OpenGL(QWidget *parent = nullptr);

  void saveSettings();
  void loadSettings();

  QMessageBox warning;
  QString path, nameGif;
  data_t ObjData = {0, 0, 0, NULL, NULL};
  value_t coords;
  QSettings *settings;

  int vertexes = 0, edges = 0, checkTimer = 1;
  double red = 0, green = 0, blue = 0;
  double rbg = 0, gbg = 0, bbg = 0;
  double red_point = 0, green_point = 0, blue_point = 0;
  double point_size = 0;
  double normallize_coef = 0, scale = 0, value = 0;
  double x_coef = 0, y_coef = 0, z_coef = 0;
  double central_x = 0, central_y = 0, central_z = 0;
  int ProjectionType = 0, LineType = 0, PointType = 1;

  float zRot = 0, xRot = 0, yRot = 0;
  double start_x = 0, start_y = 0;

  double ZoomSize = 1;
  double SizeLine = 1;

  double move_x = 0, move_y = 0;
  double rotate_x = 0, rotate_y = 0, rotate_z = 0;

  void read_objFile();

  void record();

  QTimer *timer = NULL;

 private:
  void initializeGL() override;
  void paintGL() override;
  void draw();
  void draw_points();

  QPoint mPos;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

  QGifImage *gifImage = NULL;
  int indexTime = 0;

 private slots:
  void gif();

 signals:
  void stopRecord();
};

#endif  // OPENGL_H
