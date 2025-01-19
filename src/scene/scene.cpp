#include "scene/scene.hpp"

Scene::Scene(SDL_Window *_window) {
    window = _window;
    camera = new Camera;
    camera->scene = this;

    program_id = LoadShaders("src/shaders/base.vs", "src/shaders/base.fs");
    matrix_id = glGetUniformLocation(program_id, "MVP");

    objects.push_back(new SkyBox());
    objects.push_back(new Sphere(1));

    for (auto o : objects) o->scene = this;

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
    projection = glm::perspective(glm::radians(90.0f), get_ratio(), 0.1f, 100000.0f);
    view = camera->get_view();
    model = glm::mat4(1.0);
    glm::mat4 mvp = projection * view * model;


    for (Object * obj : objects) {
        glUseProgram(program_id);
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);
        obj->draw(mvp);
    }

    frames++;
}

void Scene::debug() {
    int id = 0;
    ImGui::Begin("Scene debug");
    ImGui::SeparatorText("SCENE");
    ImGui::Text("Frames: %d.", frames);
    ImGui::Text("Ratio: %.3f.", get_ratio());
    ImGui::SeparatorText("CAMERA");
    camera->debug();
    for (auto &o : objects) {
        ImGui::PushID(id++);
        o->debug();
        ImGui::PopID();
    }
    ImGui::End();
}