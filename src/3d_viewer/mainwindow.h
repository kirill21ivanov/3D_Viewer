#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>

extern "C" {
#include "../viewer.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  QString str;
  int line = 0;

 private slots:
  void on_choose_file_clicked();
  void on_png_clicked();
  void on_bmp_clicked();
  void on_jpg_clicked();
  void on_save_gif_clicked();
  void changeColor();

  void on_dotted_clicked(bool checked);
  void on_solid_clicked();
  void on_perspective_clicked(bool checked);
  void on_orthogonal_clicked();

  void on_apply_color_clicked();
  void on_apply_background_clicked();

  void on_SliderSize_valueChanged(int value);
  void on_Size_line_valueChanged(int value);

  void on_x_rotate_valueChanged(int x);
  void on_y_rotate_valueChanged(int y);
  void on_z_rotate_valueChanged(int z);

  void on_Size_point_valueChanged(int size);

  void on_x_move_valueChanged(int x);
  void on_y_move_valueChanged(int y);

  void on_sbros_clicked();

  void on_krug_clicked();
  void on_kvadrat_clicked();
  void on_without_points_clicked();

  void on_apply_pointColor_clicked();

 private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
