#include "objects/orbit_line.hpp"
#include "scene/scene.hpp"

OrbitLine::OrbitLine(Scene *s, TLE t) : Object(s), 
    periapsis(scene, glm::vec3(0, 0, 0), glm::vec4(0.6, 0.6, 1, 1)),
    apoapsis(scene, glm::vec3(0, 0, 0), glm::vec4(0.6, 1, 0.6, 1)) {
    tle = t;
    matrix_id = glGetUniformLocation(scene->orbit_program_id, "MVP");
    // base_id = glGetUniformLocation(scene->orbit_program_id, "base");
    // offset_id = glGetUniformLocation(scene->orbit_program_id, "offset");
    // a_id = glGetUniformLocation(scene->orbit_program_id, "a");
    b_id = glGetUniformLocation(scene->orbit_program_id, "b");

    draw_mesh = true;
    build_orbit();
    build();
    // manage_m_buffers();

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (1 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void *) (2 * sizeof(glm::vec3)));

    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void * ) 0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, (void * ) 0);

    glBindVertexArray(0);
}

void OrbitLine::build_orbit() {
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

    base = m1 * m2 * m3;
    // re-swap Y and Z
    std::swap(base[2], base[1]);

    // thanks https://space.stackexchange.com/questions/18289/how-to-get-semi-major-axis-from-tle
    semi_major_axis = glm::pow(3.986004418*1e14, 1.0f / 3) / glm::pow(2.0f * tle.revloutions_per_day * M_PI / 86400.0f, 2.0f / 3.0f) / 1000.0f;
    semi_minor_axis = semi_major_axis * sqrt(1.0f - tle.eccentricity * tle.eccentricity);

    linear_eccentricity = sqrt(semi_major_axis * semi_major_axis - semi_minor_axis * semi_minor_axis);
    offset = glm::vec3(-linear_eccentricity, 0, 0) * base;

    // jan 1 of epoch year (seconds per year isn't uniform)
    std::tm tm = {};
    tm.tm_year = tle.epoch_year - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = 1;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    epoch = ((double) std::mktime(&tm)) + (tle.epoch_day - 1 + tle.epoch_frac) * 86400;

    get_true_anomaly(true);
}

void OrbitLine::build() {

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

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, lines_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * 3 * sizeof(float), (float *) lines.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, lines_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, lines_colors.size() * 4 * sizeof(float), (float *) lines_colors.data(), GL_STATIC_DRAW);


    glBindVertexArray(0);

}

void OrbitLine::draw() {
    glUseProgram(scene->orbit_program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &(scene->mvp)[0][0]);
    // glUniformMatrix3fv(base_id, 1, GL_FALSE, &base[0][0]);
    // glUniform3fv(offset_id, 1, &offset[0]);

    double delta = (double) time(NULL) - epoch;
    double days_since_epoch = delta / SECS_DAY;

    real_time_mean_anomaly = tle.mean_anomaly + days_since_epoch * tle.revloutions_per_day * M_PI * 2;
    real_time_mean_anomaly = fmod(real_time_mean_anomaly, M_PI * 2);

    get_true_anomaly(false);

    int pos_index = (int) (true_anomaly / (M_PI * 2) * 360) * 2;
    float inter = (true_anomaly / (M_PI * 2) * 360) - floor(true_anomaly / (M_PI * 2) * 360);

    glm::vec3 prev = lines[pos_index];
    glm::vec3 next = lines[(pos_index + 2) % (360 * 2)];

    pos = inter * next + (1 - inter) * prev;

    // glUniform1f(a_id, true_anomaly);
    // draw_m();

    int i = ++scene->i;
    scene->bases[i] = base;
    scene->offsets[i] = offset;
    scene->true_anomalies[i] = true_anomaly;
    
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_LINES, 0, lines.size(), scene->bases.size());
    glBindVertexArray(0);

    if (show_apsis) {
        periapsis.pos = lines[0];
        periapsis.draw();
        apoapsis.pos = lines[360];
        apoapsis.draw();
    }
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
        ImGui::Text("lat: %f lon: %f", angle[0] / M_PI * 180, angle[1] / M_PI * 180);
        ImGui::Text("Altitude: %f", (float) glm::length(pos) - 6371);
        build_orbit();
        build();
        // manage_m_buffers();

        if (ImGui::Checkbox("Show apsis", &show_apsis)) {
            apoapsis.debug();
            periapsis.debug();
        }
    }
}

void OrbitLine::get_true_anomaly(bool compute) {
    if (compute) {
        compute_true_anomalies();
    }

    int index =  (int) glm::degrees(fmod(real_time_mean_anomaly, M_PI * 2));
    // std::cout << real_time_mean_anomaly << " " <<  index << std::endl;
    float delta = real_time_mean_anomaly - floor(real_time_mean_anomaly);

    true_anomaly = delta * true_anomaly_index[index] + (1 - delta) * true_anomaly_index[(index+1) % 360];
}

// big thanks https://github.com/duncaneddy/rastro/blob/main/rastro/src/orbits.rs
void OrbitLine::compute_true_anomalies() {
    int max_iter = 20;
    double e = 0.0001;
    
    for (int i = 0; i < 360; i++) {

        float tmp_mean_anomaly = glm::radians((float) i);

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

        true_anomaly_index[i] = fmod(anm_ecc + M_PI * 2, M_PI * 2);
    }
}

void OrbitLine::compute_pitch_yaw() {
    float yaw = atan2(pos.z, pos.x);
    float pitch = atan2(pos.y, sqrt(pos.x * pos.x + pos.z * pos.z));

    angle[0] = pitch;
    angle[1] = yaw;
}