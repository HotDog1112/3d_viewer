#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QInputDialog>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QColorDialog>
#include "OpenGL/gltypes.h"
extern "C"
{
    #include "s21_viewer_3d.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void new_file();
    double scal = 1, rotate_x = 0, rotate_y = 0, rotate_z = 0;
    double move_x = 0, move_y = 0, move_z = 0;

private slots:
    void on_button_choose_file_clicked();
    void on_doubleSpinBox_valueChanged(double arg1);

    void on_spin_rotate_x_valueChanged(double arg1);

    void on_spin_rotate_y_valueChanged(double arg1);

    void on_spin_rotate_z_valueChanged(double arg1);

    void on_spin_move_x_valueChanged(double arg1);

    void on_spin_move_y_valueChanged(double arg1);

    void on_spin_move_z_valueChanged(double arg1);

    void on_reset_model_clicked();

private:
    Ui::MainWindow *ui;

signals:

};
#endif // MAINWINDOW_H
