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

    if (bits & (1 << DISPLAY_NAME)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Name");
        DisplayName &name = ecs.get_DisplayName(entity_id);

        char buf[256];
        strcpy(buf, name.c_str());
        ImGui::InputText("Name", buf, 256);
        name = buf;
    }

    if (bits & (1 << PARENT)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Position");
        Parent &parent = ecs.get_Parent(entity_id);

        ImGui::DragScalar("Parent", ImGuiDataType_U64, &parent);
        if (ImGui::TreeNode("Show Parent:")) {
            debug_entity(scene, ecs, parent);
            ImGui::TreePop();
        }
    }

    if (bits & (1 << POSITION)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Position");
        ImGui::DragFloat3("##1", &ecs.get_Position(entity_id).x);
        if (ImGui::Button("Follow")) {
            scene->follow_entity = entity_id; 
        }
    }

    if (bits & (1 << ROTATION)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Rotation");
        Rotation &rota = ecs.get_Rotation(entity_id);
        ImGui::Text("\t%f\t%f\t%f", rota[0][0], rota[1][0], rota[2][0]);
        ImGui::Text("\t%f\t%f\t%f", rota[0][1], rota[1][1], rota[2][1]);
        ImGui::Text("\t%f\t%f\t%f", rota[0][2], rota[1][2], rota[2][2]);
    }

    if (bits & (1 << SCALE)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Scale");
        ImGui::DragFloat("##5", &ecs.get_Scale(entity_id));
    }

    if (bits & (1 << MAJOR_BODY)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Major Body");

        MajorBody &mb = ecs.get_MajorBody(entity_id);

        ImGui::DragInt("Major Body ID", &mb.major_body_id);
        ImGui::Text("Body name: %s", mb.name);
        ImGui::Text("Designation: %s", mb.designation);
        ImGui::Text("Alias: %s", mb.alias);
        ImGui::DragScalar("Mass", ImGuiDataType_Double, &mb.mass);
        ImGui::DragScalar("Gravitational Constant", ImGuiDataType_Double, &mb.heliocentric_gravitaional_constant);
        ImGui::DragScalar("Radius", ImGuiDataType_Double, &mb.radius);
    }
    
    if (bits & (1 << EPHEMERIS)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Ephemeris lines");

        Ephemeris &ephemeris = ecs.get_Ephemeris(entity_id);
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

        TLE &tle = ecs.get_TLE(entity_id);

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

        Orbit &orbit = ecs.get_Orbit(entity_id);

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

        Epoch &epoch = ecs.get_Epoch(entity_id);

        ImGui::DragScalar("Epoch", ImGuiDataType_Double, &epoch);
    }

    if (bits & (1 << TRUE_ANOMALY_INDEX)) {
        ImGui::Spacing();
        ImGui::SeparatorText("True anomaly index");

        AnomalyIndex &index = ecs.get_AnomalyIndex(entity_id);

        ImGui::PlotLines("haha", index.begin(), 360, 0, NULL, 0, M_PI * 2, ImVec2(0, 80));
    }

    if (bits & (1 << ROTATION_INFO)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Rotation Info");

        RotationInfo &info = ecs.get_RotationInfo(entity_id);

        debug_json(info);
    }

    if (bits & (1 << RING)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Ring");

        Ring &ring = ecs.get_Ring(entity_id);

        ImGui::DragFloat2("Range", &ring.range[0], 10.0);
        ImGui::DragFloat("Transmittance", &ring.transmittance, 0.01f, 0, 1);
        ImGui::Checkbox("Selected", (bool *) &ring.selected);

        ImGui::BeginChild("Bitset", ImVec2(0, 100), ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
        for (int i = 0; i < ring.bits.size(); i++) {
            bool tmp = ring.bits.test(i);
            if (ImGui::Selectable(fmt::format("##{}", i).c_str(), &tmp, 0, ImVec2(0.1, 50))) {
                ring.bits.set(i, tmp);
            }
            ImGui::SameLine();
        }
        ImGui::EndChild();
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

size_t get_major_body_by_name(ECSTable &ecs, std::string name) {
    for (size_t i = 0; i < ecs.size; i++) {
        if (!(ecs.bits[i] & (1 << MAJOR_BODY))) continue;

        MajorBody &mb = ecs.get_MajorBody(i);

        std::string tmp = mb.name;
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char c){return std::tolower(c);}); 
        if (tmp == name) return i;
    }

    return -1;
}

} // namespace systems