#ifndef VIEWER_H
#define VIEWER_H
#define GL_SILENCE_DEPRECATION
#include <openGL/glu.h>

#include <QOpenGLWidget>

#include "mainwindow.h"
//#include <gif.h>

class Viewer : public QOpenGLWidget {
  Q_OBJECT
 public:
  explicit Viewer(QWidget *parent = nullptr);
  data_t *obj = NULL;
  int flagDraw;
  int flag_v_hidden = 0, flag_f_hidden = 0;

  double r, g, b, rV, gV, bV, rF, gF, bF, width_f, v_size;

  int vertex_type = 0, facets_type = 0, vertex_off = 0, facets_off = 0,
      perspective_off = 1;
  int flag_axis = 1;
  void Point();
  void Axis();

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

 public slots:
  void change_b_color(QColor c);
  void change_v_color(QColor c);
  void change_f_color(QColor c);
  void change_reset_color(int f);
};

#endif  // VIEWER_H
