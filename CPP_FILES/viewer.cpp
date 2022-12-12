#include "viewer.h"

Viewer::Viewer(QWidget *parent) : QOpenGLWidget{parent} {
  width_f = 1.0;
  flagDraw = 0;
  flag_axis = 0;
  r = 0.0;
  g = 0.0;
  b = 0.0;
  rF = 0;
  gF = 1;
  bF = 0;
  rV = 0.5;
  gV = 0.5;
  bV = 0.0;
  v_size = 4;
}

void Viewer::initializeGL() {}

void Viewer::paintGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (perspective_off == 0) {
    gluPerspective(45.0f, (GLfloat)670 / (GLfloat)645, 0.0001, 1000000.0);
    glTranslatef(0, 0, -2.0f);
  }
  glClearColor(r, g, b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (flagDraw == 1) {
    if (facets_off == 0) {
      for (int i = 0; i < obj->facets; i++) {
        for (int j = 0; j < obj->polygons[i].counter_of_vertexes; j++) {
          glLineWidth(width_f);
          glColor3d(rF, gF, bF);
          if (facets_type == 1) {
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(1, 3);
          } else if (facets_type == 0) {
            glDisable(GL_LINE_STIPPLE);
          }
          glBegin(GL_LINE_LOOP);
          glVertex3d(obj->object_3d.coordinates[obj->polygons[i].vertex[j]].x,
                     obj->object_3d.coordinates[obj->polygons[i].vertex[j]].y,
                     obj->object_3d.coordinates[obj->polygons[i].vertex[j]].z);
        }
        glEnd();
      }
    }
    if (vertex_off == 0) {
      Point();
    }
  }
  if (flag_axis == 2) {
    Axis();
  }
}

void Viewer::resizeGL(int w, int h) {}

void Viewer::Point() {
  for (int i = 0; i < obj->facets; i++) {
    for (int j = 0; j < obj->polygons[i].counter_of_vertexes; j++) {
      glColor3d(rV, gV, bV);
      glEnable(GL_BLEND);
      if (vertex_type == 0) {  //круг
        glEnable(GL_POINT_SMOOTH);
      } else if (vertex_type == 1) {  //квадрат
        glDisable(GL_POINT_SMOOTH);
      }
      glPointSize(v_size);
      glBegin(GL_POINTS);
      glVertex3d(obj->object_3d.coordinates[obj->polygons[i].vertex[j]].x,
                 obj->object_3d.coordinates[obj->polygons[i].vertex[j]].y,
                 obj->object_3d.coordinates[obj->polygons[i].vertex[j]].z);
    }
    glEnd();
  }
}

void Viewer::Axis() {
  glDisable(GL_LINE_STIPPLE);
  glLineWidth(2.0f);
  glColor4f(0.3, 0.3, 0.3, 0);
  glBegin(GL_LINES);
  glVertex3d(1.0, 0.0, 0.0);
  glVertex3d(-1.0, 0.0, 0.0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3d(0.0, 1.0, 0.0);
  glVertex3d(0.0, -1.0, 0.0);
  glEnd();
}

void Viewer::change_b_color(QColor c) {
  c.toRgb();
  r = c.red();
  g = c.green();
  b = c.blue();
  r /= 255, g /= 255, b /= 255;
  if (flag_v_hidden == 1) {
    rV = r, bV = b, gV = g;
  }
  if (flag_f_hidden == 1) {
    rF = r, bF = b, gF = g;
  }
}

void Viewer::change_v_color(QColor c) {
  c.toRgb();
  rV = c.red();
  gV = c.green();
  bV = c.blue();
  rV /= 255, gV /= 255, bV /= 255;
}

void Viewer::change_f_color(QColor c) {
  c.toRgb();
  rF = c.red();
  gF = c.green();
  bF = c.blue();
  rF /= 255, gF /= 255, bF /= 255;
}

void Viewer::change_reset_color(int f) {
  width_f = 1.0;
  v_size = 4.0;
  flagDraw = 0;
  r = 0.0;
  g = 0.0;
  b = 0.0;
  rF = 0;
  gF = 1;
  bF = 0;
  rV = 0.5;
  gV = 0.5;
  bV = 0.0;
  facets_type = 0;
  vertex_type = 0;
}
