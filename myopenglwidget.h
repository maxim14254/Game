#include <QtOpenGLWidgets>
#include "control.h""

#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H


class MyOpenGLWidget : public QOpenGLWidget
{
public:
    explicit MyOpenGLWidget(QWidget* parent);
    size_t GameLevel = 0;
    bool gameNew = false;
    Control control;
     ~MyOpenGLWidget();
protected:
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
private:
    void Scene1();
    void Scene2();
    void OnIdle();
};

#endif // MYOPENGLWIDGET_H
