#include "scene/camera.hpp"

void Camera::translate(glm::vec3 vec) {
    position += vec;
}

void Camera::go_to(glm::vec3 _position) {
    position = _position;
}

void Camera::look_at(glm::vec3 _center) {
    def.look_at(_center);
    anchor = &def;
}

glm::mat4 Camera::get_view() {
    return glm::lookAt(
        position,
        anchor->get_camera_center(),
        glm::vec3(0,1,0)
    );
}

void Camera::handle_mouse_move(SDL_Event& event) {
    if (!enable_move) return;

    float x = event.motion.xrel;
    float y = event.motion.yrel;

    yaw+= x * 0.01;

    pitch+= y * 0.01;
    pitch = fmax(-M_PI_2, pitch);
    pitch = fmin(M_PI_2, pitch);

    update_pos();
}

void Camera::handle_mouse_scroll(SDL_Event& event) {
    if (event.wheel.y == 0) return;
    distance *= abs(event.wheel.y) * (1 - 0.1 * event.wheel.y / abs(event.wheel.y));
    update_pos();
}

void Camera::update_pos() {
    position[0] = cos(yaw) * cos(pitch);
    position[1] = sin(pitch);
    position[2] = sin(yaw) * cos(pitch);

    position *= distance;
    position += anchor->get_camera_center();
}

void Camera::set_anchor(CameraAnchor *a) {
    anchor = a;
    update_pos();
}

void Camera::debug() {
    ImGui::InputFloat3("Camera center", (float*) &anchor->get_camera_center());
    ImGui::InputFloat3("Camera position", (float*)&position);
    if (ImGui::Button("RESET")) {
        def.look_at(glm::vec3(0));
        anchor = &def;
        distance = 10000;
    }
    ImGui::Checkbox("clicked", &enable_move);
    ImGui::Text("yaw: %.3f.", yaw);
    ImGui::Text("pitch: %.3f.", pitch);
    ImGui::Text("distance: %.3f.", distance);
}