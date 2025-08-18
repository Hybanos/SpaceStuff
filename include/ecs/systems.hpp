#pragma once

#include "imgui.h"

#include "ecs/ecs.hpp"
#include "scene/scene.hpp"
#include "render/objects/orbits.hpp"

namespace systems {
    void compute_orbit_from_tle(ECSTable &ecs);
    void index_true_anomalies(ECSTable &ecs);
    void compute_true_anomalies(ECSTable &ecs, double t);
    void compute_pos_along_orbit(ECSTable &ecs);
    void draw_orbits(Scene *scene, ECSTable &ecs);

    void debug_entities(ECSTable &ecs);
} // namespace systems