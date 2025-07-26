#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

class MajorBody {
    private:
        int id = -1;
        std::vector<glm::vec3> pos;
        glm::mat3 rota;
    public:
        MajorBody(int _id);
        glm::mat4 get_transform();
};