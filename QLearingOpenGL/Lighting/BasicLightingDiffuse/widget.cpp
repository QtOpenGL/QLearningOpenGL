#include "widget.h"
#include <QDebug>
#include <QKeyEvent>

static QVector3D lightPos(4.1f, 5.0f, 2.0f);
static QVector3D lightColor(1.0f, 1.0f, 1.0f);
static QVector3D objectColor(1.0f, 0.5f, 0.31f);

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
    m_vao = new QOpenGLVertexArrayObject(this);
    m_vbo = new QOpenGLBuffer;
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    m_shaderProgram = new QOpenGLShaderProgram(this);
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/basicllighting/diffuse.vert");
    m_shaderProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/basicllighting/diffuse.frag");
    m_shaderProgram->link();

    m_vao->create();
    m_vbo->create();

    m_viewUniform = m_shaderProgram->uniformLocation("view");
    m_modelUniform = m_shaderProgram->uniformLocation("model");
    m_projectiveUniform = m_shaderProgram->uniformLocation("projective");
    m_lightColorUniform = m_shaderProgram->uniformLocation("lightColor");
    m_lightPosUniform = m_shaderProgram->uniformLocation("lightPos");
    m_objectColorUniform = m_shaderProgram->uniformLocation("objectColor");

    m_vertextAttr = m_shaderProgram->attributeLocation("vertex");
    m_normalAttr = m_shaderProgram->attributeLocation("aNormal");

//    m_vao->bind();
//    glVertexAttribPointer(GLuint(m_vertextAttr), 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void*)0);
//    glEnableVertexAttribArray(GLuint(m_vertextAttr));
//    glVertexAttribPointer(GLuint(m_normalAttr), 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void*) (3 * sizeof(float)));
//    glEnableVertexAttribArray(GLuint(m_normalAttr));
}

void Widget::paintGL()
{
    glViewport(0, 0, width(), height());
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaderProgram->bind();

    // vertex data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };


    m_shaderProgram->setUniformValue(m_viewUniform, m_camera->getViewMatrix());
    m_shaderProgram->setUniformValue(m_modelUniform, m_model);
    m_shaderProgram->setUniformValue(m_projectiveUniform, m_projective);
    m_shaderProgram->setUniformValue(m_lightPosUniform, lightPos);
    m_shaderProgram->setUniformValue(m_lightColorUniform, lightColor);
    m_shaderProgram->setUniformValue(m_objectColorUniform, objectColor);

    m_vao->bind();
    m_vbo->bind();
    m_vbo->allocate(vertices, sizeof (vertices));

    glVertexAttribPointer(GLuint(m_vertextAttr), 3, GL_FLOAT, false, 6 * sizeof (float), (void*)0);
    glVertexAttribPointer(GLuint(m_normalAttr), 3, GL_FLOAT, false, 6 * sizeof (float) , (void*)(3 * sizeof (float)));

    glEnableVertexAttribArray(GLuint(m_vertextAttr));
    glEnableVertexAttribArray(GLuint(m_normalAttr));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDisableVertexAttribArray(GLuint(m_vertextAttr));
    glDisableVertexAttribArray(GLuint(m_normalAttr));
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
