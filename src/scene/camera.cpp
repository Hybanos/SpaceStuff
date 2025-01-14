#include "scene/camera.hpp"

void Camera::translate(glm::vec3 vec) {
    position += vec;
}

void Camera::go_to(glm::vec3 _position) {
    position = _position;
}

void Camera::look_at(glm::vec3 _center) {
    center = _center;
}

glm::mat4 Camera::get_view() {
    return glm::lookAt(
        position,
        center,
        glm::vec3(0,1,0)
    );
}