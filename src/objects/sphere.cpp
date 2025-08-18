#include "objects/sphere.hpp"
#include "scene/scene.hpp"

Sphere::Sphere(Scene *s, std::string path) : 
Object(s),
body(10, s->db),
mesh(scene->texture_shader) {
    mesh.gen_cubemap(path);
    build();
    manage_buffers();

    display_name = "Sphere";
}

Sphere::Sphere(Scene *s, int _id) : 
Object(s),
body(_id, s->db),
mesh(scene->texture_shader) {
    id = _id;

    EphemerisLine line = scene->db.get_ephemeris_line(id);
    MajorBody b = scene->db.get_major_body(id);
    fmt::print("got body {}\n", b.name);
    size = b.radius;
    display_name = b.name;
    mesh.gen_cubemap("assets/textures/cubemaps/" + b.name);

    std::cout << line.x << " " << line.y << " " << line.z << std::endl;

    pos = glm::vec3(line.x, line.y, line.z);

    build();
    manage_buffers();
}

void Sphere::build() {
    triangles.clear();
    triangles_colors.clear();

    std::vector<glm::vec3> v;
    int n = resolution;

    // create first face points
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            glm::vec3 point((float)i / (n-1) * 2 - 1, (float)j / (n-1) * 2 - 1, 1);
            v.push_back(point);
        }
    }

    // normalise point distance
    for (auto &point : v) {
        point = (glm::normalize(point) * normalise_amount + point * (1 - normalise_amount));
        point = point * size;
    }

    // first face triangles / mesh
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            triangles.push_back(v[i * n + j]);
            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j]);

            triangles.push_back(v[i * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j + 1]);
            triangles.push_back(v[(i + 1) * n + j]);

            triangles_colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            triangles_colors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    int size_triangles = triangles.size();
    glm::quat q;

    // copy and rotate face 5 times
    for (glm::quat q : {
        glm::quat(glm::vec3(0, M_PI_2, 0)),
        glm::quat(glm::vec3(0, M_PI, 0)),
        glm::quat(glm::vec3(0, -M_PI_2, 0)),
        glm::quat(glm::vec3(M_PI_2, 0, 0)),
        glm::quat(glm::vec3(-M_PI_2, 0, 0))
    }) {
        for (int i = 0; i < size_triangles; i++) {
            triangles.push_back(triangles[i] * q);
            triangles_colors.push_back(triangles_colors[i]);
        }
    }
}

void Sphere::draw() {
    if (!d_draw) return;
    double julian_date = scene->time.get_julian(); 
    double angle = (0.7790572732640 + 1.00273781191135448 * (julian_date - 2451545.0)) * (M_PI * 2);
    angle = fmod(angle, M_PI * 2);

    rota = glm::mat3(
        glm::vec3(cos(angle), 0, -sin(angle)),
        glm::vec3(0, 1, 0),
        glm::vec3(sin(angle), 0, cos(angle))
    );
    pos = body.get_pos(scene->get_time().time_since_epoch().count());
    rota = body.get_rota(scene->time.get_julian());

    if (rebuild) {
        build();
        manage_buffers();
    }

    glm::mat4 mvp = scene->mvp;

    mesh.set_mat4("MVP", mvp);
    mesh.set_mat3("rota", rota);
    mesh.set_int("flip", 0);
    mesh.set_vec3("pos", pos - scene->camera->get_center());

    mesh.draw_cubemap(GL_TRIANGLES, 0, triangles.size() * 3);
    for (auto c : children) {
        c->set_pos(pos);
        c->set_rota(rota);
        c->draw();
    }
}

void Sphere::manage_buffers() {
    mesh.set_location(0, triangles.data(), triangles.size());
}

void Sphere::debug() {
    if (ImGui::TreeNode(display_name.c_str())) {
        if (ImGui::Button("Follow")) scene->camera->set_anchor(this);
        ImGui::Checkbox("Draw", &d_draw);
        ImGui::Checkbox("Build each frame", &rebuild);
        ImGui::SliderInt("Resolution:", &resolution, 2, 100);
        ImGui::SliderFloat("Radius:", &size, 0, 100);
        ImGui::SliderFloat("Normalisation amound:", &normalise_amount, 0, 1);
        ImGui::Text("Rotation matrix:");
        ImGui::Text("\t%f\t%f\t%f", rota[0][0], rota[1][0], rota[2][0]);
        ImGui::Text("\t%f\t%f\t%f", rota[0][1], rota[1][1], rota[2][1]);
        ImGui::Text("\t%f\t%f\t%f", rota[0][2], rota[1][2], rota[2][2]);
        ImGui::Text("Rotation angle (deg): %f", acos(rota[0][0]) * (180 / M_PI)); 
        for (auto c : children) c->debug();
        ImGui::TreePop();
    }
}

glm::vec3 Sphere::get_camera_center() {
    return pos;
}