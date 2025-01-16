#include "object_mesh.hpp"
#include "object_faces.hpp"
#include "object.hpp"

class Sphere : public ObjectMesh, public ObjectFaces {
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
        void build();
        void debug();
};