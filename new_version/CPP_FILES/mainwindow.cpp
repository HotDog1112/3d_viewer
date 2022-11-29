#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtGui>
#include <QOpenGLWidget>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    ui->widget->setStyleSheet("background-color: rgb(30, 30, 30)");
    ui->label_f->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    ui->label_v->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    ui->file_name_label->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    ui->doubleSpinBox->setValue(0.00);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_button_choose_file_clicked() {
    int error = 0;
    if (!ui->file_name_label->text().isEmpty()) {
        new_file();
        ui->widget->flagDraw = 0;
        if (ui->widget->obj != NULL) {
            free_data(ui->widget->obj);
        }
    }

    QString filename = QFileDialog::getOpenFileName(this, tr("Open obj"), "file:///",
                                            tr("Object Files (*.obj)"));
    if (filename.isEmpty()) {
            ui->file_name_label->setText("No file");
    } else {
        ui->file_name_label->setWordWrap(true);
        ui->file_name_label->setText(filename);
        QByteArray str = filename.toLocal8Bit();
        char *filename_str = str.data();
        int clean_count = 0;
        ui->widget->obj = obj_point_output(filename_str, &error);
        center(ui->widget->obj);
        adjust(ui->widget->obj, 0.50);
        ui->label_v->setText(QString::number(ui->widget->obj->object_3d.amount_points));
        ui->label_f->setText(QString::number(ui->widget->obj->facets));
        new_file();
        ui->widget->flagDraw = 1;
    }

}



void MainWindow::new_file() {
    ui->doubleSpinBox->setValue(0.50);
    scal = ui->doubleSpinBox->value();
    ui->spin_move_x->setValue(0.0);
    ui->spin_move_y->setValue(0.0);
    ui->spin_move_z->setValue(0.0);
    ui->spin_rotate_x->setValue(0.0);
    ui->spin_rotate_y->setValue(0.0);
    ui->spin_rotate_z->setValue(0.0);
    rotate_x = 0, rotate_y = 0, rotate_z = 0;
    move_x = 0, move_y = 0, move_z = 0;

}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    if (arg1 > 0 && ui->widget->flagDraw == 1) {
        scale(ui->widget->obj, arg1 / scal);
        ui->widget->update();
        scal = arg1;
    }
}


void MainWindow::on_spin_rotate_x_valueChanged(double arg1)
{
    if (ui->widget->flagDraw == 1) {
        rotateX(ui->widget->obj, arg1 - rotate_x);
        ui->widget->update();
        rotate_x = arg1;
    }
}


void MainWindow::on_spin_rotate_y_valueChanged(double arg1)
{
    if (ui->widget->flagDraw == 1) {
        rotateY(ui->widget->obj, arg1 - rotate_y);
        ui->widget->update();
        rotate_y = arg1;
    }
}


void MainWindow::on_spin_rotate_z_valueChanged(double arg1)
{
    if (ui->widget->flagDraw == 1) {
        rotateZ(ui->widget->obj, arg1 - rotate_z);
        ui->widget->update();
        rotate_z = arg1;
    }
}


void MainWindow::on_spin_move_x_valueChanged(double arg1)
{
    if (ui->widget->flagDraw == 1) {
        moveObj_x(ui->widget->obj, arg1 - move_x);
        ui->widget->update();
        move_x = arg1;
    }
}


void MainWindow::on_spin_move_y_valueChanged(double arg1)
{
    if (ui->widget->flagDraw == 1) {
        moveObj_y(ui->widget->obj, arg1 - move_y);
        ui->widget->update();
        move_y = arg1;
    }
}


void MainWindow::on_spin_move_z_valueChanged(double arg1)
{
    if (ui->widget->flagDraw == 1) {
        moveObj_z(ui->widget->obj, arg1 - move_z);
        ui->widget->update();
        move_z = arg1;
    }
}


void MainWindow::on_reset_model_clicked()
{
    int error = 0;
    if (ui->widget->obj != NULL) {
        ui->widget->flagDraw = 0;
        free_data(ui->widget->obj);
    }
    QString filename = ui->file_name_label->text();
    QByteArray str = filename.toLocal8Bit();
    char *filename_str = str.data();
    ui->widget->obj = obj_point_output(filename_str, &error);
    if (error == 2) {
        ui->file_name_label->setText("No file");
    } else {
        new_file();
        center(ui->widget->obj);
        adjust(ui->widget->obj, ui->doubleSpinBox->value());
        scal = ui->doubleSpinBox->value();
        ui->widget->flagDraw = 1;
        ui->widget->update();
    }

}

