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
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/hellotexture.vert");
    m_shaderProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/hellotexture.frag");
    m_shaderProgram->link();

    m_viewUniform = m_shaderProgram->uniformLocation("view");
    m_modelUniform = m_shaderProgram->uniformLocation("model");
    m_projectiveUniform = m_shaderProgram->uniformLocation("projective");
    m_textureUniform = m_shaderProgram->uniformLocation("texture");
    m_texCoordAttr = m_shaderProgram->attributeLocation("texCoord");
    m_vertextAttr = m_shaderProgram->attributeLocation("vertex");


    m_img = QImage(":/img/a.jpg");
    m_texture = new QOpenGLTexture(m_img.mirrored());
//    m_shaderProgram->setUniformValue(m_textureUniform, 0);
}

void Widget::paintGL()
{
    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.05f, 0.06f, 0.07f, 1);
    m_shaderProgram->bind();

    float rat = m_img.size().height() * 1.0f / m_img.size().width();

    // vertex data
    GLfloat vertexes[] = {
        -1, rat,
        1, rat,
        1, -rat,
        -1, -rat
    };

    // color data
    GLfloat texCoords[] = {
        0, 1,
        1, 1,
        1, 0,
        0, 0
    };

    m_shaderProgram->setUniformValue(m_viewUniform, m_camera->getViewMatrix());
    m_shaderProgram->setUniformValue(m_modelUniform, m_model);
    m_shaderProgram->setUniformValue(m_projectiveUniform, m_projective);

    glVertexAttribPointer(GLuint(m_vertextAttr), 2, GL_FLOAT, false, 0, vertexes);
    glVertexAttribPointer(GLuint(m_texCoordAttr), 2, GL_FLOAT, false, 0, texCoords);

    glEnableVertexAttribArray(GLuint(m_vertextAttr));
    glEnableVertexAttribArray(GLuint(m_texCoordAttr));

    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture->bind();

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(GLuint(m_vertextAttr));
    glDisableVertexAttribArray(GLuint(m_texCoordAttr));
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
