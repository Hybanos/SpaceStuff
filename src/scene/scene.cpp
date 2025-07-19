#include "scene/scene.hpp"

using std::chrono::nanoseconds;
using std::chrono::high_resolution_clock;

Scene::Scene(SDL_Window *_window) { 

    camera = new Camera;
    camera->scene = this;
    window = _window;

    objects.push_back(new SkyBox(this));
    objects.push_back(new Grid(this));
    objects.push_back(new Sphere(this, "assets/cubemaps/earth"));

    std::vector<TLE> t = db.get_all_tle();
    objects.push_back(new Orbits(this, t, objects[2]));
    
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
    auto t1 = high_resolution_clock::now();

    while (db.signals.size()) {
        fmt::print("scene: {} signals\n", db.signals.size());
        Signal s = db.signals[db.signals.size() - 1]; 
        db.signals.pop_back();
        fmt::print("scene: {} signals\n", db.signals.size());
        for (Object *obj : objects) obj->on_signal(s);
    }

    projection = glm::perspective(glm::radians(80.0f), get_ratio(), 0.1f, 100000000.0f);
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

    auto t2 = high_resolution_clock::now();
    ttr = (t2 - t1).count();
}

void Scene::debug() {
    int id = 0;
    ImGui::Begin("Scene debug");
    if (ImGui::Button("HAHAHAHAHA")) db.ingest_major_bodies();
    ImGui::SeparatorText("SCENE");
    ImGui::Text("Time to render: %fms", ttr / 1e6);
    ImGui::Text("Frames: %ld.", frames);
    ImGui::Text("Ratio: %.3f.", get_ratio());
    ImGui::Text("Lines drawn %ld.", lines_drawn);
    ImGui::Text("Triangles drawn %ld.", triangles_drawn);
    ImGui::Text("Texture triangles drawn %ld.", triangles_t_drawn);
    ImGui::SeparatorText("CAMERA");
    camera->debug();
    ImGui::SeparatorText("OBJECTS");
    for (auto &o : objects) {
        ImGui::PushID(id++);
        o->debug();
        ImGui::PopID();
    }
    ImGui::End();
    time.debug();
    db.debug();
}