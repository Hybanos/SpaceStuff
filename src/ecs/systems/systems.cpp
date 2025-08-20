#include "ecs/systems.hpp"

void debug_json(nlohmann::ordered_json obj) {
    if (obj.is_array()) {
        int i = 0;
        for (auto it = obj.begin(); it != obj.end(); it++) {
            if (ImGui::TreeNode(fmt::format("{}", i).c_str())) {
                debug_json(it.value());
                ImGui::TreePop();
            }
            i++;
        }
    }

    if (obj.is_object()) {
        for (auto it = obj.begin(); it != obj.end(); it++) {
            if (ImGui::TreeNode(it.key().c_str())) {
                debug_json(it.value());
                ImGui::TreePop();
            }
        }
    }

    std::string fmt;
    if (obj.is_null()) fmt = fmt::format("NULL");
    if (obj.is_boolean()) fmt = fmt::format("{}", (bool) obj);
    if (obj.is_number_integer()) fmt = fmt::format("{}", (int) obj);
    if (obj.is_number_float()) fmt = fmt::format("{}", (float) obj);
    if (obj.is_string()) fmt = fmt::format("{}", (std::string) obj);

    ImGui::Text(fmt.c_str());
}

namespace systems {

void debug_entity(Scene *scene, ECSTable &ecs, size_t entity_id) {
        int bits = ecs.bits[entity_id];
        if (bits & (1 << POSITION)) {
            ImGui::Spacing();
            ImGui::SeparatorText("Position");
            ImGui::DragFloat3("##1", &((Position *) ecs.component_table[POSITION])[entity_id].x);
        }

        if (bits & (1 << ROTATION)) {
            ImGui::Spacing();
            ImGui::SeparatorText("Rotation");
            glm::mat3 &rota = ((glm::mat3 *) ecs.component_table[ROTATION])[entity_id];
            ImGui::Text("\t%f\t%f\t%f", rota[0][0], rota[1][0], rota[2][0]);
            ImGui::Text("\t%f\t%f\t%f", rota[0][1], rota[1][1], rota[2][1]);
            ImGui::Text("\t%f\t%f\t%f", rota[0][2], rota[1][2], rota[2][2]);
            // ImGui::DragFloat3("##3", &rota[0][0]);
            // ImGui::DragFloat3("##3", &rota[1][0]);
            // ImGui::DragFloat3("##4", &rota[2][0]);
        }

        if (bits & (1 << SCALE)) {
            ImGui::Spacing();
            ImGui::SeparatorText("Scale");
            ImGui::DragFloat("##5", &((float *)ecs.component_table[SCALE])[entity_id]);
        }

        if (bits & (1 << MAJOR_BODY)) {
            ImGui::Spacing();
            ImGui::SeparatorText("Major Body");

            MajorBody &mb = ((MajorBody *)ecs.component_table[MAJOR_BODY])[entity_id];

            ImGui::DragInt("Major Body ID", &mb.major_body_id);
            ImGui::Text("Body name: %s", mb.name);
            ImGui::Text("Designation: %s", mb.designation);
            ImGui::Text("Alias: %s", mb.alias);
            float tmp = mb.mass;
            ImGui::DragFloat("Mass", &tmp);
            mb.mass = tmp;
            tmp = mb.heliocentric_gravitaional_constant;
            ImGui::DragFloat("Gravitational Constant", &tmp);
            mb.heliocentric_gravitaional_constant = tmp;
            tmp = mb.radius;
            ImGui::DragFloat("Radius", &tmp);
            mb.radius = tmp;
        }
        
        if (bits & (1 << EPHEMERIS)) {
            ImGui::Spacing();
            ImGui::SeparatorText("Ephemeris lines");

            Ephemeris &ephemeris = ((Ephemeris *)ecs.component_table[EPHEMERIS])[entity_id];
            int line_count = sizeof(Ephemeris) / sizeof(EphemerisLine);

            if (ImGui::BeginTable("Lines", 9,  ImGuiTableFlags_ScrollX | ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable, ImVec2(0, 300))) {
                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Timestamp");
                ImGui::TableSetupColumn("Julian Day");
                ImGui::TableSetupColumn("x");
                ImGui::TableSetupColumn("y");
                ImGui::TableSetupColumn("z");
                ImGui::TableSetupColumn("vx");
                ImGui::TableSetupColumn("vy");
                ImGui::TableSetupColumn("vz");
                ImGui::TableHeadersRow();
                for (int i = 0; i < line_count; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", ephemeris[i].id);
                    ImGui::TableNextColumn();
                    ImGui::Text("%ld", ephemeris[i].timestamp);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", ephemeris[i].julian_day_number);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", ephemeris[i].x);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", ephemeris[i].y);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", ephemeris[i].z);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", ephemeris[i].vx);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", ephemeris[i].vy);
                    ImGui::TableNextColumn();
                    ImGui::Text("%f", ephemeris[i].vz);
                    ImGui::TableNextColumn();
                }
                ImGui::EndTable();
            }
        }

        if (bits & (1 << TWO_LINE_ELEMENT)) {
            ImGui::Spacing();
            ImGui::SeparatorText("TLE");

            TLE &tle = ((TLE *)ecs.component_table[TWO_LINE_ELEMENT])[entity_id];

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

            Orbit &orbit = ((Orbit *)ecs.component_table[ORBIT])[entity_id];

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

            Epoch &epoch = ((Epoch *) ecs.component_table[EPOCH])[entity_id];

            float tmp = epoch;
            ImGui::DragFloat("Epoch", &tmp);
            epoch = tmp;
        }

        if (bits & (1 << TRUE_ANOMALY_INDEX)) {
            ImGui::Spacing();
            ImGui::SeparatorText("True anomaly index");

            AnomalyIndex &index = ((AnomalyIndex *) ecs.component_table[TRUE_ANOMALY_INDEX])[entity_id];

            ImGui::PlotLines("haha", index.begin(), 360, 0, NULL, 0, M_PI * 2, ImVec2(0, 80));
        }

        if (bits & (1 << ROTATION_INFO)) {
            ImGui::Spacing();
            ImGui::SeparatorText("Rotation Info");

            RotationInfo &info = ((RotationInfo *) ecs.component_table[ROTATION_INFO])[entity_id];

            debug_json(info);
        }
}

void debug_entities(Scene *scene, ECSTable &ecs) {
    ImGui::Begin("ECS");
    ImGui::BeginTabBar("ECS tabs");

    if (ImGui::BeginTabItem("ECS table")) {
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

        ImGui::Text("Entity %lld:", selected);
        debug_entity(scene, ecs, selected);

        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Orbit filter")) {
        systems::orbit::filter_orbits(scene, ecs);
        ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();
}

} // namespace systems