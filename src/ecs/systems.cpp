#include "ecs/systems.hpp"

namespace systems {

void compute_orbit_from_tle(ECSTable &ecs) {
    int flags = (1 << TWO_LINE_ELEMENT) | (1 << ORBIT) | (1 << ROTATION) | (1 << EPOCH);

    for (size_t i = 0; i < ecs.size; i++) {
        if ((ecs.bits[i] & flags) != flags) continue;

        TLE &tle = ((TLE *) ecs.component_table[TWO_LINE_ELEMENT])[i];
        Orbit &orbit = ((Orbit *) ecs.component_table[ORBIT])[i];
        Rotation &rotation = ((Rotation *) ecs.component_table[ROTATION])[i];
        Epoch &epoch = ((Epoch *) ecs.component_table[EPOCH])[i];

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

        float linear_eccentricity = sqrt(orbit.semi_major_axis * orbit.semi_major_axis - orbit.semi_minor_axis * orbit.semi_minor_axis);
        orbit.offset = glm::vec3(-linear_eccentricity, 0, 0) * rotation;

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
    int flags = (1 << TWO_LINE_ELEMENT) | (1 << ORBIT) | (1 << TRUE_ANOMALY_INDEX);
    int max_iter = 20;
    double e = 0.0001;

    for (size_t i = 0; i < ecs.size; i++) {
        if ((ecs.bits[i] & flags) != flags) continue;

        TLE &tle = ((TLE *) ecs.component_table[TWO_LINE_ELEMENT])[i];
        Orbit &orbit = ((Orbit *) ecs.component_table[ORBIT])[i];
        AnomalyIndex &anomaly_index = ((AnomalyIndex *) ecs.component_table[TRUE_ANOMALY_INDEX])[i];

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
    int flags = (1 << TWO_LINE_ELEMENT) | (1 << ORBIT) | (1 << EPOCH) | (1 << TRUE_ANOMALY_INDEX);

    for (size_t i = 0; i < ecs.size; i++) {
        if ((ecs.bits[i] & flags) != flags) continue;

        TLE &tle = ((TLE *) ecs.component_table[TWO_LINE_ELEMENT])[i];
        Orbit &orbit = ((Orbit *) ecs.component_table[ORBIT])[i];
        Epoch &epoch = ((Epoch *) ecs.component_table[EPOCH])[i];
        AnomalyIndex &anomaly_index = ((AnomalyIndex *) ecs.component_table[TRUE_ANOMALY_INDEX])[i];

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
    int flags = (1 << ORBIT) | (1 << ROTATION) | (1 << POSITION);

    for (size_t i = 0; i < ecs.size; i++) {
        if ((ecs.bits[i] & flags) != flags) continue;

        Orbit &orbit = ((Orbit *) ecs.component_table[ORBIT])[i];
        Rotation &rota = ((Rotation *) ecs.component_table[ROTATION])[i];
        Position &position = ((Position *) ecs.component_table[POSITION])[i];

        int pos_index = (int) (orbit.true_anomaly / (M_PI * 2) * 360);
        float inter = (orbit.true_anomaly / (M_PI * 2) * 360) - floor(orbit.true_anomaly / (M_PI * 2) * 360);

        float angle = glm::radians((float) pos_index);
        float anglep1 = glm::radians((float) pos_index + 1);
        glm::vec3 prev = glm::vec3(glm::cos(angle), 0, -glm::sin(angle)) * glm::vec3(orbit.semi_major_axis, 0, orbit.semi_minor_axis) * rota + orbit.offset;
        glm::vec3 next = glm::vec3(glm::cos(anglep1), 0, -glm::sin(anglep1)) * glm::vec3(orbit.semi_major_axis, 0, orbit.semi_minor_axis) * rota + orbit.offset;
        
        position = inter * next + (1 - inter) * prev;
    }
}

void debug_entities(ECSTable &ecs) {
    ImGui::Begin("ECS");

    static size_t selected = 0;
    if (ImGui::BeginTable("ecs_entity_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable, ImVec2(0, 300))) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("bits");
        ImGui::TableHeadersRow();
        for (size_t i = 0; i < ecs.size; i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            bool sel = (selected == i);
            ImGui::Selectable(fmt::format("{}", i).c_str(), &sel, ImGuiSelectableFlags_SpanAllColumns);
            if (sel) selected = i;
            ImGui::TableNextColumn();
            ImGui::Text("%d", ecs.bits[i]);
        }
        ImGui::EndTable();
    };

    ImGui::Spacing();

    int bits = ecs.bits[selected];
    ImGui::Text("Entity %lld:", selected);

    if (bits & (1 << POSITION)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Position");
        ImGui::DragFloat3("##1", &((Position *) ecs.component_table[POSITION])[selected].x);
    }

    if (bits & (1 << ROTATION)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Rotation");
        ImGui::DragFloat3("##2", &((glm::mat3 *) ecs.component_table[ROTATION])[selected][0][0]);
        ImGui::DragFloat3("##3", &((glm::mat3 *) ecs.component_table[ROTATION])[selected][1][0]);
        ImGui::DragFloat3("##4", &((glm::mat3 *) ecs.component_table[ROTATION])[selected][2][0]);
    }

    if (bits & (1 << SCALE)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Scale");
        ImGui::DragFloat("##5", &((float *)ecs.component_table[SCALE])[selected]);
    }

    if (bits & (1 << TWO_LINE_ELEMENT)) {
        ImGui::Spacing();
        ImGui::SeparatorText("TLE");

        TLE &tle = ((TLE *)ecs.component_table[TWO_LINE_ELEMENT])[selected];

        ImGui::Text(tle.name.c_str());
        ImGui::Text("Catalog number: %d", tle.cat_number);
        ImGui::Text("Catalog class: %c", tle.classification);
        ImGui::Text("Int. designator: %s", tle.international_designator);
        ImGui::DragInt("Epoch year", &tle.epoch_year);
        ImGui::DragInt("Epoch day", &tle.epoch_day);
        ImGui::DragFloat("Epoch frac", &tle.epoch_frac);
        ImGui::Spacing();
        ImGui::SliderFloat("Inclination", &tle.inclination, 0, M_PI * 2);
        ImGui::SliderFloat("Ascending node", &tle.ascending_node_longitude, 0, M_PI * 2);
        ImGui::SliderFloat("Eccentricity", &tle.eccentricity, 0, 1);
        ImGui::SliderFloat("Argument of perigee", &tle.argument_of_perigee, 0, M_PI * 2);
        ImGui::SliderFloat("Mean anomaly (at epoch)", &tle.mean_anomaly, 0, M_PI * 2);
        ImGui::DragFloat("Revs. per day", &tle.revloutions_per_day);
        ImGui::DragInt("Revs at epoch", &tle.revolutions_at_epoch);
    }

    if (bits & (1 << ORBIT)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Orbit");

        Orbit &orbit = ((Orbit *)ecs.component_table[ORBIT])[selected];

        ImGui::DragFloat("Semi major axis", &orbit.semi_major_axis);
        ImGui::DragFloat("Semi minor axis", &orbit.semi_minor_axis);
        ImGui::Text("True anomaly: %f", orbit.true_anomaly);
        ImGui::Text("True to mean anomalies:");
        ImGui::DragFloat3("Offset", &orbit.offset[0]);
        ImGui::InputFloat("Flag", &orbit.flag);
    }

    if (bits & (1 << EPOCH)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Epoch");

        Epoch &epoch = ((Epoch *) ecs.component_table[EPOCH])[selected];

        float tmp = epoch;
        ImGui::DragFloat("Epoch", &tmp);
        epoch = tmp;
    }

    if (bits & (1 << TRUE_ANOMALY_INDEX)) {
        ImGui::Spacing();
        ImGui::SeparatorText("True anomaly index");

        AnomalyIndex &index = ((AnomalyIndex *) ecs.component_table[TRUE_ANOMALY_INDEX])[selected];

        ImGui::PlotLines("haha", index.begin(), 360, 0, NULL, 0, M_PI * 2, ImVec2(0, 80));
    }

    ImGui::End();
}

} // namespace systems