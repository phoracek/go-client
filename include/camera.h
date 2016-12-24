#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

class Camera {
public:
    Camera();
    ~Camera();

    glm::mat4 getViewMatrix() const;

    void addPosition(glm::vec3 relative_position);
    void setPosition(glm::vec3 position);
    void addRotation(GLfloat pitch, GLfloat yaw);
    void setRotation(GLfloat pitch, GLfloat yaw);

    // TODO type
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;

private:
    void update();

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;


    GLfloat pitch_;
    GLfloat yaw_;
};

#endif // CAMERA_H_