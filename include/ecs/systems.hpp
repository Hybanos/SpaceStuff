#pragma once

#include "imgui.h"

#include "ecs/ecs.hpp"
#include "scene/scene.hpp"
#include "render/objects/orbits.hpp"

namespace systems::sphere {

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