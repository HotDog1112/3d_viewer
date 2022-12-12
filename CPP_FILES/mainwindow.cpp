#include "mainwindow.h"

#include <QFileDialog>
#include <QOpenGLWidget>
#include <QtGui>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  timer = new QTimer(0);
  timerSave = new QTimer(0);
  timerRecord = new QTimer(0);
  setSetings_flag();
  ui->actionShow_2->setDisabled(true);
  ui->actionShow->setDisabled(true);
  ui->actionHide_2->setDisabled(true);
  ui->actionHide_4->setDisabled(true);
  ui->action_bmp->setDisabled(true);
  ui->action_jpeg->setDisabled(true);
  ui->comboBox->addItem("parallel");
  ui->comboBox->addItem("central");
  ui->widget->flag_axis = ui->checkBox_axis->checkState();
  ui->widget->setStyleSheet("background-color: rgb(30, 30, 30)");
  ui->file_name_label->setAlignment(Qt::AlignLeft);
  ui->doubleSpinBox->setValue(0.00);
  connect(this, &MainWindow::signal_color_change_b, ui->widget,
          &Viewer::change_b_color);
  connect(this, &MainWindow::signal_color_change_vertex, ui->widget,
          &Viewer::change_v_color);
  connect(this, &MainWindow::signal_reset_colors, ui->widget,
          &Viewer::change_reset_color);
  connect(this, &MainWindow::signal_color_change_facets, ui->widget,
          &Viewer::change_f_color);
  connect(ui->record, SIGNAL(clicked()), this, SLOT(record_gif()));
  connect(timer, SIGNAL(timeout()), this, SLOT(timer_run()));
  connect(timerSave, SIGNAL(timeout()), this, SLOT(set_label_save()));
  connect(timerRecord, SIGNAL(timeout()), this, SLOT(changeSetStyleBar()));
  if (ui->widget->obj == NULL) {
    ui->reset_model->setDisabled(true);
    ui->record->setDisabled(true);
    ui->reset_model->setStyleSheet(Style);
    ui->record->setStyleSheet(Style);
  }
  if (settings_flag == 1) {
    loadSettings();
  }
}

MainWindow::~MainWindow() {
  delete timerRecord;
  delete timerSave;
  delete timer;
  delete ui;
}

void MainWindow::on_actionOpen_triggered() {
  ui->widget->flagDraw = 0;
  ui->widget->update();

  filename = QFileDialog::getOpenFileName(this, tr("Open obj"), "file:///",
                                          tr("Object Files (*.obj)"));
  if (!ui->file_name_label->text().isEmpty() && filename.isEmpty()) {
    ui->widget->flagDraw = 1;
    ui->widget->update();
  } else if (filename.isEmpty()) {
    ui->file_name_label->setText("File not found!");
  } else {
    if (ui->widget->obj != NULL) {
      free_data(ui->widget->obj);
    }
    ui->widget->perspective_off = 1;
    ui->comboBox->setCurrentIndex(0);
    ui->action_bmp->setEnabled(true);
    ui->action_jpeg->setEnabled(true);
    ui->reset_model->setEnabled(true);
    ui->record->setEnabled(true);
    ui->reset_model->setStyleSheet(StyleActiv);
    ui->record->setStyleSheet(StyleActiv);
    ui->file_name_label->setWordWrap(true);
    ui->file_name_label->setText(filename);
    QByteArray str = filename.toLocal8Bit();
    char *filename_str = str.data();
    int clean_count = 0;
    ui->widget->obj = obj_point_output(filename_str, &error);
    if (error == 0) {
      new_file();
      center(ui->widget->obj);
      adjust(ui->widget->obj, scal);
      copyObj = copyData(ui->widget->obj, &error);
      QString all_info_down =
          "V: " + QString::number(ui->widget->obj->object_3d.amount_points) +
          " F: " + QString::number(ui->widget->obj->facets) + " | " +
          ui->file_name_label->text();
      ui->down_label->setText(all_info_down);
      ui->widget->flagDraw = 1;
      ui->widget->update();
    } else {
      ui->file_name_label->setText("File not found!");
    }
  }
}

void MainWindow::new_file() {
  ui->actionShow_2->setDisabled(true);
  ui->actionHide_2->setEnabled(true);
  ui->widget->vertex_off = 0;
  ui->actionHide_4->setEnabled(true);
  ui->actionShow->setDisabled(true);
  ui->widget->facets_off = 0;
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

void MainWindow::figure_center() {
  moveObj_x(ui->widget->obj, 0 - move_x);
  moveObj_y(ui->widget->obj, 0 - move_y);
  moveObj_z(ui->widget->obj, 0 - move_z);
}

void MainWindow::put_back_in_place() {
  moveObj_x(ui->widget->obj, move_x);
  moveObj_y(ui->widget->obj, move_y);
  moveObj_z(ui->widget->obj, move_z);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1) {
  if (arg1 > 0 && ui->widget->flagDraw == 1) {
    figure_center();
    scale(ui->widget->obj, arg1 / scal);
    put_back_in_place();
    ui->widget->update();
    scal = arg1;
  }
}

void MainWindow::on_spin_rotate_x_valueChanged(double arg1) {
  if (ui->widget->flagDraw == 1) {
    figure_center();
    rotateZ(ui->widget->obj, 0 - rotate_z);
    rotateY(ui->widget->obj, 0 - rotate_y);
    rotateX(ui->widget->obj, arg1 - rotate_x);
    rotateY(ui->widget->obj, rotate_y);
    rotateZ(ui->widget->obj, rotate_z);
    put_back_in_place();
    ui->widget->update();
    rotate_x = arg1;
  }
}

void MainWindow::on_spin_rotate_y_valueChanged(double arg1) {
  if (ui->widget->flagDraw == 1) {
    figure_center();
    rotateZ(ui->widget->obj, 0 - rotate_z);
    rotateX(ui->widget->obj, 0 - rotate_x);
    rotateX(ui->widget->obj, rotate_x);
    rotateY(ui->widget->obj, arg1 - rotate_y);
    rotateZ(ui->widget->obj, rotate_z);
    put_back_in_place();
    ui->widget->update();
    rotate_y = arg1;
  }
}

void MainWindow::on_spin_rotate_z_valueChanged(double arg1) {
  if (ui->widget->flagDraw == 1) {
    figure_center();
    rotateY(ui->widget->obj, 0 - rotate_y);
    rotateX(ui->widget->obj, 0 - rotate_x);
    rotateX(ui->widget->obj, rotate_x);
    rotateY(ui->widget->obj, rotate_y);
    rotateZ(ui->widget->obj, arg1 - rotate_z);
    put_back_in_place();
    ui->widget->update();
    rotate_z = arg1;
  }
}

void MainWindow::on_spin_move_x_valueChanged(double arg1) {
  if (ui->widget->flagDraw == 1) {
    moveObj_x(ui->widget->obj, arg1 - move_x);
    ui->widget->update();
    move_x = arg1;
  }
}

void MainWindow::on_spin_move_y_valueChanged(double arg1) {
  if (ui->widget->flagDraw == 1) {
    moveObj_y(ui->widget->obj, arg1 - move_y);
    ui->widget->update();
    move_y = arg1;
  }
}

void MainWindow::on_spin_move_z_valueChanged(double arg1) {
  if (ui->widget->flagDraw == 1) {
    moveObj_z(ui->widget->obj, arg1 - move_z);
    ui->widget->update();
    move_z = arg1;
  }
}

void MainWindow::on_reset_model_clicked() {
  if (ui->widget->obj != NULL) {
    ui->widget->flagDraw = 0;
    free_data(ui->widget->obj);
    ui->widget->obj = NULL;
    ui->widget->update();
    new_file();
    ui->widget->obj = copyData(copyObj, &error);
  }
  if (error == 0) {
    ui->actionHide_2->setEnabled(true);
    ui->actionHide_4->setEnabled(true);
    emit signal_reset_colors(1);
    ui->widget->perspective_off = 1;
    ui->comboBox->setCurrentIndex(0);
    ui->widget->flagDraw = 1;
    ui->widget->update();
  } else {
    ui->file_name_label->setText("Memory error!!!");
  }
}

void MainWindow::on_actionBackground_triggered() {
  QColor f_color = QColorDialog::getColor(QColorConstants::Green);
  if (f_color.isValid()) {
    emit signal_color_change_b(f_color);
  }
}

void MainWindow::on_actionColor_triggered() {
  QColor f_color = QColorDialog::getColor(QColorConstants::Green);
  if (f_color.isValid()) {
    emit signal_color_change_vertex(f_color);
  }
}

void MainWindow::on_actionColor_2_triggered() {
  QColor f_color = QColorDialog::getColor(QColorConstants::Green);
  if (f_color.isValid()) {
    emit signal_color_change_facets(f_color);
  }
}

void MainWindow::on_actionSize_triggered() {
  bool res = false;
  double s = QInputDialog::getInt(0, "Size of vertexes",
                                  "Input value: (max = 30)", 0, 1, 30, 1, &res);
  if (res == true) {
    ui->widget->v_size = s;
    ui->widget->update();
  }
}

void MainWindow::on_actionHide_2_triggered() {
  ui->actionShow_2->setEnabled(true);
  ui->actionHide_2->setDisabled(true);
  ui->widget->vertex_off = 1;
  ui->widget->update();
}

void MainWindow::on_actionShow_2_triggered() {
  ui->actionShow_2->setDisabled(true);
  ui->actionHide_2->setEnabled(true);
  ui->widget->vertex_off = 0;
  ui->widget->update();
}

void MainWindow::on_actionHide_4_triggered() {
  ui->actionShow->setEnabled(true);
  ui->actionHide_4->setDisabled(true);
  ui->widget->facets_off = 1;
  ui->widget->update();
}

void MainWindow::on_actionShow_triggered() {
  ui->actionHide_4->setEnabled(true);
  ui->actionShow->setDisabled(true);
  ui->widget->facets_off = 0;
  ui->widget->update();
}

void MainWindow::on_actionWidth_triggered() {
  bool res = false;
  double w = QInputDialog::getInt(0, "Width of facets",
                                  "Input value (max = 30):", 0, 1, 30, 1, &res);
  if (res == true) {
    ui->widget->width_f = w;
    ui->widget->update();
  }
}

void MainWindow::on_actionRound_triggered() {
  ui->widget->vertex_type = 0;
  ui->widget->update();
}

void MainWindow::on_actionSquare_triggered() {
  ui->widget->vertex_type = 1;
  ui->widget->update();
}

void MainWindow::on_actionSolid_triggered() {
  ui->widget->facets_type = 0;
  ui->widget->update();
}

void MainWindow::on_actionDotted_triggered() {
  ui->widget->facets_type = 1;
  ui->widget->update();
}

void MainWindow::on_action_bmp_triggered() {
  QString path = NULL;
  path =
      QFileDialog::getSaveFileName(this, "File save .bmp", NULL, "BMP (*.bmp)");
  if (!path.isNull()) {
    this->ui->widget->grabFramebuffer().save(path, NULL, -1);
  }
}

void MainWindow::on_action_jpeg_triggered() {
  QString path = NULL;
  path = QFileDialog::getSaveFileName(this, "File save .jpeg", NULL,
                                      "JPEG (*.jpeg)");
  if (!path.isNull()) {
    this->ui->widget->grabFramebuffer().save(path, NULL, -1);
  }
}

void MainWindow::on_checkBox_axis_stateChanged(int arg1) {
  if (ui->checkBox_axis->checkState() == 2) {
    ui->axisX->setText("X");
    ui->axisY->setText("Y");
    ui->axisZ->setText("Z");
    ui->widget->flag_axis = 2;
  } else {
    ui->axisX->setText("");
    ui->axisY->setText("");
    ui->axisZ->setText("");
    ui->widget->flag_axis = 0;
  }
  ui->widget->update();
}

void MainWindow::on_comboBox_activated(int index) {
  if (index == 0) {
    ui->widget->perspective_off = 1;
    ui->widget->update();
  } else {
    ui->widget->perspective_off = 0;
    ui->widget->update();
  }
}

void MainWindow::record_gif() {
  timer->start(100);
  timerRecord->start(700);
  ui->record->setDisabled(true);
  ui->record->setStyleSheet(StyleRecord);
  timer_run();
  count = 0.0;
}

void MainWindow::timer_run() {
  if (count <= 5.0) {
    gif.push_back(ui->widget->grab().toImage());
    count += 0.1;
  } else {
    timerRecord->stop();
    make_gif();
    timer->stop();
  }
}

void MainWindow::make_gif() {
  QString gif_name =
      QFileDialog::getSaveFileName(this, "Save gif", NULL, "*.gif");
  if (!gif_name.isNull()) {
    QGifImage gif_image(QSize(1343, 1293));
    gif_image.setDefaultDelay(100);

    for (QVector<QImage>::Iterator screen = gif.begin(); screen != gif.end();
         ++screen) {
      gif_image.addFrame(*screen);
    }
    gif_image.save(gif_name);
  }
  gif.clear();
  count = 0.0;
  ui->record->setEnabled(true);
  ui->record->setStyleSheet(StyleActiv);
  ui->down_label->setStyleSheet(downStyle);
  changeStylerReecordBar = 0;
}

void MainWindow::changeSetStyleBar() {
  if (changeStylerReecordBar == 0) {
    ui->down_label->setStyleSheet(downRecord);
    changeStylerReecordBar = 1;
  } else {
    ui->down_label->setStyleSheet(downStyle);
    changeStylerReecordBar = 0;
  }
}

void MainWindow::on_save_settings_clicked() {
  settings_flag = 1;
  path = QDir::homePath() + "/settengs.ini";
  QSettings settings(path, QSettings::IniFormat);
  settings.setValue("settings_flag", settings_flag);
  settings.setValue("filename", filename);
  settings.setValue("rotate_x", rotate_x);
  settings.setValue("rotate_y", rotate_y);
  settings.setValue("rotate_z", rotate_z);
  settings.setValue("move_x", move_x);
  settings.setValue("move_y", move_y);
  settings.setValue("move_z", move_z);
  settings.setValue("scal", scal);
  settings.setValue("flagDraw", ui->widget->flagDraw);
  settings.setValue("width_f", ui->widget->width_f);
  settings.setValue("flag_axis", ui->widget->flag_axis);
  settings.setValue("r", ui->widget->r);
  settings.setValue("g", ui->widget->g);
  settings.setValue("b", ui->widget->b);
  settings.setValue("rF", ui->widget->rF);
  settings.setValue("gF", ui->widget->gF);
  settings.setValue("bF", ui->widget->bF);
  settings.setValue("rV", ui->widget->rV);
  settings.setValue("gV", ui->widget->gV);
  settings.setValue("bV", ui->widget->bV);
  settings.setValue("v_size", ui->widget->v_size);
  settings.setValue("perspective_off", ui->widget->perspective_off);
  settings.setValue("vertex_type", ui->widget->vertex_type);
  settings.setValue("facets_type", ui->widget->facets_type);
  settings.setValue("vertex_off", ui->widget->vertex_off);
  settings.setValue("facets_off", ui->widget->facets_off);
  ui->label_save->setText("Settings saved!");
  timerSave->start(2000);
}

void MainWindow::set_label_save() {
  ui->label_save->setText("");
  timerSave->stop();
}

void MainWindow::loadSettings() {
  path = QDir::homePath() + "/settengs.ini";
  QSettings settings(path, QSettings::IniFormat);
  ui->action_bmp->setEnabled(true);
  ui->action_jpeg->setEnabled(true);
  ui->reset_model->setEnabled(true);
  ui->record->setEnabled(true);
  ui->reset_model->setStyleSheet(StyleActiv);
  ui->record->setStyleSheet(StyleActiv);
  ui->file_name_label->setWordWrap(true);
  filename = settings.value("filename").toString();
  ui->file_name_label->setText(filename);
  QByteArray str = filename.toLocal8Bit();
  char *filename_str = str.data();
  int clean_count = 0;
  ui->widget->obj = obj_point_output(filename_str, &error);
  if (error == 0) {
    QString all_info_down =
        "V: " + QString::number(ui->widget->obj->object_3d.amount_points) +
        " F: " + QString::number(ui->widget->obj->facets) + " | " +
        ui->file_name_label->text();
    ui->down_label->setText(all_info_down);
    new_file();
    center(ui->widget->obj);
    adjust(ui->widget->obj, 0.50);
    copyObj = copyData(ui->widget->obj, &error);
    ui->widget->flagDraw = settings.value("flagDraw", 0).toDouble();
    ui->widget->update();
    double rotate_x1 = settings.value("rotate_x", 0).toDouble();
    double rotate_y1 = settings.value("rotate_y", 0).toDouble();
    double rotate_z1 = settings.value("rotate_z", 0).toDouble();
    double move_x1 = settings.value("move_x", 0).toDouble();
    double move_y1 = settings.value("move_y", 0).toDouble();
    double move_z1 = settings.value("move_z", 0).toDouble();
    double scal1 = settings.value("scal", 0).toDouble();
    ui->doubleSpinBox->setValue(scal1);
    ui->spin_rotate_x->setValue(rotate_x1);
    ui->spin_rotate_y->setValue(rotate_y1);
    ui->spin_rotate_z->setValue(rotate_z1);
    ui->spin_move_x->setValue(move_x1);
    ui->spin_move_y->setValue(move_y1);
    ui->spin_move_z->setValue(move_z1);

    ui->widget->width_f = settings.value("width_f", 1.0).toDouble();
    ui->widget->flag_axis = settings.value("flag_axis", 0).toDouble();
    ui->widget->r = settings.value("r", 0.0).toDouble();
    ui->widget->g = settings.value("g", 0.0).toDouble();
    ui->widget->b = settings.value("b", 0.0).toDouble();
    ui->widget->rF = settings.value("rF", 0.0).toDouble();
    ui->widget->gF = settings.value("gF", 1.0).toDouble();
    ui->widget->bF = settings.value("bF", 0.0).toDouble();
    ui->widget->rV = settings.value("rV", 0.5).toDouble();
    ui->widget->gV = settings.value("gV", 0.5).toDouble();
    ui->widget->bV = settings.value("bV", 0.0).toDouble();
    ui->widget->v_size = settings.value("v_size", 4.0).toDouble();
    ui->widget->vertex_type = settings.value("vertex_type", 0).toInt();
    ui->widget->facets_type = settings.value("facets_type", 0).toInt();
    ui->widget->vertex_off = settings.value("vertex_off", 0).toInt();
    ui->widget->facets_off = settings.value("facets_off", 0).toInt();
    ui->widget->perspective_off = settings.value("perspective_off", 0).toInt();
    if (ui->widget->perspective_off == 0) {
      ui->comboBox->setCurrentIndex(1);
    } else {
      ui->comboBox->setCurrentIndex(0);
    }
    if (ui->widget->vertex_off == 0) {
      on_actionShow_2_triggered();
    } else {
      on_actionHide_2_triggered();
    }
    if (ui->widget->facets_off == 0) {
      on_actionShow_triggered();
    } else {
      on_actionHide_4_triggered();
    }
    if (ui->widget->flag_axis == 0) {
      ui->axisX->setText("");
      ui->axisY->setText("");
      ui->axisZ->setText("");
      ui->widget->flag_axis = 0;
      ui->checkBox_axis->setChecked(false);
    } else {
      ui->axisX->setText("X");
      ui->axisY->setText("Y");
      ui->axisZ->setText("Z");
      ui->widget->flag_axis = 2;
      ui->checkBox_axis->setChecked(true);
    }

    on_checkBox_axis_stateChanged(settings.value("checkBox_axis", 0).toInt());
    ui->widget->update();
  } else {
    ui->file_name_label->setText("Error!");
  }
}

void MainWindow::setSetings_flag() {
  path = QDir::homePath() + "/settengs.ini";
  QSettings settings(path, QSettings::IniFormat);
  settings_flag = settings.value("settings_flag", 0).toInt();
}
