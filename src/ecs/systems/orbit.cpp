#include "ecs/systems.hpp"

namespace systems::orbit {

void compute_orbit_from_tle(ECSTable &ecs) {
    for (size_t i = 0; i < ecs.size(); i++) {
        if ((ecs.bits[i].to_ulong() & DRAWABLE_ORBIT) != DRAWABLE_ORBIT) continue;

        TLE &tle = ecs.get_TLE(i);
        Orbit &orbit = ecs.get_Orbit(i);
        Rotation &rotation = ecs.get_Rotation(i);
        Epoch &epoch = ecs.get_Epoch(i);

        // ref: https://en.wikipedia.org/wiki/Orbital_elements#Euler_angle_transformations
        // note: Y and Z axis are swapped (we are in Y-up right-handed geometry, wikipedia is Z-up)
        // we compute the orbit base with the axis swapped, and re-swap them later
        glm::mat3 m1 = glm::mat3(
            cos(tle.argument_of_perigee), 0.0f, -sin(tle.argument_of_perigee),
            sin(tle.argument_of_perigee), 0.0f, cos(tle.argument_of_perigee),
            0.0f, 1.0f, 0.0f
        );

        glm::mat3 m2 = glm::mat3(
            1.0f, 0.0f, 0.0f,
            0.0f, -sin(tle.inclination), cos(tle.inclination),
            0.0f, cos(tle.inclination), sin(tle.inclination)
        );

        glm::mat3 m3 = glm::mat3(
            cos(tle.ascending_node_longitude), 0.0f, -sin(tle.ascending_node_longitude),
            sin(tle.ascending_node_longitude), 0.0f, cos(tle.ascending_node_longitude),  
            0.0f, 1.0f, 0.0f
        );

        rotation = m1 * m2 * m3;
        // re-swap Y and Z
        std::swap(rotation[2], rotation[1]);

        // thanks https://space.stackexchange.com/questions/18289/how-to-get-semi-major-axis-from-tle
        orbit.semi_major_axis = glm::pow(3.986004418*1e14, 1.0f / 3) / glm::pow(2.0f * tle.revloutions_per_day * M_PI / 86400.0f, 2.0f / 3.0f) / 1000.0f;
        orbit.semi_minor_axis = orbit.semi_major_axis * sqrt(1.0f - tle.eccentricity * tle.eccentricity);
        orbit.flag = 1;

        // jan 1 of epoch year (seconds per year isn't uniform)
        std::tm tm = {};
        tm.tm_year = tle.epoch_year - 1900;
        tm.tm_mon = 0;
        tm.tm_mday = 1;
        tm.tm_hour = 1;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        epoch = ((double) std::mktime(&tm)) + (tle.epoch_day - 1 + tle.epoch_frac) * 86400;
    }
}

void index_true_anomalies(ECSTable &ecs) {
    int max_iter = 20;
    double e = 0.0001;

    for (size_t i = 0; i < ecs.size(); i++) {
        if ((ecs.bits[i].to_ulong() & DRAWABLE_ORBIT) != DRAWABLE_ORBIT) continue;

        TLE &tle = ecs.get_TLE(i);
        Orbit &orbit = ecs.get_Orbit(i);
        AnomalyIndex &anomaly_index = ecs.get_AnomalyIndex(i);

        for (int angle = 0; angle < 360; angle++) {
            float tmp_mean_anomaly = glm::radians((float) angle);

            double anm_ecc = tle.eccentricity < 0.8 ? tmp_mean_anomaly : M_PI;

            double f = anm_ecc - tle.eccentricity * sin(anm_ecc) - tmp_mean_anomaly;
            int ite = 0;

            while (abs(f) > e) {
                f = anm_ecc - tle.eccentricity * sin(anm_ecc) - tmp_mean_anomaly;
                anm_ecc = anm_ecc - f / (1.0f - tle.eccentricity * cos(anm_ecc));

                ite += 1;
                if (ite > max_iter) {
                    std::cout << "Failing to converge true anomaly calculation for " << tle.name << std::endl;
                    break;
                }
            }

            anomaly_index[angle] = fmod(anm_ecc + M_PI * 2, M_PI * 2);
        }
    }
}

void compute_true_anomalies(ECSTable &ecs, double t) {
    for (size_t i = 0; i < ecs.size(); i++) {
        if ((ecs.bits[i].to_ulong() & DRAWABLE_ORBIT) != DRAWABLE_ORBIT) continue;

        TLE &tle = ecs.get_TLE(i);
        Orbit &orbit = ecs.get_Orbit(i);
        Epoch &epoch = ecs.get_Epoch(i);
        AnomalyIndex &anomaly_index = ecs.get_AnomalyIndex(i);

        double days_since_epoch = (t - epoch) / (24 * 3600);

        float real_time_mean_anomaly = tle.mean_anomaly + days_since_epoch * tle.revloutions_per_day * M_PI * 2;
        real_time_mean_anomaly = fmod(real_time_mean_anomaly, M_PI * 2);

        int index =  (int) glm::degrees(real_time_mean_anomaly);
        float delta = floor(glm::degrees(real_time_mean_anomaly)) - glm::degrees(real_time_mean_anomaly);

        float ta_i = anomaly_index[index];
        float ta_ip1 = index != 359 ? anomaly_index[(index+1) % 360] : M_PI * 2;

        orbit.true_anomaly = delta * ta_i + (1 - delta) * ta_ip1;
        orbit.true_anomaly = fmod(orbit.true_anomaly, M_PI * 2);
    }
}

void compute_pos_along_orbit(ECSTable &ecs) {
    for (size_t i = 0; i < ecs.size(); i++) {
        if ((ecs.bits[i].to_ulong() & DRAWABLE_ORBIT) != DRAWABLE_ORBIT) continue;

        Orbit &orbit = ecs.get_Orbit(i);
        Rotation &rota = ecs.get_Rotation(i);
        Position &position = ecs.get_Position(i);
        Rotation &rotation = ecs.get_Rotation(i);
        Parent &parent = ecs.get_Parent(i);

        if (ecs.bits[i].to_ulong() & (1 << PARENT)) {

            float linear_eccentricity = sqrt(orbit.semi_major_axis * orbit.semi_major_axis - orbit.semi_minor_axis * orbit.semi_minor_axis);
            orbit.offset = glm::vec3(-linear_eccentricity, 0, 0) * rotation + ecs.get_Position(parent);
        }

        int pos_index = (int) (orbit.true_anomaly / (M_PI * 2) * 360);
        float inter = (orbit.true_anomaly / (M_PI * 2) * 360) - floor(orbit.true_anomaly / (M_PI * 2) * 360);

        float angle = glm::radians((float) pos_index);
        float anglep1 = glm::radians((float) pos_index + 1);
        glm::vec3 prev = glm::vec3(glm::cos(angle), 0, -glm::sin(angle)) * glm::vec3(orbit.semi_major_axis, 0, orbit.semi_minor_axis) * rota + orbit.offset;
        glm::vec3 next = glm::vec3(glm::cos(anglep1), 0, -glm::sin(anglep1)) * glm::vec3(orbit.semi_major_axis, 0, orbit.semi_minor_axis) * rota + orbit.offset;
        
        position = inter * next + (1 - inter) * prev;
    }
}

void draw_orbits(Scene *scene, ECSTable &ecs) {
    size_t first = ecs.get_first(DRAWABLE_ORBIT);
    size_t last = ecs.get_last(DRAWABLE_ORBIT);
    render::orbit::draw(scene, ecs, first, last);
}

void filter_orbits(Scene *scene, ECSTable &ecs) {
    static ImGuiTextFilter filter;

    if (ImGui::InputTextWithHint("##Filter", "filter", filter.InputBuf, IM_ARRAYSIZE(filter.InputBuf))) {
        filter.Build();
    }

    for (size_t i = 0; i < ecs.size(); i++) {
        if ((ecs.bits[i].to_ulong() & DRAWABLE_ORBIT) != DRAWABLE_ORBIT) continue;

        TLE &tle = ecs.get_TLE(i);
        Orbit &orbit = ecs.get_Orbit(i);

        if (filter.PassFilter(tle.name.c_str())) {
            orbit.flag = 1;
            if (ImGui::TreeNode(fmt::format("{}##{}", tle.name, i).c_str())) {
                debug_entity(scene, ecs, i);
                orbit.flag = 2;
                ImGui::TreePop();
            }
        } else {
            orbit.flag = 0;
        }
    } 
}

} // namespace systems::orbit