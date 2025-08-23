#include "ecs/systems.hpp"

namespace systems::ring {

void build_rings_from_json(ECSTable &ecs, nlohmann::ordered_json json) {
    for (auto body = json.begin(); body != json.end(); body++) {
        std::string body_name = body.key().c_str();

        size_t parent_entity = systems::get_major_body_by_name(ecs, body_name);

        nlohmann::ordered_json &obj = body.value();
        for (auto itt = obj.begin(); itt != obj.end(); itt++) {
            nlohmann::ordered_json &ring = itt.value(); 

            Ring r;

            r.range.x = ring["body_distance"];
            r.range.y = r.range.x + (int) ring["width"];
            r.transmittance = 0;
            r.selected = 0;
            for (int i = 0; i < r.bits.size(); i++) r.bits.set(i, 1);

            if (!ring["transmittance"].is_null()) {
                r.transmittance = ring["transmittance"];
            }

            if (!ring["gaps"].is_null()) {
                for (auto ittt = ring["gaps"].begin(); ittt != ring["gaps"].end(); ittt++) {
                    nlohmann::ordered_json &gap = ittt.value();

                    int gap_inner = gap["body_distance"];
                    int gap_outer = gap_inner + (int) gap["width"];

                    for (int i = 0; i < r.bits.size(); i++) {
                        float ii = (float) i / r.bits.size();
                        bool up =   (ii * (int) ring["width"] > gap_inner - r.range.x);
                        bool down = (ii * (int) ring["width"] < gap_outer - r.range.x);

                        if (up && down) {
                            r.bits.set(i, 0);
                        }
                    }
                }
            }

            size_t e = ecs.request_entity();

            ecs.set_component(e, DISPLAY_NAME);
            ecs.set_component(e, PARENT);
            ecs.set_component(e, RING);

            ecs.set_DisplayName(e, ring["name"]);
            ecs.set_Parent(e, parent_entity);
            ecs.set_Ring(e, r);
        }
    }
}

void draw_rings(Scene *scene, ECSTable &ecs) {
    size_t first = ecs.get_first(DRAWABLE_RING);
    size_t last = ecs.get_last(DRAWABLE_RING);

    render::ring::draw(scene, ecs, first, last);
}
} // namespace systems::ring
