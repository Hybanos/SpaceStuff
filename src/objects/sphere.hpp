#include "object_mesh.hpp"
#include "object_faces.hpp"
#include "object_texture.hpp"

class Sphere : public ObjectMesh, public ObjectFaces, public ObjectTexture {
    private:
        bool rebuild = false;
        int resolution = 4;
        float size = 1;
        float normalise_amount = 1.0f;

        void manageBuffers();
    public:
        Sphere(int);
        void draw();
        void draw_m();
        void draw_f();
        void draw_t();
        void build();
        void debug();
};