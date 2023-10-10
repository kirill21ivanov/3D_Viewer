#include "opengl.h"

OpenGL::OpenGL(QWidget* parent) : QOpenGLWidget(parent) {
  //    settings = new QSettings("School 21", "viewer_settings", this);
}

void OpenGL::saveSettings() {
  settings->setValue("are_settings", 1);
  settings->setValue("projection_type", ProjectionType);
  settings->setValue("point_type", PointType);
  settings->setValue("line_type", LineType);
  settings->setValue("line_color_red", red);
  settings->setValue("line_color_green", green);
  settings->setValue("line_color_blue", blue);
  settings->setValue("point_red", red_point);
  settings->setValue("point_green", green_point);
  settings->setValue("point_blue", blue_point);
  settings->setValue("background_red", rbg);
  settings->setValue("background_green", gbg);
  settings->setValue("background_blue", bbg);
}

void OpenGL::loadSettings() {
  if (settings->value("are_settings").toInt() == 1) {
    ProjectionType = settings->value("projection_type").toInt();
    PointType = settings->value("point_type").toInt();
    LineType = settings->value("line_type").toInt();
    red = settings->value("line_color_red").toInt();
    green = settings->value("line_color_green").toInt();
    blue = settings->value("line_color_blue").toInt();
    red_point = settings->value("point_red").toInt();
    green_point = settings->value("point_green").toInt();
    blue_point = settings->value("point_blue").toInt();
    rbg = settings->value("background_red").toInt();
    gbg = settings->value("background_green").toInt();
    bbg = settings->value("background_blue").toInt();
  }
}

void OpenGL::read_objFile() {
  QByteArray ba = path.toLocal8Bit();
  char* cpath = ba.data();
  init_data(&this->ObjData);
  init_CentralCoords(&this->coords);
  qDebug() << "ll";
  if (ObjParser(cpath, &this->ObjData, &this->coords)) {
    normallize_position(&this->ObjData, &this->coords, &normallize_coef,
                        &scale);
    edges = calculate_edges(&this->ObjData);
  } else {
    warning.setText("Incorrect file!");
    warning.setIcon(QMessageBox::Warning);
    warning.exec();
  }
}

void OpenGL::initializeGL() {
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
}

void OpenGL::paintGL() {
  glClearColor(rbg, gbg, bbg, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotated(rotate_x, 0, 0, rotate_x);
  glRotated(rotate_y, rotate_y, 0, 0);
  glRotated(rotate_z, 0, rotate_z, 0);
  glTranslated(move_x, move_y, 0);
  if (ProjectionType) {
    glFrustum(-1, 1, -1, 1, 3, 10);
    glTranslatef(0.0, 0.0, -5.0);
  }
  glVertexPointer(3, GL_DOUBLE, 0, ObjData.vertexes);
  glScaled(ZoomSize, ZoomSize, ZoomSize);
  glLineWidth(SizeLine);
  glEnableClientState(GL_VERTEX_ARRAY);
  if (PointType != 0) {
    draw_points();
  }
  draw();
  glDisableClientState(GL_VERTEX_ARRAY);
}

void OpenGL::draw() {
  glColor3d(red, green, blue);
  if (LineType == 1) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x00FF);
  } else {
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
  }
  glDrawElements(GL_LINES, this->ObjData.count_of_vertexes_in_facets * 2,
                 GL_UNSIGNED_INT, this->ObjData.polygons);
}

void OpenGL::draw_points() {
  glColor3d(red_point, green_point, blue_point);
  glPointSize(point_size);
  if (PointType == 1) glEnable(GL_POINT_SMOOTH);
  if (PointType == 2) glDisable(GL_POINT_SMOOTH);
  glDrawArrays(GL_POINTS, 1, this->ObjData.count_of_vertexes);
}

void OpenGL::mousePressEvent(QMouseEvent* mo) {
  mPos = mo->pos();
  start_x = mo->pos().x();
  start_y = mo->pos().y();
}

void OpenGL::mouseMoveEvent(QMouseEvent* mo) {
  xRot = 0.6 / M_PI * (mo->pos().y() - start_y);
  yRot = 0.6 / M_PI * (mo->pos().x() - start_x);
  rotateViewer(&ObjData, xRot, 'x');
  rotateViewer(&ObjData, yRot, 'y');
  start_x = mo->pos().x();
  start_y = mo->pos().y();
  update();
}

void OpenGL::record() {
  gifImage = new QGifImage();
  timer = new QTimer();
  checkTimer = 0;
  timer->start(100);
  connect(timer, SIGNAL(timeout()), this, SLOT(gif()));
}

void OpenGL::gif() {
  indexTime++;
  gifImage->addFrame(OpenGL::grabFramebuffer());
  if (indexTime == 50) {
    timer->stop();
    gifImage->save(nameGif);
    indexTime = 0;
    emit stopRecord();
    checkTimer = 1;
    delete timer;
    delete gifImage;
  }
}
