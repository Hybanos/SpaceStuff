#include "object_mesh.hpp"
#include "object_faces.hpp"
#include "object_cubemap.hpp"

class Sphere : public ObjectMesh, public ObjectFaces, public ObjectCubeMap {
    private:
        GLuint cubemap_id;
        bool rebuild = false;
        int resolution = 15;
        float size = 1;
        float normalise_amount = 1.0f;

        void manageBuffers();
        virtual void manage_texture();
    public:
        Sphere(int);
        void draw(glm::mat4);
        void draw_m();
        void draw_f();
        void draw_t(glm::mat4);
        void build();
        void debug();
};