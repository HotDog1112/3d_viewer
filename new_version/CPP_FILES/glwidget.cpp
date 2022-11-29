#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget{parent}
{
    r = 0;
    g = 0;
    b = 0;

}

//void GLWidget::foo(int) {
//    r = 1;
//    initializeGL();
//    DrawAxis();
//    printf("!!!!!");
//    update();
//}


//void GLWidget::initializeGL() {
//  glEnable(GL_DEPTH_TEST);
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//}

//void GLWidget::DrawAxis() {
//  glLineWidth(2.0f);
//  glColor4f(32, 42, 0, 0.1);
//  glBegin(GL_LINES);
//  glVertex3f(1.0f, 0.0f, 0.0f);
//  glVertex3f(-1.0f, 0.0f, 0.0f);
//  glEnd();
//  glBegin(GL_LINES);
//  glVertex3f(0.0f, 1.0f, 0.0f);
//  glVertex3f(0.0f, -1.0f, 0.0f);
//  glEnd();
//  glBegin(GL_LINES);
//  glVertex3f(0.0f, 0.0f, 1.0f);
//  glVertex3f(0.0f, 0.0f, -1.0f);
//  glEnd();
//}
