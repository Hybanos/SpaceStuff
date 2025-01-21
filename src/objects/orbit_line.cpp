#include "orbit_line.hpp"
#include "scene/scene.hpp"

OrbitLine::OrbitLine(Scene *s, TLE t) : Object(s) {
    tle = t;

    draw_mesh = true;
    build();
    manage_m_buffers();
}

void OrbitLine::build() {
    lines.clear();
    lines_colors.clear();

    glm::mat3 m1 = glm::mat3(
        glm::vec3(cos(tle.argument_of_perigee), -sin(tle.argument_of_perigee), 0.0f),
        glm::vec3(sin(tle.argument_of_perigee), cos(tle.argument_of_perigee), 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    glm::mat3 m2 = glm::mat3(
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, cos(tle.inclination), -sin(tle.inclination)),
        glm::vec3(0.0f, sin(tle.inclination), cos(tle.inclination))
    );

    glm::mat3 m3 = glm::mat3(
        glm::vec3(sin(tle.ascending_node_longitude), cos(tle.ascending_node_longitude), 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(cos(tle.ascending_node_longitude), -sin(tle.ascending_node_longitude), 0.0f)  
    );

    // thanks https://en.wikipedia.org/wiki/Orbital_elements#Euler_angle_transformations
    glm::mat3 base = m1 * m2 * m3;


    // thanks https://space.stackexchange.com/questions/18289/how-to-get-semi-major-axis-from-tle
    float semi_major_axis = glm::pow(3.986004418*1e14, 1.0f / 3) / glm::pow(2.0f * tle.revloutions_per_day * M_PI / 86400, 2.0f / 3) / 1000;
    float semi_minor_axis = semi_major_axis * sqrt(1 - tle.eccentricity * tle.eccentricity);

    float linear_eccentricity = sqrt(semi_major_axis * semi_major_axis - semi_minor_axis * semi_minor_axis);
    glm::vec3 offset = glm::vec3(-linear_eccentricity, 0, 0) * base;

    for (int i = 0; i < 360; i++) {

        float i_rad = glm::radians((float) i);
        float ip1_rad = glm::radians((float) i + 1);

        lines.push_back(glm::vec3(semi_major_axis * cos(i_rad), semi_minor_axis * sin(i_rad), 0) * base + offset);
        lines.push_back(glm::vec3(semi_major_axis * cos(ip1_rad), semi_minor_axis * sin(ip1_rad), 0) * base + offset);

        lines_colors.push_back(glm::vec3((float) i / 360, (float) i / 360, 1.0f));
        lines_colors.push_back(glm::vec3((float) i / 360, (float) i / 360, 1.0f));
    }
}

void OrbitLine::draw() {
    glUseProgram(scene->base_program_id);

    // build();
    // manage_m_buffers();
    draw_m();
}

void OrbitLine::debug() {
    if (ImGui::CollapsingHeader(tle.name.data())) {
        ImGui::Text("Catalog number: %d", tle.cat_number);
        ImGui::Text("Catalog class: %c", tle.classification);
        ImGui::Text("Int. designator: ", tle.international_designator);
        ImGui::DragInt("Epoch year", &tle.epoch_year);
        ImGui::DragInt("Epoch day", &tle.epoch_day);
        ImGui::DragFloat("Epoch frac", &tle.epoch_frac);
        ImGui::Spacing();
        ImGui::SliderFloat("Inclination", &tle.inclination, 0, M_PI * 2);
        ImGui::SliderFloat("Ascending node", &tle.ascending_node_longitude, 0, M_PI * 2);
        ImGui::SliderFloat("Eccentricity", &tle.eccentricity, 0, 1);
        ImGui::SliderFloat("Argument of perigee", &tle.argument_of_perigee, 0, M_PI * 2);
        ImGui::DragFloat("Mean anomaly", &tle.mean_anomaly);
        ImGui::DragFloat("Revs. per day", &tle.revloutions_per_day);
        ImGui::DragInt("Revs at epoch", &tle.revolutions_at_epoch);
    }
}