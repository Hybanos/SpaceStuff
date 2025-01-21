#include "scene/scene.hpp"

std::string earth_files[6] = {
    "assets/cubemaps/earth/front.jpg",
    "assets/cubemaps/earth/back.jpg",
    "assets/cubemaps/earth/top.jpg",
    "assets/cubemaps/earth/bottom.jpg",
    "assets/cubemaps/earth/left.jpg",
    "assets/cubemaps/earth/right.jpg"
};

Scene::Scene(SDL_Window *_window) {
    camera = new Camera;
    camera->scene = this;
    window = _window;

    base_program_id = LoadShaders("src/shaders/base.vs", "src/shaders/base.fs");
    texture_program_id = LoadShaders("src/shaders/texture.vs", "src/shaders/texture.fs");

    objects.push_back(new SkyBox(this));
    objects.push_back(new Sphere(this, earth_files));

    std::vector<TLE> t = read_tle_file("haha.tle");

    for (auto tle : t) {
        objects.push_back(new OrbitLine(this, tle));
    }

    camera->look_at(glm::vec3(0, 0, 0));
    camera->update_pos();
}

int Scene::get_width() {
    int width;
    int height;

    SDL_GetWindowSize(window, &width, &height);

    return width;
}

int Scene::get_height() {
    int width;
    int height;

    SDL_GetWindowSize(window, &width, &height);

    return height;
}

float Scene::get_ratio() {
    return (float) get_width() / get_height();
}
 
void Scene::render() {
    projection = glm::perspective(glm::radians(90.0f), get_ratio(), 0.1f, 100000000.0f);
    view = camera->get_view();
    model = glm::mat4(1.0);
    mvp = projection * view * model;

    lines_drawn = 0;
    triangles_drawn = 0;
    triangles_t_drawn = 0;

    for (Object * obj : objects) {
        obj->draw();
    }
    frames++;
}

void Scene::debug() {
    int id = 0;
    ImGui::Begin("Scene debug");
    ImGui::SeparatorText("SCENE");
    ImGui::Text("Frames: %ld.", frames);
    ImGui::Text("Ratio: %.3f.", get_ratio());
    ImGui::Text("Lines drawn %ld.", lines_drawn);
    ImGui::Text("Triangles drawn %ld.", triangles_drawn);
    ImGui::Text("Texture triangles drawn %ld.", triangles_t_drawn);
    ImGui::SeparatorText("CAMERA");
    camera->debug();
    for (auto &o : objects) {
        ImGui::PushID(id++);
        o->debug();
        ImGui::PopID();
    }
    ImGui::End();
}