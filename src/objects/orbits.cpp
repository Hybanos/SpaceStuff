#include "objects/orbits.hpp"
#include "scene/scene.hpp"

Orbits::Orbits(Scene *s, std::vector<TLE>& t) : Object(s), 
    periapsis(scene, glm::vec3(0, 0, 0), glm::vec4(0.6, 0.6, 1, 1)),
    apoapsis(scene, glm::vec3(0, 0, 0), glm::vec4(0.6, 1, 0.6, 1)) {
    tle = t;
    matrix_id = glGetUniformLocation(scene->orbits_program_id, "MVP");
    base_id = glGetUniformLocation(scene->orbits_program_id, "base");
    offset_id = glGetUniformLocation(scene->orbits_program_id, "offset");
    a_id = glGetUniformLocation(scene->orbits_program_id, "a");
    b_id = glGetUniformLocation(scene->orbits_program_id, "b");

    draw_mesh = true;

    for (int i = 0; i < tle.size(); i++) {
        build_orbit(i);
        build(i);
    }

    manage_m_buffers();
}

void Orbits::build_orbit(int i) {
    // ref: https://en.wikipedia.org/wiki/Orbital_elements#Euler_angle_transformations
    // note: Y and Z axis are swapped (we are in Y-up right-handed geometry, wikipedia is Z-up)
    // we compute the orbit base with the axis swapped, and re-swap them later
    glm::mat3 m1 = glm::mat3(
        cos(tle[i].argument_of_perigee), 0.0f, -sin(tle[i].argument_of_perigee),
        sin(tle[i].argument_of_perigee), 0.0f, cos(tle[i].argument_of_perigee),
        0.0f, 1.0f, 0.0f
    );

    glm::mat3 m2 = glm::mat3(
        1.0f, 0.0f, 0.0f,
        0.0f, -sin(tle[i].inclination), cos(tle[i].inclination),
        0.0f, cos(tle[i].inclination), sin(tle[i].inclination)
    );

    glm::mat3 m3 = glm::mat3(
        cos(tle[i].ascending_node_longitude), 0.0f, -sin(tle[i].ascending_node_longitude),
        sin(tle[i].ascending_node_longitude), 0.0f, cos(tle[i].ascending_node_longitude),  
        0.0f, 1.0f, 0.0f
    );

    base = m1 * m2 * m3;
    // re-swap Y and Z
    std::swap(base[2], base[1]);

    // thanks https://space.stackexchange.com/questions/18289/how-to-get-semi-major-axis-from-tle
    semi_major_axis = glm::pow(3.986004418*1e14, 1.0f / 3) / glm::pow(2.0f * tle[i].revloutions_per_day * M_PI / 86400.0f, 2.0f / 3.0f) / 1000.0f;
    semi_minor_axis = semi_major_axis * sqrt(1.0f - tle[i].eccentricity * tle[i].eccentricity);

    linear_eccentricity = sqrt(semi_major_axis * semi_major_axis - semi_minor_axis * semi_minor_axis);
    offset = glm::vec3(-linear_eccentricity, 0, 0) * base;

    // jan 1 of epoch year (seconds per year isn't uniform)
    std::tm tm = {};
    tm.tm_year = tle[i].epoch_year - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = 1;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    epoch = ((double) std::mktime(&tm)) + (tle[i].epoch_day - 1 + tle[i].epoch_frac) * 86400;

    get_true_anomaly(i, true);
}

void Orbits::build(int i) {

    lines.clear();
    lines_colors.clear();

    for (int i = 0; i < 360; i++) {
        float i_rad = glm::radians((float) i);
        float ip1_rad = glm::radians((float) i + 1);

        // unit circle is counter-clockwise, so we invert the sin to have clockwise orbits
        lines.push_back(glm::vec3(semi_major_axis * cos(i_rad), 0, semi_minor_axis * -sin(i_rad)) * base + offset);
        lines.push_back(glm::vec3(semi_major_axis * cos(ip1_rad), 0, semi_minor_axis * -sin(ip1_rad)) * base + offset);

        lines_colors.push_back(glm::vec4(i_rad, 0.0f, 0.0f, 0.0f));
        lines_colors.push_back(glm::vec4(ip1_rad, 0.0f, 0.0f, 0.0f));
    }
}

void Orbits::draw() {
    glUseProgram(scene->orbits_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &(scene->mvp)[0][0]);

    for (int i = 0; i < tle.size(); i++) {
        glUniformMatrix3fv(base_id, 1, GL_FALSE, &base[0][0]);
        glUniform3fv(offset_id, 1, &offset[0]);

        double delta = (double) time(NULL) - epoch;
        double days_since_epoch = delta / SECS_DAY;

        real_time_mean_anomaly = tle[i].mean_anomaly + days_since_epoch * tle[i].revloutions_per_day * M_PI * 2;
        real_time_mean_anomaly = fmod(real_time_mean_anomaly, M_PI * 2);

        get_true_anomaly(i, false);

        int pos_index = (int) (true_anomaly / (M_PI * 2) * 360) * 2;
        float inter = (true_anomaly / (M_PI * 2) * 360) - floor(true_anomaly / (M_PI * 2) * 360);

        glm::vec3 prev = lines[pos_index];
        glm::vec3 next = lines[(pos_index + 2) % (360 * 2)];

        pos = inter * next + (1 - inter) * prev;

        glUniform1f(a_id, true_anomaly);
        draw_m();

        if (show_apsis) {
            periapsis.pos = lines[0];
            periapsis.draw();
            apoapsis.pos = lines[360];
            apoapsis.draw();
        }
    }
}

void Orbits::debug() {
    for (int i = 0; i < tle.size(); i++) {
        if (ImGui::CollapsingHeader(tle[i].name.data())) {
            ImGui::Text("Catalog number: %d", tle[i].cat_number);
            ImGui::Text("Catalog class: %c", tle[i].classification);
            ImGui::Text("Int. designator: ", tle[i].international_designator);
            ImGui::DragInt("Epoch year", &tle[i].epoch_year);
            ImGui::DragInt("Epoch day", &tle[i].epoch_day);
            ImGui::DragFloat("Epoch frac", &tle[i].epoch_frac);
            ImGui::Spacing();
            ImGui::SliderFloat("Inclination", &tle[i].inclination, 0, M_PI * 2);
            ImGui::SliderFloat("Ascending node", &tle[i].ascending_node_longitude, 0, M_PI * 2);
            ImGui::SliderFloat("Eccentricity", &tle[i].eccentricity, 0, 1);
            ImGui::SliderFloat("Argument of perigee", &tle[i].argument_of_perigee, 0, M_PI * 2);
            ImGui::SliderFloat("Mean anomaly", &tle[i].mean_anomaly, 0, M_PI * 2);
            ImGui::DragFloat("Revs. per day", &tle[i].revloutions_per_day);
            ImGui::DragInt("Revs at epoch", &tle[i].revolutions_at_epoch);
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
            compute_pitch_yaw(i);
            ImGui::Text("pitch: %f yaw: %f", angle[0], angle[1]);
            ImGui::Text("lat: %f lon: %f", angle[0] / M_PI * 180, angle[1] / M_PI * 180);
            ImGui::Text("Altitude: %f", (float) glm::length(pos) - 6371);
            build_orbit(i);
            build(i);
            manage_m_buffers();

            if (ImGui::Checkbox("Show apsis", &show_apsis)) {
                apoapsis.debug();
                periapsis.debug();
            }
        }
    }
}

void Orbits::get_true_anomaly(int i, bool compute) {
    if (compute) {
        compute_true_anomalies();
    }

    int index =  (int) glm::degrees(fmod(real_time_mean_anomaly, M_PI * 2));
    // std::cout << real_time_mean_anomaly << " " <<  index << std::endl;
    float delta = real_time_mean_anomaly - floor(real_time_mean_anomaly);

    true_anomaly = delta * true_anomaly_index[index] + (1 - delta) * true_anomaly_index[(index+1) % 360];
}

// big thanks https://github.com/duncaneddy/rastro/blob/main/rastro/src/orbits.rs
void Orbits::compute_true_anomalies() {
    int max_iter = 20;
    double e = 0.0001;

    for (int i = 0; i < 360; i++) {
        for (int j = 0; j < tle.size(); j++) {

            float tmp_mean_anomaly = glm::radians((float) i);

            double anm_ecc = tle[j].eccentricity < 0.8 ? tmp_mean_anomaly : M_PI;

            double f = anm_ecc - tle[j].eccentricity * sin(anm_ecc) - tmp_mean_anomaly;
            int ite = 0;

            while (abs(f) > e) {
                f = anm_ecc - tle[j].eccentricity * sin(anm_ecc) - tmp_mean_anomaly;
                anm_ecc = anm_ecc - f / (1.0f - tle[j].eccentricity * cos(anm_ecc));

                ite += 1;
                if (ite > max_iter) {
                    std::cout << "Failing to converge true anomaly calculation for " << tle[j].name << std::endl;
                    break;
                }
            }

            true_anomaly_index[i] = fmod(anm_ecc + M_PI * 2, M_PI * 2);
        }
    }
}

void Orbits::compute_pitch_yaw(int i) {
    float yaw = atan2(pos.z, pos.x);
    float pitch = atan2(pos.y, sqrt(pos.x * pos.x + pos.z * pos.z));

    angle[0] = pitch;
    angle[1] = yaw;
}