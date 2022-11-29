#include "viewer.h"

Viewer::Viewer(QWidget *parent)
    : QOpenGLWidget{parent} {
    thick_line = 2.0;
    flagDraw = 0;
    r = 0.3;
    g = 0.3;
    b = 0.3;
    rF = 0;
    gF = 1;
    bF = 0;
    rV = 0.5;
    gV = 0.5;
    bV = 0;
}

void Viewer::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_TEXTURE);
//  glPerspective(130, 1, 50, 0);
  glLoadIdentity();
}

void Viewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (flagDraw == 1) {
        //Point();
//        glEnable(GL_LINE_STIPPLE);
//        glLineStipple(1, 5);

        for (int i = 0; i < obj->facets; i++) {
            for (int j = 0; j < obj->polygons[i].counter_of_vertexes; j++) {
                glLineWidth(thick_line);
                glColor3d(rF, gF, bF);
                glBegin(GL_LINE_LOOP);
                glVertex3d(obj->object_3d.coordinates[obj->polygons[i].vertex[j]].x,
                            obj->object_3d.coordinates[obj->polygons[i].vertex[j]].y,
                            obj->object_3d.coordinates[obj->polygons[i].vertex[j]].z);
            }
            glEnd();
        }
        Point();
    }
    Axis();

}

void Viewer::resizeGL(int w, int h) {}

void Viewer::Point() {
    //glDisable(GL_POINT_SMOOTH);
    for (int i = 0; i < obj->facets; i++) {
        for (int j = 0; j < obj->polygons[i].counter_of_vertexes; j++) {
            glColor3d(rV, gV, bV);
            glEnable(GL_BLEND);
            glEnable(GL_POINT_SMOOTH);
            glPointSize(5.0f);
            glBegin(GL_POINTS);
            glVertex3d(obj->object_3d.coordinates[obj->polygons[i].vertex[j]].x,
                        obj->object_3d.coordinates[obj->polygons[i].vertex[j]].y,
                        obj->object_3d.coordinates[obj->polygons[i].vertex[j]].z);
        }
        glEnd();
    }
}

void Viewer::Axis() {
    glLineWidth(2.0f);
    glColor4f(r, g, b, 0.1);
    glBegin(GL_LINES);
    glVertex3d(1.0, 0.0, 0.0);
    glVertex3d(-1.0, 0.0, 0.0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3d(0.0, 1.0, 0.0);
    glVertex3d(0.0, -1.0, 0.0);
    glEnd();
}
