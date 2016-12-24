#include "camera.h"

Camera::Camera() {
    position_ = glm::vec3(0.0f, -2.0f, 0.0f);
    yaw_ = -90.f;
    pitch_ = 25.f;

    update();
}

Camera::~Camera() {

}

void Camera::addPosition(glm::vec3 delta) {
    position_ += front_ * delta.z;
    position_ += right_ * delta.y;

    update();
}

void Camera::setPosition(glm::vec3 position) {
    position_ = position;

    update();
}

void Camera::addRotation(GLfloat pitch, GLfloat yaw) {
    pitch_ -= pitch / 5.f;
    if (pitch_ < -80.f) {
        pitch_ = -80.f;
    } else if (pitch_ > 80.f) {
        pitch_ = 80.f;
    }
    yaw_ += yaw / 5.f;

    update();
}


void Camera::setRotation(GLfloat pitch, GLfloat yaw) {
    pitch_ = pitch;
    yaw_ = yaw;

    update();
}


void Camera::update() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(pitch_)) * std::cos(glm::radians(yaw_));
    front.y = std::sin(glm::radians(pitch_));
    front.z = std::cos(glm::radians(pitch_)) * std::sin(glm::radians(yaw_));
    front_ = glm::normalize(front);

    right_ = glm::normalize(glm::cross(front_, glm::vec3(0.0f, 1.0f, 0.0f)));
    up_ = glm::normalize(glm::cross(right_, front_));
}


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}


glm::vec3 Camera::getPosition() const {
    return position_;
}

glm::vec3 Camera::getFront() const {
    return front_;
}
