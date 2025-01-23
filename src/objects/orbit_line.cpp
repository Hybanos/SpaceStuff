#include "orbit_line.hpp"
#include "scene/scene.hpp"

OrbitLine::OrbitLine(Scene *s, TLE t) : Object(s) {
    tle = t;
    matrix_id = glGetUniformLocation(scene->orbit_program_id, "MVP");
    base_id = glGetUniformLocation(scene->orbit_program_id, "base");
    offset_id = glGetUniformLocation(scene->orbit_program_id, "offset");
    a_id = glGetUniformLocation(scene->orbit_program_id, "a");
    b_id = glGetUniformLocation(scene->orbit_program_id, "b");

    draw_mesh = true;
    build_orbit();
    build();
    manage_m_buffers();
}

void OrbitLine::build_orbit() {
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
    base = m1 * m2 * m3;

    // thanks https://space.stackexchange.com/questions/18289/how-to-get-semi-major-axis-from-tle
    semi_major_axis = glm::pow(3.986004418*1e14, 1.0f / 3) / glm::pow(2.0f * tle.revloutions_per_day * M_PI / 86400, 2.0f / 3) / 1000;
    semi_minor_axis = semi_major_axis * sqrt(1 - tle.eccentricity * tle.eccentricity);

    linear_eccentricity = sqrt(semi_major_axis * semi_major_axis - semi_minor_axis * semi_minor_axis);
    offset = glm::vec3(-linear_eccentricity, 0, 0) * base;

    compute_true_anomaly();
}

void OrbitLine::build() {

    lines.clear();
    lines_colors.clear();

    float orbit_line_length = glm::min((float) 2 / tle.revloutions_per_day * 90, 180.0f);

    for (int i = 0; i < 360; i++) {
        float i_rad = glm::radians((float) i) + true_anomaly;
        float ip1_rad = glm::radians((float) i + 1) + true_anomaly;

        lines.push_back(glm::vec3(semi_major_axis * cos(i_rad), semi_minor_axis * sin(i_rad), 0));
        lines.push_back(glm::vec3(semi_major_axis * cos(ip1_rad), semi_minor_axis * sin(ip1_rad), 0));

        lines_colors.push_back(glm::vec4(i_rad, 0.0f, 0.0f, 0.0f));
        lines_colors.push_back(glm::vec4(ip1_rad, 0.0f, 0.0f, 0.0f));
    }
}

void OrbitLine::draw() {
    glUseProgram(scene->orbit_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &(scene->mvp)[0][0]);
    glUniformMatrix3fv(base_id, 1, GL_FALSE, &base[0][0]);
    glUniform3fv(offset_id, 1, &offset[0]);
    glUniform1f(a_id, true_anomaly);

    // build_orbit();
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
        ImGui::SliderFloat("Mean anomaly", &tle.mean_anomaly, 0, M_PI * 2);
        ImGui::DragFloat("Revs. per day", &tle.revloutions_per_day);
        ImGui::DragInt("Revs at epoch", &tle.revolutions_at_epoch);
        ImGui::Spacing();
        ImGui::Text("Semi major axis: %f", semi_major_axis);
        ImGui::Text("Semi minor axis: %f", semi_minor_axis);
        ImGui::Text("Linear eccentricity: %f", linear_eccentricity);
        ImGui::Text("True anomaly: %f", true_anomaly);

        build_orbit();
        build();
        manage_m_buffers();
    }
}

// big thanks https://github.com/duncaneddy/rastro/blob/main/rastro/src/orbits.rs
void OrbitLine::compute_true_anomaly() {
    int max_iter = 10;
    float e = 0.0001;

    float anm_ecc = tle.eccentricity < 0.8 ? tle.mean_anomaly : M_PI;

    float f = anm_ecc - tle.eccentricity * sin(anm_ecc) - tle.mean_anomaly;
    int ite = 0;

    while (abs(f) > e) {
        f = anm_ecc - tle.eccentricity * sin(anm_ecc) - tle.mean_anomaly;
        anm_ecc = anm_ecc - f / (1.0f - tle.eccentricity * cos(anm_ecc));

        ite += 1;
        if (ite > max_iter) {
            std::cout << "Failing to converge true anomaly calculation for " << tle.name << std::endl;
            break;
        }
    }

    true_anomaly = anm_ecc;
}