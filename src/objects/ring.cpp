#include "objects/ring.hpp"
#include "scene/scene.hpp"

Ring::Ring(Scene *s, std::string body_name) : 
Object(s),
mesh(scene->ring_shader) {
    name = body_name;
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){return std::tolower(c);}); 

    ring_info = json::parse(std::ifstream("assets/rings.json"))[name];

    if (ring_info.is_null()) ring_info = json::array();

    for (auto &j : ring_info) {
        if (j["name"].is_null()) j["name"] = "Unknown ring";
        if (j["body_distance"].is_null()) j["body_distance"] = 0; 
        if (j["width"].is_null()) j["width"] = 0;
        if (j["thickness"].is_null()) j["thickness"] = 0;
        if (j["transmittance"].is_null()) j["transmittance"] = 0.0; 
        if (j["mass"].is_null()) j["mass"] = -1; 
        if (j["gaps"].is_null()) j["gaps"] = json::array();

        for (auto &g : j["gaps"]) {
            if (g["name"].is_null()) g["name"] = "Unknow gap";
            if (g["body_distance"].is_null()) g["body_distance"] = 0;
            if (g["width"].is_null()) g["width"] = 0;
        }
    }

    build();

    mesh.gen_texture("assets/perlin-512_6-oct.png");
}

void Ring::build() {
    triangles.clear();
    triangles.reserve(360 * 6 * ring_info.size());
    std::vector<glm::vec3> points;
    points.resize(360);
    transmittance.clear();
    radius.clear();

    for (int i = 0; i < 360; i++) {
        float i_rad = glm::radians((float) i);
        points[i] = rota * glm::vec3(glm::cos(i_rad), 0, glm::sin(i_rad));
    }

    json ring_with_gaps = json::array();

    for (auto j : ring_info) {
        int inner = j["body_distance"];
        int outer = inner + (int) j["width"];

        for (auto &g : j["gaps"]) {
            outer = g["body_distance"];
            j["body_distance"] = inner;
            j["width"] = outer - inner;
            ring_with_gaps.push_back(j);
            inner = outer;
        }

        ring_with_gaps.push_back(j);
    }

    int index = 0;
    for (auto &j : ring_with_gaps) {
        float inner = j["body_distance"];
        float outer = (int) j["body_distance"] + (int) j["width"];

        for (int i = 0; i < 360; i++) {
            triangles.push_back(points[i] * inner);
            triangles.push_back(points[i] * outer);
            triangles.push_back(points[(i + 1) % 360] * outer);

            triangles.push_back(points[i] * inner);
            triangles.push_back(points[(i + 1) % 360] * outer);
            triangles.push_back(points[(i + 1) % 360] * inner);

            float t = j["transmittance"];

            transmittance.push_back(t);
            transmittance.push_back(t);
            transmittance.push_back(t);
            transmittance.push_back(t);
            transmittance.push_back(t);
            transmittance.push_back(t);

            radius.push_back(inner);
            radius.push_back(outer);
            radius.push_back(outer);
            radius.push_back(inner);
            radius.push_back(outer);
            radius.push_back(inner);
        }
        index++;
    }

}

void Ring::draw() {
    build();

    mesh.set_mat4("MVP", scene->mvp);
    mesh.set_vec3("pos", pos - scene->camera->get_center());

    mesh.set_buffer(0, triangles);
    mesh.set_buffer(1, transmittance);
    mesh.set_buffer(2, radius);

    mesh.draw(GL_TRIANGLES, 0, triangles.size());
}

void Ring::debug() {
    if (ImGui::TreeNode(("Ring: " + name).c_str())) {

        int i = 0;
        for (auto &j : ring_info) {
            static bool sel = false;
            ImGui::Text(((std::string) j["name"]).c_str());
            ImGui::Text("Distance: %d", (int) j["body_distance"]);
            ImGui::Text("Width: %d", (int) j["width"]);
            ImGui::Text("Thickness: %d", (int) j["thickness"]);
            ImGui::Text("transmittance: %f", (double) j["transmittance"]);
            ImGui::Text("mass: %d", (int) j["mass"]);
            for (auto &g : j["gaps"]) {
                if (ImGui::TreeNode(((std::string) g["name"]).c_str())) {
                    ImGui::Text("Distance: %d", (int) g["body_distance"]);
                    ImGui::Text("width: %d", (int) g["width"]);
                    ImGui::TreePop();
                }
            }
            ImGui::NewLine();
            i++;
        }

        ImGui::TreePop();
    }
}
