#include "ecs/systems.hpp"

void debug_entities(ECSTable &ecs) {
    ImGui::Begin("ECS");

    if (ImGui::BeginTable("ecs_entity_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable, ImVec2(0, 300))) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("bits");
        ImGui::TableHeadersRow();
        for (int i = 0; i < ecs.size; i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%lld", i);
            ImGui::TableNextColumn();
            ImGui::Text("%d", ecs.bits[i]);
        }
        ImGui::EndTable();
    };
    ImGui::End();
}