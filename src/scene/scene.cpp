#include "scene/scene.hpp"
#include "shaders/shader.hpp"

Scene::Scene(SDL_Window *_window) {
    window = _window;
    camera = new Camera;
    camera->scene = this;

    program_id = LoadShaders("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");
    matrix_id = glGetUniformLocation(program_id, "MVP");

    // objects.push_back(new Cube(1));
    // objects.push_back(new Cube(1, -0.5, -0.5, -0.5));
    objects.push_back(new Sphere(1));

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
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), get_ratio(), 1.0f, 100.0f);
    glm::mat4 view = camera->get_view();
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 mvp = projection * view * model;

    glUseProgram(program_id);
    glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

    for (Object * obj : objects) {
        obj->draw();
    }

    // camera->translate(glm::vec3(sin((float) frames / 500) * 3, 3, cos((float) frames / 500) * 3));

    frames++;
}

void Scene::debug() {
    ImGui::Begin("Scene debug");
    ImGui::SeparatorText("SCENE");
    ImGui::Text("Frames: %d.", frames);
    ImGui::Text("Ratio: %.3f.", get_ratio());
    ImGui::SeparatorText("CAMERA");
    camera->debug();
    for (auto &o : objects) {
        o->debug();
    }
    ImGui::End();
}