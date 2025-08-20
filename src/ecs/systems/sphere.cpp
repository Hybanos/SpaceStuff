#include "ecs/systems.hpp"

static double get_angle(nlohmann::json o, double julian_date, double f(double)) {
    if (!o.size()) return 0.0f;
    double angle;
    for (auto &j : o) {
        if (j.size() == 1) angle += (double) j * julian_date; 
        else if (j.size() == 2) angle += (double) j[0] * (julian_date * julian_date); // NOTE: ugly
        else angle += (double) j[0] * f((double) j[1] + (double) j[2] * julian_date);
    }
    return angle;
}

namespace systems::sphere {

void init(Scene *scene, ECSTable &ecs) {
    for (size_t i = 0; i < ecs.size; i++) {
        if (ecs.bits[i] != DRAWABLE_SPHERE) continue;

        Position &position = ((Position *) ecs.component_table[POSITION])[i];
        Rotation &rotation = ((Rotation *) ecs.component_table[ROTATION])[i];
        Scale &scale = ((Scale *) ecs.component_table[SCALE])[i];
        MajorBody &mb = ((MajorBody *) ecs.component_table[MAJOR_BODY])[i];
        Ephemeris &ephemeris = ((Ephemeris *) ecs.component_table[EPHEMERIS])[i];
        RotationInfo &rotation_info = ((RotationInfo *) ecs.component_table[ROTATION_INFO])[i];

        std::string name(mb.name);
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){return std::tolower(c);}); 
        rotation_info = json::parse(std::ifstream("assets/data/bodies.json"))[name]; 

        std::vector<EphemerisLine> tmp = scene->db.get_ephemeris_year(mb.major_body_id, 2025);
        memcpy(&ephemeris, &tmp[0], tmp.size() * sizeof(EphemerisLine));

        scale = mb.radius;
    }
}

void compute_pos(Scene *scene, ECSTable &ecs) {
    for (size_t i = 0; i < ecs.size; i++) {
        if (ecs.bits[i] != DRAWABLE_SPHERE) continue;

        Position &position = ((Position *) ecs.component_table[POSITION])[i];
        Rotation &rotation = ((Rotation *) ecs.component_table[ROTATION])[i];
        Scale &scale = ((Scale *) ecs.component_table[SCALE])[i];
        MajorBody &mb = ((MajorBody *) ecs.component_table[MAJOR_BODY])[i];
        Ephemeris &ephemeris = ((Ephemeris *) ecs.component_table[EPHEMERIS])[i];
        RotationInfo &rotation_info = ((RotationInfo *) ecs.component_table[ROTATION_INFO])[i];

        long timestamp = scene->get_time().time_since_epoch().count(); 
        long diff = timestamp - ephemeris[0].timestamp;
        int days = diff / (24l * 3600 * 1'000'000'000);
        long s_until_next_day = (timestamp / 1'000'000'000) % (24l * 3600);
        float day_percent = (float) s_until_next_day / (24l * 3600);

        EphemerisLine curr = ephemeris[days % ephemeris.size()];
        EphemerisLine next = ephemeris[(days+1) % ephemeris.size()];

        position.x = curr.x * (1 - day_percent) + next.x * (day_percent);
        position.y = curr.y * (1 - day_percent) + next.y * (day_percent);
        position.z = curr.z * (1 - day_percent) + next.z * (day_percent);
    }
}

void compute_rota(Scene *scene, ECSTable &ecs) {
    for (size_t i = 0; i < ecs.size; i++) {
        if (ecs.bits[i] != DRAWABLE_SPHERE) continue;

        Position &position = ((Position *) ecs.component_table[POSITION])[i];
        Rotation &rotation = ((Rotation *) ecs.component_table[ROTATION])[i];
        Scale &scale = ((Scale *) ecs.component_table[SCALE])[i];
        MajorBody &mb = ((MajorBody *) ecs.component_table[MAJOR_BODY])[i];
        Ephemeris &ephemeris = ((Ephemeris *) ecs.component_table[EPHEMERIS])[i];
        RotationInfo &rotation_info = ((RotationInfo *) ecs.component_table[ROTATION_INFO])[i];

        double julian_day = scene->time.get_julian() - 2451545.0;
        double julian_year = julian_day / 36525;

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

        rotation = z * y;
    }
}

void draw_spheres(Scene *scene, ECSTable &ecs) {
    size_t first = ecs.get_first(DRAWABLE_SPHERE);
    size_t last = ecs.get_last(DRAWABLE_SPHERE);
    render::sphere::draw(scene, ecs, first, last);
}

} // namespace systems::sphere