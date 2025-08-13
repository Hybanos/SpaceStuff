#include "data/position.hpp"

MajorBody_simu::MajorBody_simu(int _id, DBManager &_db) : id{_id}, db{_db} {
    MajorBody body = db.get_major_body(id);
    name = body.name;

    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c){return std::tolower(c);}); 
    rotation_info = json::parse(std::ifstream("assets/data/bodies.json"))[lower_name]; 

    compute_transforms(2025);
}

void MajorBody_simu::compute_transforms(int year) {
    std::vector<EphemerisLine> lines = db.get_ephemeris_year(id, year); 
    jan_1_ts = lines[0].timestamp;

    pos.resize(lines.size());

    for (int i = 0; i < lines.size(); i++) {
        EphemerisLine l = lines[i];

        // transforms[i] = glm::mat4(1);
        // transforms[i] = glm::translate(transforms[i], glm::vec3(l.x, l.y, l.z));
        pos[i].x = l.x;
        pos[i].y = l.y;
        pos[i].z = l.z;
    } 
}

glm::vec3 MajorBody_simu::get_pos(long timestamp) {
    long diff = timestamp - jan_1_ts;  
    int days = diff / (24l * 3600 * 1'000'000'000);
    long s_until_nex_day = (timestamp / 1'000'000'000) % (24l * 3600);
    float day_percent = (float) s_until_nex_day / (24l * 3600); 

    // fmt::print("diff: {}, days: {}, day percent: {}\n", diff, days, day_percent);

    glm::vec3 curr = pos[days % pos.size()] * (1 - day_percent);
    glm::vec3 next = pos[(days+1) % pos.size()] * (day_percent);

    return (curr + next);
}

static double get_angle(json o, double julian_date, double f(double)) {
    if (!o.size()) return 0.0f;
    double angle;
    for (auto &j : o) {
        if (j.size() == 1) angle += (double) j * julian_date; 
        else if (j.size() == 2) angle += (double) j[0] * (julian_date * julian_date); // NOTE: ugly
        else angle += (double) j[0] * f((double) j[1] + (double) j[2] * julian_date);
    }
    return angle;
}

glm::mat3 MajorBody_simu::get_rota(double julian_day) {
    julian_day = julian_day - 2451545.0;
    double julian_year = julian_day / 36525;

    if (rotation_info.size() == 0) return glm::mat3(1);

    double a0 = 90.0f + (double) rotation_info["a0"];
    double d0 = 90.0f - (double) rotation_info["d0"];
    double W = rotation_info["W"];

    a0 += get_angle(rotation_info["a0_d"], julian_day, glm::sin);
    a0 += get_angle(rotation_info["a0_T"], julian_year, glm::sin);
    d0 += get_angle(rotation_info["d0_d"], julian_day, glm::cos);
    d0 += get_angle(rotation_info["d0_T"], julian_year, glm::cos);
    W += get_angle(rotation_info["W_d"], julian_day, glm::sin);
    W += get_angle(rotation_info["W_T"], julian_year, glm::sin);

    a0 = glm::radians(fmod(a0, 360));
    d0 = glm::radians(fmod(d0, 360));
    W = glm::radians(fmod(W, 360));
    a0 += W;

    glm::mat3 y(
        glm::cos(a0), 0, -glm::sin(a0),
        0, 1, 0,
        glm::sin(a0), 0, glm::cos(a0)
    );

    glm::mat3 z(
        glm::cos(d0), glm::sin(d0), 0,
        -glm::sin(d0), glm::cos(d0), 0,
        0, 0, 1
    );

    return z * y;
}