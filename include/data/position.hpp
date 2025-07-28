#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>

#include "scene/time.hpp"
#include "data/horizons.hpp"
#include "data/db_manager.hpp"

using json = nlohmann::json;

class MajorBody_simu {
    private:
        DBManager db;
        json rotation_info;

        int id = -1;
        std::string name;

        // std::vector<glm::mat4> transforms;
        std::vector<glm::vec3> pos;
        long jan_1_ts;
    public:
        MajorBody_simu(int _id, DBManager &_db);

        void compute_transforms(int year);
        glm::vec3 get_pos(long timestamp);
        glm::mat3 get_rota(double julian_date);
};