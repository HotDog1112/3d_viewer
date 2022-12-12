#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMovie>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>

#include "OpenGL/gltypes.h"
#include "QtGifImage-master/src/gifimage/qgifimage.h"

extern "C" {
#include "s21_viewer_3d.h"
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
  void new_file();
  void figure_center();
  void put_back_in_place();
  void loadSettings();
  void setSetings_flag();

  double scal = 1, rotate_x = 0, rotate_y = 0, rotate_z = 0;
  double move_x = 0, move_y = 0, move_z = 0, count = 0.0;
  int error = 0, settings_flag = 0, changeStylerReecordBar = 0;
  data_t *copyObj = NULL;
  QString path, filename;
  const QString Style =
      "QPushButton {\
                                background-color: rgb(90, 90, 90);\
                                color: white;\
                                border-radius: 2px;\
                              }";
  const QString StyleActiv =
      "QPushButton {\
                                background-color: rgb(32,178,70);\
                                color: white;\
                                border-radius: 2px;\
                              }\
                                QPushButton::pressed {\
                                     background-color: 	rgb(27, 152, 60);\
                                }";
  const QString StyleRecord =
      "QPushButton {\
                                background-color: rgb(100, 0, 0);\
                                color: white;\
                                border-radius: 2px;\
                              }";
  const QString downRecord =
      "QLabel {\
                               background-color:rgb(100, 0, 0);\
                               color: white;\
                                padding-left:4px;\
                                font: 200 12pt Apple;}";
  const QString downStyle =
      "QLabel {\
                                background-color:rgb(30,30,30);\
                                padding-left:4px;\
                                color: rgb(120,120,120);\
                                font: 200 12pt Apple;\
                                border-radius:2px;\
                            }";
 private slots:
  void on_doubleSpinBox_valueChanged(double arg1);
  void on_actionOpen_triggered();
  void on_spin_rotate_x_valueChanged(double arg1);

  void on_spin_rotate_y_valueChanged(double arg1);

  void on_spin_rotate_z_valueChanged(double arg1);

  void on_spin_move_x_valueChanged(double arg1);

  void on_spin_move_y_valueChanged(double arg1);

  void on_spin_move_z_valueChanged(double arg1);

  void on_reset_model_clicked();

  void on_actionBackground_triggered();
  void on_actionColor_triggered();
  void on_actionColor_2_triggered();

  void on_actionSize_triggered();

  void on_actionHide_2_triggered();
  void on_actionShow_triggered();
  void on_actionHide_4_triggered();
  void on_actionShow_2_triggered();

  void on_actionRound_triggered();
  void on_actionSquare_triggered();

  void on_actionDotted_triggered();
  void on_actionSolid_triggered();

  void on_actionWidth_triggered();

  void on_action_bmp_triggered();
  void on_action_jpeg_triggered();

  void on_checkBox_axis_stateChanged(int arg1);

  void on_comboBox_activated(int index);

  void timer_run();
  void make_gif();
  void record_gif();

  void on_save_settings_clicked();
  void changeSetStyleBar();
  void set_label_save();

 private:
  Ui::MainWindow *ui;
  QTimer *timer;
  QTimer *timerSave;
  QTimer *timerRecord;
  QVector<QImage> gif;

 signals:
  void signal_color_change_b(QColor c);
  void signal_color_change_vertex(QColor c);
  void signal_color_change_facets(QColor c);
  void signal_reset_colors(int f);
};
#endif  // MAINWINDOW_H
