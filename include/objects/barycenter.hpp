#pragma once

#include "objects/object.hpp"
#include "objects/particle.hpp"

class Barycenter : virtual public Object {
    private:
        int id;
    public:
        Barycenter(Scene *s, int _id);

        void draw();
        void debug();
};