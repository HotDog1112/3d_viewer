#include <locale.h>

#include <QApplication>
#include <QtGui>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_ALL, "C");
  QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
  MainWindow w;
  w.setFixedSize(w.geometry().width(), w.geometry().height());
  w.show();
  return a.exec();
}
