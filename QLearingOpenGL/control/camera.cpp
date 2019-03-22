#include "camera.h"
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

Camera::Camera(QObject *parent, QVector3D position, QVector3D up, float yaw, float pitch):QObject (parent)
{
    Position = position;
    WorldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;
    Front = QVector3D(0.0f, 0.0f, -1.0f);
    m_movementSpeed = SPEED;
    m_mouseSensitivity = SENSITIVITY;
    m_zoom = ZOOM;
    updateCameraVectors();

    if(parent){
        parent->installEventFilter(this);
    }
}

Camera::~Camera()
{

}

void Camera::setMovementSpeed(const float speed)
{
    m_movementSpeed = speed;
}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 mat;
    mat.lookAt(Position, Position + Front, Up);
    return mat;
}

QVector3D Camera::position()
{
    return this->Position;
}

void Camera::processKeyBoard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    if (m_zoom >= 1.0f && m_zoom <= 45.0f)
        m_zoom -= yoffset;
    if (m_zoom <= 1.0f)
        m_zoom = 1.0f;
    if (m_zoom >= 45.0f)
        m_zoom = 45.0f;
}

void Camera::processKeyEvent(const float &delta)
{
    foreach (const int& key, m_pressedKeys) {
        if(key == Qt::Key_W || key == Qt::Key_Up) {
            processKeyBoard(FORWARD, delta);
        }
        if(key == Qt::Key_S || key == Qt::Key_Down){
            processKeyBoard(BACKWARD, delta);
        }
        if(key == Qt::Key_Right || key == Qt::Key_D) {
            processKeyBoard(RIGHT, delta);
        }
        if(key == Qt::Key_Left || key == Qt::Key_A) {
            processKeyBoard(LEFT, delta);
        }
    }
}

bool Camera::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        if(!m_pressedKeys.contains(keyEvent->key()))
            m_pressedKeys << keyEvent->key();
    }
    if(e->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        if(m_pressedKeys.contains(keyEvent->key()))
            m_pressedKeys.removeOne(keyEvent->key());
    }
    if(e->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        QPoint dp = mouseEvent->pos() - m_lastMousePos;
        m_lastMousePos = mouseEvent->pos();
        processMouseMovement(dp.x(), -dp.y());
    }

    if(e->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(e);
        m_lastMousePos = mouseEvent->pos();
    }
    return QObject::eventFilter(obj, e);
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    float fx = qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch));
    float fy = qSin(qDegreesToRadians(m_pitch));
    float fz = qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch));
    Front.setX(fx);
    Front.setY(fy);
    Front.setZ(fz);
    Front.normalize();
    // Also re-calculate the Right and Up vector
    Right = QVector3D::crossProduct(Front, WorldUp);  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = QVector3D::crossProduct(Right, Front);
    Right.normalize();
    Up.normalize();
}
