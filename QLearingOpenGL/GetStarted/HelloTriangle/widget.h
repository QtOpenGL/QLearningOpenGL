#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "camera.h"

class Widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget() override;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

protected:
    void timerEvent(QTimerEvent* e) override;

private:
    int m_viewUniform;
    int m_modelUniform;
    int m_projectiveUniform;
    int m_vertextAttr;
    int m_colorAttr;

    QOpenGLShaderProgram* m_shaderProgram;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;
    QMatrix4x4 m_projective;
    Camera* m_camera;
    QPoint m_LastMousePos;
    QList<int> m_pressedKeys;
};


#endif // WIDGET_H
