#include "objects/barycenter.hpp"
#include "scene/scene.hpp"

Barycenter::Barycenter(Scene *s, int _id) : Object(s), id{_id} {

}

void Barycenter::draw() {
    Particle p(scene, pos, glm::vec4(1));
    p.draw();
}

void Barycenter::debug() {

}