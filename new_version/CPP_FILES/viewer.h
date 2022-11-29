#ifndef VIEWER_H
#define VIEWER_H
#define GL_SILENCE_DEPRECATION
#include <QOpenGLWidget>
#include "mainwindow.h"
//#include <GL/glu.h>

class Viewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = nullptr);
    data_t *obj = NULL;
    int flagDraw;
    double r, g, b, rV, gV, bV, rF, gF, bF, thick_line;

    void Point();
    void Axis();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

};

#endif // VIEWER_H
