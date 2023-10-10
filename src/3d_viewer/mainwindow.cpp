#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setFixedSize(this->size());
  ui->openGLWidget->settings =
      new QSettings("School 21", "viewer_settings", this);
  ui->lines->setTitle("");
  ui->projection->setTitle("");
  ui->points->setTitle("");
}

MainWindow::~MainWindow() {
  init_data(&(this->ui->openGLWidget->ObjData));
  init_CentralCoords(&(this->ui->openGLWidget->coords));
  this->ui->openGLWidget->saveSettings();
  delete ui->openGLWidget->settings;
  delete ui;
}

void MainWindow::on_choose_file_clicked() {
  str = QFileDialog::getOpenFileName(this, "Choose a picture",
                                     "../../../../pictures", "OBJ (*.obj)");
  if (!str.isEmpty()) {
    this->ui->openGLWidget->path = str;
    this->ui->path_to_file->setText(str);
    if (ui->openGLWidget->settings->value("are_settings").toInt() != 1) {
      this->ui->openGLWidget->red = this->ui->red_color->text().toDouble();
      this->ui->openGLWidget->green = this->ui->green_color->text().toDouble();
      this->ui->openGLWidget->blue = this->ui->blue_color->text().toDouble();
      this->ui->openGLWidget->rbg = this->ui->red_back->text().toDouble();
      this->ui->openGLWidget->gbg = this->ui->green_back->text().toDouble();
      this->ui->openGLWidget->bbg = this->ui->blue_back->text().toDouble();
      this->ui->openGLWidget->red_point =
          this->ui->red_point->text().toDouble();
      this->ui->openGLWidget->green_point =
          this->ui->green_point->text().toDouble();
      this->ui->openGLWidget->blue_point =
          this->ui->blue_point->text().toDouble();
    } else {
      this->ui->openGLWidget->loadSettings();
    }

    this->ui->openGLWidget->read_objFile();

    this->ui->vertexes_count->setText(
        QString::number(this->ui->openGLWidget->ObjData.count_of_vertexes));
    this->ui->edges_count->setText(
        QString::number(this->ui->openGLWidget->edges));
    this->ui->openGLWidget->loadSettings();
    this->ui->openGLWidget->update();
    this->ui->path_to_file->update();
  }
}

void MainWindow::on_png_clicked() {
  QString dirname = QFileDialog::getExistingDirectory(this, "Выберите папку",
                                                      "../../../../screen/");

  if (!dirname.isEmpty()) {
    QPixmap screenshot = ui->openGLWidget->grab();

    if (!screenshot.isNull()) {
      QImage image = screenshot.toImage();

      QString timestamp =
          QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
      QString filename = dirname + "/screenshot_" + timestamp + ".png";

      image.save(filename, "PNG");
    }
  }
}

void MainWindow::on_jpg_clicked() {
  QString dirname = QFileDialog::getExistingDirectory(this, "Выберите папку",
                                                      "../../../../screen/");

  if (!dirname.isEmpty()) {
    QPixmap screenshot = ui->openGLWidget->grab();

    if (!screenshot.isNull()) {
      QImage image = screenshot.toImage();

      QString timestamp =
          QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
      QString filename = dirname + "/screenshot_" + timestamp + ".jpeg";

      image.save(filename, "JPEG");
    }
  }
}

void MainWindow::on_bmp_clicked() {
  QString dirname = QFileDialog::getExistingDirectory(this, "Выберите папку",
                                                      "../../../../screen/");

  if (!dirname.isEmpty()) {
    QPixmap screenshot = ui->openGLWidget->grab();

    if (!screenshot.isNull()) {
      QImage image = screenshot.toImage();

      QString timestamp =
          QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
      QString filename = dirname + "/screenshot_" + timestamp + ".bmp";

      image.save(filename, "BMP");
    }
  }
}

void MainWindow::on_dotted_clicked(bool checked) {
  this->ui->openGLWidget->LineType = checked;
  this->ui->openGLWidget->update();
}

void MainWindow::on_solid_clicked() {
  this->ui->openGLWidget->LineType = 0;
  this->ui->openGLWidget->update();
}

void MainWindow::on_perspective_clicked(bool checked) {
  this->ui->openGLWidget->ProjectionType = checked;
  this->ui->openGLWidget->update();
}

void MainWindow::on_orthogonal_clicked() {
  this->ui->openGLWidget->ProjectionType = 0;
  this->ui->openGLWidget->update();
}

void MainWindow::on_apply_color_clicked() {
  this->ui->openGLWidget->red = this->ui->red_color->text().toDouble();
  this->ui->openGLWidget->green = this->ui->green_color->text().toDouble();
  this->ui->openGLWidget->blue = this->ui->blue_color->text().toDouble();
  this->ui->openGLWidget->update();
}

void MainWindow::on_apply_background_clicked() {
  this->ui->openGLWidget->rbg = this->ui->red_back->text().toDouble();
  this->ui->openGLWidget->gbg = this->ui->green_back->text().toDouble();
  this->ui->openGLWidget->bbg = this->ui->blue_back->text().toDouble();
  this->ui->openGLWidget->update();
}

void MainWindow::on_SliderSize_valueChanged(int value) {
  ui->openGLWidget->ZoomSize = (double)value / 100;
  ui->openGLWidget->update();
}

void MainWindow::on_Size_line_valueChanged(int value) {
  ui->openGLWidget->SizeLine = value / 100;
  ui->openGLWidget->update();
}

void MainWindow::on_x_rotate_valueChanged(int x) {
  ui->openGLWidget->rotate_x = x;
  ui->openGLWidget->update();
}

void MainWindow::on_y_rotate_valueChanged(int y) {
  ui->openGLWidget->rotate_y = y;
  ui->openGLWidget->update();
}

void MainWindow::on_z_rotate_valueChanged(int z) {
  ui->openGLWidget->rotate_z = z;
  ui->openGLWidget->update();
}

void MainWindow::on_Size_point_valueChanged(int size) {
  ui->openGLWidget->point_size = size;
  ui->openGLWidget->update();
}

void MainWindow::on_x_move_valueChanged(int x) {
  ui->openGLWidget->move_x = (double)x / 50;
  ui->openGLWidget->update();
}

void MainWindow::on_y_move_valueChanged(int y) {
  ui->openGLWidget->move_y = (double)y / 50;
  ui->openGLWidget->update();
}

void MainWindow::on_sbros_clicked() {
  ui->Size_line->setValue(0);
  ui->Size_point->setValue(0);
  ui->SliderSize->setValue(0);
  ui->x_move->setValue(0);
  ui->y_move->setValue(0);
  ui->z_rotate->setValue(0);
  ui->x_rotate->setValue(0);
  ui->y_rotate->setValue(0);
  ui->openGLWidget->rotate_z = 0;
  ui->openGLWidget->rotate_y = 0;
  ui->openGLWidget->rotate_x = 0;
  ui->openGLWidget->move_y = 0;
  ui->openGLWidget->move_x = 0;
  ui->openGLWidget->ZoomSize = 140 / 100;
  ui->openGLWidget->SizeLine = 1;
  ui->openGLWidget->point_size = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_krug_clicked() {
  this->ui->openGLWidget->PointType = 1;
  this->ui->openGLWidget->update();
}

void MainWindow::on_kvadrat_clicked() {
  this->ui->openGLWidget->PointType = 2;
  this->ui->openGLWidget->update();
}

void MainWindow::on_without_points_clicked() {
  this->ui->openGLWidget->PointType = 0;
  this->ui->openGLWidget->update();
}

void MainWindow::on_apply_pointColor_clicked() {
  this->ui->openGLWidget->red_point = this->ui->red_point->text().toDouble();
  this->ui->openGLWidget->green_point =
      this->ui->green_point->text().toDouble();
  this->ui->openGLWidget->blue_point = this->ui->blue_point->text().toDouble();
  this->ui->openGLWidget->update();
}

void MainWindow::on_save_gif_clicked() {
  QString name = QFileDialog::getSaveFileName(
      this, "Save as...", "../saved/name.gif", tr("GIF (*.gif)"));
  if (!name.isEmpty() && this->ui->openGLWidget->checkTimer == 1) {
    ui->save_gif->setStyleSheet("background-color:#FF0000");
    this->ui->openGLWidget->nameGif = name;
    this->ui->openGLWidget->record();
  }
}

void MainWindow::changeColor() {
  this->ui->save_gif->setStyleSheet(
      "QPushButton {background-color: rgb(102, 192, 220);color:white;} "
      "QPushButton:hover{background-color: rgb(152, 185, 255);color:rgb(78, "
      "120, 254);} ");
}
