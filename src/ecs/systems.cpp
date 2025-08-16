#include "ecs/systems.hpp"

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
        ImGui::DragFloat3("", &((Position *) ecs.component_table[POSITION])[selected].x);
    }

    if (bits & (1 << ROTATION)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Rotation");
        ImGui::DragFloat3("", &((glm::mat3 *) ecs.component_table[ROTATION])[selected][0][0]);
        ImGui::DragFloat3("", &((glm::mat3 *) ecs.component_table[ROTATION])[selected][1][0]);
        ImGui::DragFloat3("", &((glm::mat3 *) ecs.component_table[ROTATION])[selected][2][0]);
    }

    if (bits & (1 << SCALE)) {
        ImGui::Spacing();
        ImGui::SeparatorText("Scale");
        ImGui::DragFloat("", &((float *)ecs.component_table[SCALE])[selected]);
    }

    if (bits & (1 << TWO_LINE_ELEMENT)) {
        ImGui::Spacing();
        ImGui::SeparatorText("TLE");

        TLE &tle = ((TLE *)ecs.component_table[TWO_LINE_ELEMENT])[selected];

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
        float tmp = orbit.epoch;
        ImGui::DragFloat("Epoch", &tmp);
        orbit.epoch = tmp;
        ImGui::Text("True anomaly: %f", orbit.true_anomaly);
        ImGui::Text("True to mean anomalies:");
        ImGui::PlotLines("haha", orbit.true_anomaly_index, 360, 0, NULL, 0, M_PI * 2, ImVec2(0, 80));
        ImGui::DragFloat3("Offset", &orbit.offset[0]);
        ImGui::InputFloat("Flag", &orbit.flag);
    }

    ImGui::End();
}