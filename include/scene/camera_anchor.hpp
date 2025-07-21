#pragma once

#include "glm/glm.hpp"

class CameraAnchor {
    private:
    public:
        virtual glm::vec3 get_camera_center() = 0;
};

class SimpleAnchor : virtual public CameraAnchor {
    private:
        glm::vec3 center = glm::vec3(0, 0, 0);
    public:
        void look_at(glm::vec3 c) {
            center = c;
        }

        glm::vec3 get_camera_center() {
            return center;
        }
};