#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QtMath>

// Default camera values
#define YAW          -90.0f
#define PITCH         0.0f
#define SPEED         2.5f
#define SENSITIVITY   0.1f
#define ZOOM          45.0f

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera: public QObject
{
    Q_OBJECT
public:
    // Constructor with vectors
    explicit Camera(QObject* parent = nullptr, QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    ~Camera() override;
    // Camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;

    // Camera options
    float Zoom;

    void setMovementSpeed(const float speed);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    QMatrix4x4 getViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyBoard(Camera_Movement direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset);

    void processKeyEvent(const float& delta);

protected:
    bool eventFilter(QObject* obj, QEvent* e) override;

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    QPoint m_lastMousePos;
    QList<int> m_pressedKeys;
    float m_movementSpeed;
    float m_mouseSensitivity;

    // Euler Angles
    float m_yaw;
    float m_pitch;
    float m_zoom;
};

#endif // CAMERA_H
