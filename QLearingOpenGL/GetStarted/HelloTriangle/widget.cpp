#include "widget.h"
#include <QDebug>
#include <QKeyEvent>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    resize(600, 400);
    m_model = QMatrix4x4();
    m_view = QMatrix4x4();
    m_view.translate(0, 0, -2);
    m_camera = new Camera(this, QVector3D(0, 0, 3));
    m_camera->setMovementSpeed(2);
    setUpdateBehavior(QOpenGLWidget::PartialUpdate);
    startTimer(16);
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    m_shaderProgram = new QOpenGLShaderProgram(this);
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/hellotriangle.vert");
    m_shaderProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/hellotriangle.frag");
    m_shaderProgram->link();

    m_viewUniform = m_shaderProgram->uniformLocation("view");
    m_modelUniform = m_shaderProgram->uniformLocation("model");
    m_projectiveUniform = m_shaderProgram->uniformLocation("projective");
    m_colorAttr = m_shaderProgram->attributeLocation("vcolor");
    m_vertextAttr = m_shaderProgram->attributeLocation("vertex");
}

void Widget::paintGL()
{
    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
    m_shaderProgram->bind();

    // vertex data
    GLfloat vertexes[] = {
        0, 1,
        -1, -1,
        1, -1
    };

    // color data
    GLfloat colors[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    m_shaderProgram->setUniformValue(m_viewUniform, m_camera->getViewMatrix());
    m_shaderProgram->setUniformValue(m_modelUniform, m_model);
    m_shaderProgram->setUniformValue(m_projectiveUniform, m_projective);

    glVertexAttribPointer(GLuint(m_vertextAttr), 2, GL_FLOAT, false, 0, vertexes);
    glVertexAttribPointer(GLuint(m_colorAttr), 3, GL_FLOAT, false, 0, colors);

    glEnableVertexAttribArray(GLuint(m_vertextAttr));
    glEnableVertexAttribArray(GLuint(m_colorAttr));

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(GLuint(m_vertextAttr));
    glDisableVertexAttribArray(GLuint(m_colorAttr));
    m_shaderProgram->release();
}

void Widget::resizeGL(int w, int h)
{
    m_projective = QMatrix4x4();
    m_projective.perspective(60, float(w * 1.0 / h), 0, 1000.0);
    m_shaderProgram->setUniformValue(m_projectiveUniform, m_projective);
}

void Widget::timerEvent(QTimerEvent *e)
{
    m_camera->processKeyEvent(0.016f);
    update();
    QOpenGLWidget::timerEvent(e);
}
