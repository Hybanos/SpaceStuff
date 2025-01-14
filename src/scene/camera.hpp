#ifndef SPACESTUFF_CAMERA_HPP
#define SPACESTUFF_CAMERA_HPP

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
    private:
        glm::vec3 position;
        glm::vec3 center;
    public:
        void translate(glm::vec3);
        void go_to(glm::vec3);
        void look_at(glm::vec3);
        glm::mat4 get_view();
};

#endif