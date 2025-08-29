#include "scene/scene.hpp"
#include "ecs/systems.hpp"

using std::chrono::nanoseconds;
using std::chrono::high_resolution_clock;

Scene::Scene(SDL_Window *_window) { 

    camera = new Camera;
    camera->scene = this;
    window = _window;
    frame_time = time.get();

    build_solar_system();
    
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
    frame_time = time.get();
    // fmt::print("current time: {}\n", frame_time.time_since_epoch().count());

    while (db.signals.size()) {
        fmt::print("scene: {} signals\n", db.signals.size());
        Signal s = db.signals[db.signals.size() - 1]; 
        db.signals.pop_back();
        fmt::print("scene: {} signals\n", db.signals.size());
    }

    lines_drawn = 0;
    triangles_drawn = 0;
    triangles_t_drawn = 0;

    // TODO: find a better solution for that
    if (frames % 1000 == 0) {
        systems::orbit::compute_orbit_from_tle(this, ecs);
    }

    systems::sphere::compute_pos(this, ecs);
    systems::sphere::compute_rota(this, ecs);
    
    systems::orbit::compute_true_anomalies(this, ecs);
    systems::orbit::compute_pos_along_orbit(this, ecs);

    if (follow_entity != -1) {
        Position tmp = ecs.get_Position(follow_entity);
        camera->look_at(tmp);
    }

    projection = glm::perspective(glm::radians(60.0f), get_ratio(), 10.0f, 10000000000.0f);
    view = camera->get_view();
    model = glm::mat4(1.0);
    mvp = projection * view * model;

    auto t2 = high_resolution_clock::now();
    ttc = (t2 - t1).count();

    t1 = high_resolution_clock::now();
    render::skybox::draw(this);
    render::grid::draw(this);
    render::sphere::draw(this, ecs);
    render::orbit::draw(this, ecs);
    render::ring::draw(this, ecs);

    frames++;

    t2 = high_resolution_clock::now();
    ttr = (t2 - t1).count();
}

std::chrono::high_resolution_clock::time_point Scene::get_time() {
    return frame_time;
}

void Scene::build_solar_system() {
    std::vector<TLE> t = db.get_all_tle();

    for (int id : {10, 199, 299, 399, 301, 499, 599, 501, 502, 503, 504, 699, 606, 799, 899}) {
        size_t e = ecs.request_entity();

        ecs.set_component(e, POSITION);
        ecs.set_component(e, ROTATION);
        ecs.set_component(e, SCALE);
        ecs.set_component(e, MAJOR_BODY);
        ecs.set_component(e, EPHEMERIS);
        ecs.set_component(e, ROTATION_INFO);

        ecs.set_MajorBody(e, db.get_major_body(id));
    }

    systems::sphere::init(this, ecs);

    for (int i = 0; i < t.size(); i++) {
        size_t e = ecs.request_entity();

        ecs.set_component(e, PARENT);
        ecs.set_component(e, POSITION);
        ecs.set_component(e, ROTATION);
        ecs.set_component(e, TWO_LINE_ELEMENT);
        ecs.set_component(e, ORBIT);
        ecs.set_component(e, EPOCH);
        ecs.set_component(e, TRUE_ANOMALY_INDEX);

        ecs.set_TLE(e, t[i]);
        ecs.set_Parent(e, 3);
    }

    systems::orbit::compute_orbit_from_tle(this, ecs);
    systems::orbit::index_true_anomalies(this, ecs);
    systems::orbit::compute_true_anomalies(this, ecs);
    systems::orbit::compute_pos_along_orbit(this, ecs);

    nlohmann::ordered_json ring_info = nlohmann::ordered_json::parse(std::ifstream("assets/data/rings.json"));
    systems::ring::build_rings_from_json(ecs, ring_info);
}

void Scene::debug() {
    auto t1 = high_resolution_clock::now();
    int id = 0;
    ImGui::Begin("Scene debug");
    ImGui::SeparatorText("SCENE");
    ImGui::Text("Time to compute: %fms", ttc / 1e6);
    ImGui::Text("Time to render: %fms", ttr / 1e6);
    ImGui::Text("Time to debug: %fms", ttd / 1e6);
    ImGui::Text("Frames: %ld.", frames);
    ImGui::Text("Ratio: %.3f.", get_ratio());
    ImGui::Text("Lines drawn %ld.", lines_drawn);
    ImGui::Text("Triangles drawn %ld.", triangles_drawn);
    ImGui::Text("Texture triangles drawn %ld.", triangles_t_drawn);
    ImGui::SeparatorText("CAMERA");
    camera->debug();
    ImGui::End();
    time.debug();
    db.debug();
    systems::debug_entities(this, ecs); 
    auto t2 = high_resolution_clock::now();
    ttd = (t2 - t1).count();
}