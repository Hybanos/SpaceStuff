#include "objects/orbits.hpp"
#include "scene/scene.hpp"

using std::chrono::nanoseconds;
using std::chrono::high_resolution_clock;

Orbits::Orbits(Scene *s, std::vector<TLE>& t) : 
Object(s),
mesh(scene->orbits_shader) { 
    tle = t;

    build();
    manage_buffers();
}

void Orbits::build() {
    lines.clear();
    lines_colors.clear();

    for (int j = 0; j < 360; j++) {
        float i_rad = glm::radians((float) j);

        // unit circle is counter-clockwise, so we invert the sin to have clockwise orbits
        lines.push_back(glm::vec3(cos(i_rad), 0, -sin(i_rad)));

        lines_colors.push_back(glm::vec4(i_rad, 0.0f, 0.0f, 0.0f));
    }
    lines.push_back(lines[0]);
    lines_colors.push_back(glm::vec4(M_PI*2, 0.0f, 0.0f, 0.0f));

    int size = tle.size();
    base.resize(size);
    semi_major_axis.resize(size);
    semi_minor_axis.resize(size);
    epoch.resize(size);
    true_anomaly.resize(size);
    true_anomaly_index.resize(size);
    offset.resize(size);
    positions.resize(size);
    angle.resize(size);
    flag.resize(size);

    for (int i = 0; i < tle.size(); i++) {
        build_orbit(i);
        flag[i] = 1;
    }
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

    base[i] = m1 * m2 * m3;
    // re-swap Y and Z
    std::swap(base[i][2], base[i][1]);

    // thanks https://space.stackexchange.com/questions/18289/how-to-get-semi-major-axis-from-tle
    semi_major_axis[i] = glm::pow(3.986004418*1e14, 1.0f / 3) / glm::pow(2.0f * tle[i].revloutions_per_day * M_PI / 86400.0f, 2.0f / 3.0f) / 1000.0f;
    semi_minor_axis[i] = semi_major_axis[i] * sqrt(1.0f - tle[i].eccentricity * tle[i].eccentricity);

    float linear_eccentricity = sqrt(semi_major_axis[i] * semi_major_axis[i] - semi_minor_axis[i] * semi_minor_axis[i]);
    offset[i] = glm::vec3(-linear_eccentricity, 0, 0) * base[i];

    // jan 1 of epoch year (seconds per year isn't uniform)
    std::tm tm = {};
    tm.tm_year = tle[i].epoch_year - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = 1;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    epoch[i] = ((double) std::mktime(&tm)) + (tle[i].epoch_day - 1 + tle[i].epoch_frac) * 86400;

    get_true_anomaly(i, true);
    compute_along_orbit(i);
}

void Orbits::compute_along_orbit(int i) {

    // get the current anomalies 
    get_true_anomaly(i, false);

    // get point in orbit lines
    int pos_index = (int) (true_anomaly[i] / (M_PI * 2) * 360);
    float inter = (true_anomaly[i] / (M_PI * 2) * 360) - floor(true_anomaly[i] / (M_PI * 2) * 360);

    glm::vec3 prev = lines[pos_index] * glm::vec3(semi_major_axis[i], 0, semi_minor_axis[i]) * base[i] + offset[i];
    glm::vec3 next = lines[(pos_index + 1) % (360)] * glm::vec3(semi_major_axis[i], 0, semi_minor_axis[i]) * base[i] + offset[i];
    
    // compute actual 3D position
    positions[i] = inter * next + (1 - inter) * prev;

    // compute pitch and yaw from earth center
    float yaw = atan2(positions[i].z, positions[i].x);
    float pitch = atan2(positions[i].y, sqrt(positions[i].x * positions[i].x + positions[i].z * positions[i].z));

    angle[i][0] = pitch;
    angle[i][1] = yaw;
}

void Orbits::get_true_anomaly(int i, bool compute) {
    if (compute) {
        compute_true_anomalies(i);
    }

    double t = scene->get_time().time_since_epoch().count() / 1e9;
    double days_since_epoch = (t - epoch[i]) / SECS_DAY;

    float real_time_mean_anomaly = tle[i].mean_anomaly + days_since_epoch * tle[i].revloutions_per_day * M_PI * 2;
    real_time_mean_anomaly = fmod(real_time_mean_anomaly, M_PI * 2);

    int index =  (int) glm::degrees(real_time_mean_anomaly);
    float delta = floor(glm::degrees(real_time_mean_anomaly)) - glm::degrees(real_time_mean_anomaly);

    float ta_i = true_anomaly_index[i][index];
    float ta_ip1 = index != 359 ? true_anomaly_index[i][(index+1) % 360] : M_PI * 2;

    true_anomaly[i] = delta * ta_i + (1 - delta) * ta_ip1;
    true_anomaly[i] = fmod(true_anomaly[i], M_PI * 2);
}

// big thanks https://github.com/duncaneddy/rastro/blob/main/rastro/src/orbits.rs
void Orbits::compute_true_anomalies(int j) {
    int max_iter = 20;
    double e = 0.0001;

    for (int i = 0; i < 360; i++) {

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

        true_anomaly_index[j][i] = fmod(anm_ecc + M_PI * 2, M_PI * 2);
    }
}

void Orbits::draw() {
    if (!d_draw) return;
    auto t1 = high_resolution_clock::now();

    // TODO: opti buffers
    for (int i = 0; i < tle.size(); i++) {
        if (flag[i] > 0) get_true_anomaly(i, false);
    }
    manage_buffers();

    mesh.set_mat4("MVP", scene->mvp);
    mesh.set_vec3("pos", pos - scene->camera->get_center());
    mesh.draw_instanced(GL_LINE_STRIP, 0, lines.size(), tle.size());
    scene->lines_drawn += lines.size() * tle.size();

    auto t2 = high_resolution_clock::now();
    ttr = (t2 - t1).count();
}

void Orbits::manage_buffers() {
    size_t size = base.size();
    mesh.set_buffer(0, lines.data(), lines.size());
    mesh.set_buffer(1, lines_colors.data(), lines_colors.size());
    mesh.set_buffer(2, base.data(), size, 1);
    mesh.set_buffer(5, offset.data(), size, 1);
    mesh.set_buffer(6, semi_major_axis.data(), size, 1);
    mesh.set_buffer(7, semi_minor_axis.data(), size, 1);
    mesh.set_buffer(8, true_anomaly.data(), size, 1);
    mesh.set_buffer(9, flag.data(), size, 1);
}

void Orbits::debug() {
    if (ImGui::TreeNode("Orbits")) {
        ImGui::Checkbox("Draw", &d_draw);
        ImGui::Text("Time to render: %fms", ttr / 1e6);
        ImGui::Text("Total orbits: %lld", tle.size());
        
        if (ImGui::InputTextWithHint("##Filter", "filter", filter.InputBuf, IM_ARRAYSIZE(filter.InputBuf))) {
            filter.Build();
        }
        
        for (int i = 0; i < tle.size(); i++) {
            
            if (filter.PassFilter(tle[i].name.data())) {
                flag[i] = 1;
            } else {
                flag[i] = 0;
            }

            if (! filter.PassFilter(tle[i].name.data())) continue;

            if (ImGui::TreeNode(fmt::format("{}##{}", tle[i].name, i).c_str())) {
                flag[i] = 2;
                build_orbit(i);
                manage_buffers();

                if (ImGui::Button("Follow")) {
                    following = i;
                    scene->camera->set_anchor(this);
                }

                ImGui::Text("Catalog number: %d", tle[i].cat_number);
                ImGui::Text("Catalog class: %c", tle[i].classification);
                ImGui::Text("Int. designator: %s", tle[i].international_designator);
                ImGui::DragInt("Epoch year", &tle[i].epoch_year);
                ImGui::DragInt("Epoch day", &tle[i].epoch_day);
                ImGui::DragFloat("Epoch frac", &tle[i].epoch_frac);
                ImGui::Spacing();
                ImGui::SliderFloat("Inclination", &tle[i].inclination, 0, M_PI * 2);
                ImGui::SliderFloat("Ascending node", &tle[i].ascending_node_longitude, 0, M_PI * 2);
                ImGui::SliderFloat("Eccentricity", &tle[i].eccentricity, 0, 1);
                ImGui::SliderFloat("Argument of perigee", &tle[i].argument_of_perigee, 0, M_PI * 2);
                ImGui::SliderFloat("Mean anomaly (at epoch)", &tle[i].mean_anomaly, 0, M_PI * 2);
                ImGui::DragFloat("Revs. per day", &tle[i].revloutions_per_day);
                ImGui::DragInt("Revs at epoch", &tle[i].revolutions_at_epoch);
                ImGui::Spacing();
                ImGui::Text("Semi major axis: %f", semi_major_axis[i]);
                ImGui::Text("Semi minor axis: %f", semi_minor_axis[i]);
                ImGui::Text("True anomaly: %f", true_anomaly[i]);
                ImGui::Text("True to mean anomalies:");
                ImGui::PlotLines("haha", true_anomaly_index[i].begin(), 360, 0, NULL, 0, M_PI * 2, ImVec2(0, 80));
                ImGui::Spacing();
                ImGui::Text("\tx\ty\tz");
                ImGui::Text("\t%f\t%f\t%f", base[i][0][0], base[i][1][0], base[i][2][0]);
                ImGui::Text("\t%f\t%f\t%f", base[i][0][1], base[i][1][1], base[i][2][1]);
                ImGui::Text("\t%f\t%f\t%f", base[i][0][2], base[i][1][2], base[i][2][2]);
                ImGui::Spacing();
                ImGui::Text("x: %f, y:%f, z:%f", positions[i].x, positions[i].y, positions[i].z);
                ImGui::Text("pitch: %f yaw: %f", angle[i][0], angle[i][1]);
                ImGui::Text("lat: %f lon: %f", angle[i][0] / M_PI * 180, angle[i][1] / M_PI * 180);
                ImGui::Text("Altitude: %f", (float) glm::length(positions[i]) - 6371);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

glm::vec3 Orbits::get_camera_center() {
    compute_along_orbit(following);
    return positions[following] + pos;
}

void Orbits::on_signal(Signal s) {
    fmt::print("orbits on signal\n");
    if (s != Signal::SAT_DATA_UPDATE) return;
    fmt::print("orbits on signal\n");

    tle = scene->db.get_all_tle();

    build();
    manage_buffers();
}