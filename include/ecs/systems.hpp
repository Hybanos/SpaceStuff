#pragma once

#include "imgui.h"

#include "ecs/ecs.hpp"
#include "scene/scene.hpp"
#include "render/objects/orbit.hpp"
#include "render/objects/sphere.hpp"
#include "render/objects/skybox.hpp"
#include "render/objects/grid.hpp"
#include "render/objects/ring.hpp"

namespace systems::sphere {
    void init(Scene *scene, ECSTable &ecs);
    void compute_pos(Scene *scene, ECSTable &ecs);
    void compute_rota(Scene *scene, ECSTable &ecs);
    void draw_spheres(Scene *scene, ECSTable &ecs);
} // namespace systems::sphere

namespace systems::orbit {
    void compute_orbit_from_tle(ECSTable &ecs);
    void index_true_anomalies(ECSTable &ecs);
    void compute_true_anomalies(ECSTable &ecs, double t);
    void compute_pos_along_orbit(ECSTable &ecs);
    void filter_orbits(Scene *scene, ECSTable &ecs);
    void draw_orbits(Scene *scene, ECSTable &ecs);
} // namespace systems::orbit

namespace systems::ring {
    void build_rings_from_json(ECSTable &ecs, nlohmann::ordered_json json);
    void draw_rings(Scene *scene, ECSTable &ecs);
} // namespace systems::ring

namespace systems {
    size_t get_major_body_by_name(ECSTable &ecs, std::string name);

    void debug_entity(Scene *scene, ECSTable &ecs, size_t entity_id);
    void debug_entities(Scene *scene, ECSTable &ecs);
} // namespace systems
