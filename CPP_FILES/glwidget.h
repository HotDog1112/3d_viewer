#ifndef GLWIDGET_H
#define GLWIDGET_H
#define GL_SILENCE_DEPRECATION
#include <QObject>
#include <QOpenGLWidget>
#include <QWidget>

#include "mainwindow.h"

class GLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  explicit GLWidget(QWidget *parent = nullptr);
  int r, g, b;

 public slots:

 signals:
};

#endif  // GLWIDGET_H
