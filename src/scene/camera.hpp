#ifndef SPACESTUFF_CAMERA_HPP
#define SPACESTUFF_CAMERA_HPP

#include "scene/scene.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <SDL3/SDL.h>

#include <iostream>

class Scene;

class Camera {
    private:
        double yaw = M_PI_4;
        double pitch = M_PI_4;
        double distance = 3;

        glm::vec3 position;
        glm::vec3 center;

    public:
        bool enable_move = false;
        

        Scene *scene = nullptr;

        void translate(glm::vec3);
        void go_to(glm::vec3);
        void look_at(glm::vec3);

        void handle_mouse_move(SDL_Event&);
        void handle_mouse_scroll(SDL_Event&);
        void update_pos();

        void debug();        
 
        glm::mat4 get_view();
        glm::vec3& get_position() {return position;}
        glm::vec3& get_center() {return center;}
        
};

#endif