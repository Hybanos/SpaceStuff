#ifndef SPACESTUFF_PARTICLE_HPP
#define SPACESTUFF_PARTICLE_HPP

#include "object_faces.hpp"

class Particle : public ObjectFaces {
    private:
        // std::string file;

        GLuint matrix_id;
        // GLuint texture_id;

        // bool draw_texture = true;
    public:
        glm::vec3 pos;
        glm::vec4 color;

        Particle(Scene *s, glm::vec3 p, glm::vec4 c); 

        void build();
        void draw();
        void debug();
};

#endif