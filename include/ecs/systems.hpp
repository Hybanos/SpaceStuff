#pragma once

#include "imgui.h"

#include "ecs/ecs.hpp"
#include "scene/scene.hpp"
#include "render/objects/orbit.hpp"
#include "render/objects/sphere.hpp"
#include "render/objects/skybox.hpp"
#include "render/objects/grid.hpp"

namespace systems::sphere {
    void init(Scene *scene, ECSTable &ecs);
    void compute_pos(Scene *scene, ECSTable &ecs);
    void compute_rota(Scene *scene, ECSTable &ecs);
    void draw_spheres(Scene *scene, ECSTable &ecs);
} // namespace systems::sphere

namespace systems::orbit {
    // orbits
    void compute_orbit_from_tle(ECSTable &ecs);
    void index_true_anomalies(ECSTable &ecs);
    void compute_true_anomalies(ECSTable &ecs, double t);
    void compute_pos_along_orbit(ECSTable &ecs);
    void draw_orbits(Scene *scene, ECSTable &ecs);
    void filter_orbits(Scene *scene, ECSTable &ecs);
} // namespace systems::orbit

namespace systems {
    void debug_entity(Scene *scene, ECSTable &ecs, size_t entity_id);
    void debug_entities(Scene *scene, ECSTable &ecs);
} // namespace systems
