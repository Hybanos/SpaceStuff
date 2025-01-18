#include "object_mesh.hpp"
#include "object_faces.hpp"
#include "object_texture.hpp"

class Sphere : public ObjectMesh, public ObjectFaces, public ObjectTexture {
    private:
        GLuint cubemap_id;
        bool rebuild = false;
        int resolution = 2;
        float size = 1;
        float normalise_amount = 0.0f;

        void manageBuffers();
    public:
        Sphere(int);
        void draw(glm::mat4);
        void draw_m();
        void draw_f();
        void draw_t(glm::mat4);
        void build();
        void debug();
};