#include "objects/orbit_line.hpp"
#include "scene/scene.hpp"

OrbitLine::OrbitLine(Scene *s, TLE t) : Object(s), 
    periapsis(scene, glm::vec3(0, 0, 0), glm::vec4(0.6, 0.6, 1, 1)),
    apoapsis(scene, glm::vec3(0, 0, 0), glm::vec4(0.6, 1, 0.6, 1)) {
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

    // periapsis = Particle(scene, glm::vec3(0, 0, 0), glm::vec4(1));
}

void OrbitLine::build_orbit() {

    // regular y and z
    // glm::mat3 m1 = glm::mat3(
    //     cos(tle.argument_of_perigee), -sin(tle.argument_of_perigee), 0.0f,
    //     sin(tle.argument_of_perigee), cos(tle.argument_of_perigee) , 0.0f,
    //     0.0f, 0.0f, 1.0f
    // );

    // glm::mat3 m2 = glm::mat3(
    //     1.0f, 0.0f, 0.0f,
    //     0.0f, cos(tle.inclination), -sin(tle.inclination),
    //     0.0f, sin(tle.inclination), cos(tle.inclination)
    // );

    // glm::mat3 m3 = glm::mat3(
    //     cos(tle.ascending_node_longitude), -sin(tle.ascending_node_longitude), 0.0f,
    //     sin(tle.ascending_node_longitude), cos(tle.ascending_node_longitude) , 0.0f,  
    //     0.0f, 0.0f, 1.0f
    // );

    // swapped y and z
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

    // thanks https://en.wikipedia.org/wiki/Orbital_elements#Euler_angle_transformations
    base = m1 * m2 * m3;

    std::swap(base[2], base[1]);

    // thanks https://space.stackexchange.com/questions/18289/how-to-get-semi-major-axis-from-tle
    semi_major_axis = glm::pow(3.986004418*1e14, 1.0f / 3) / glm::pow(2.0f * tle.revloutions_per_day * M_PI / 86400.0f, 2.0f / 3.0f) / 1000.0f;
    semi_minor_axis = semi_major_axis * sqrt(1.0f - tle.eccentricity * tle.eccentricity);

    linear_eccentricity = sqrt(semi_major_axis * semi_major_axis - semi_minor_axis * semi_minor_axis);
    offset = glm::vec3(-linear_eccentricity, 0, 0) * base;

    compute_true_anomaly();
}

void OrbitLine::build() {

    lines.clear();
    lines_colors.clear();

    for (int i = 0; i < 360; i++) {
        float i_rad = glm::radians((float) i);
        float ip1_rad = glm::radians((float) i + 1);

        lines.push_back(glm::vec3(semi_major_axis * cos(i_rad), 0, semi_minor_axis * -sin(i_rad)) * base + offset);
        lines.push_back(glm::vec3(semi_major_axis * cos(ip1_rad), 0, semi_minor_axis * -sin(ip1_rad)) * base + offset);

        lines_colors.push_back(glm::vec4(i_rad, 0.0f, 0.0f, 0.0f));
        lines_colors.push_back(glm::vec4(ip1_rad, 0.0f, 0.0f, 0.0f));
    }
}

void OrbitLine::draw() {
    glUseProgram(scene->orbit_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &(scene->mvp)[0][0]);
    glUniformMatrix3fv(base_id, 1, GL_FALSE, &base[0][0]);
    glUniform3fv(offset_id, 1, &offset[0]);

    // jan 1 of epoch year (seconds per year isn't uniform)
    std::tm tm = {};
    tm.tm_year = tle.epoch_year - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = 1;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    double epoch = ((double) std::mktime(&tm)) + (tle.epoch_day - 1 + tle.epoch_frac) * 86400;
    // std::cout << "haha   " << (int) epoch << std::endl;
    double delta = (double) time(NULL) - epoch;
    // std::cout << time(NULL) << " " << epoch << " " << delta << std::endl;
    double days_since_epoch = delta / SECS_DAY;
    // std::cout << days_since_epoch << std::endl;

    // double days_since_epoch = (double) time(NULL) / (24 * 3600);
    // double epoch = (double) (tle.epoch_year - 1970) * 365.242189 + tle.epoch_day - 1 + tle.epoch_frac;
    // std::cout << epoch << " " << days_since_epoch << std::endl;
    // days_since_epoch -= epoch;
    // std::cout << days_since_epoch << "haha " << std::endl;
    real_time_mean_anomaly = tle.mean_anomaly + days_since_epoch * tle.revloutions_per_day * M_PI * 2;
    real_time_mean_anomaly = fmod(real_time_mean_anomaly, M_PI * 2);

    compute_true_anomaly();

    int pos_index = (int) (true_anomaly / (M_PI * 2) * 360) * 2;
    float inter = (true_anomaly / (M_PI * 2) * 360) - floor(true_anomaly / (M_PI * 2) * 360);

    glm::vec3 prev = lines[pos_index];
    glm::vec3 next = lines[(pos_index + 2) % (360 * 2)];

    pos = inter * next + (1 - inter) * prev;

    // pos = lines[(int) (true_anomaly / (M_PI * 2) * 360) * 2];

    // true_anomaly = real_time_mean_anomaly;
    // true_anomaly = tle.mean_anomaly;
    glUniform1f(a_id, true_anomaly);
    draw_m();

    periapsis.pos = lines[0];
    periapsis.draw();
    apoapsis.pos = lines[360];
    apoapsis.draw();
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
        ImGui::Text("Mean anomaly: %f", real_time_mean_anomaly);
        ImGui::Text("True anomaly: %f", true_anomaly);
        ImGui::Spacing();
        ImGui::Text("\tx\ty\tz");
        ImGui::Text("\t%f\t%f\t%f", base[0][0], base[1][0], base[2][0]);
        ImGui::Text("\t%f\t%f\t%f", base[0][1], base[1][1], base[2][1]);
        ImGui::Text("\t%f\t%f\t%f", base[0][2], base[1][2], base[2][2]);
        ImGui::Spacing();
        ImGui::Text("x: %f, y:%f, z:%f", pos.x, pos.y, pos.z);
        compute_pitch_yaw();
        ImGui::Text("pitch: %f yaw: %f", angle[0], angle[1]);
        ImGui::Text("pitch: %f yaw: %f", angle[0] / M_PI * 180, angle[1] / M_PI * 180);
        ImGui::Text("Lat: %f°", pos.y / 6371 * 90);
        ImGui::Text("Altitude: %f", (float) glm::length(pos) - 6371);
        build_orbit();
        build();
        manage_m_buffers();

        periapsis.debug();
    }
}

// big thanks https://github.com/duncaneddy/rastro/blob/main/rastro/src/orbits.rs
void OrbitLine::compute_true_anomaly() {
    int max_iter = 10;
    double e = 0.0001;

    double anm_ecc = tle.eccentricity < 0.8 ? real_time_mean_anomaly : M_PI;

    double f = anm_ecc - tle.eccentricity * sin(anm_ecc) - real_time_mean_anomaly;
    int ite = 0;

    while (abs(f) > e) {
        f = anm_ecc - tle.eccentricity * sin(anm_ecc) - real_time_mean_anomaly;
        anm_ecc = anm_ecc - f / (1.0f - tle.eccentricity * cos(anm_ecc));

        ite += 1;
        if (ite > max_iter) {
            std::cout << "Failing to converge true anomaly calculation for " << tle.name << std::endl;
            break;
        }
    }

    true_anomaly = fmod(anm_ecc + M_PI * 2, M_PI * 2);
}

void OrbitLine::compute_pitch_yaw() {
    float yaw = atan2(pos.z, pos.x);
    float pitch = atan2(pos.y, sqrt(pos.x * pos.x + pos.z * pos.z));

    angle[0] = pitch;
    angle[1] = yaw;
}