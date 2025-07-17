#include "objects/sphere.hpp"
#include "scene/scene.hpp"

Sphere::Sphere(Scene *s, std::string path, Object *p) : 
Object(s, p),
mesh(scene->texture_shader) {
    mesh.gen_cubemap(path);
    build();
    manage_buffers();

}

void Sphere::build() {
    triangles.clear();
    triangles_colors.clear();

    lines.clear();
    lines_colors.clear();

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

            lines.push_back(v[i * n + j]);
            lines.push_back(v[i * n + j + 1]);

            lines.push_back(v[(i + 1) * n + j]);
            lines.push_back(v[(i + 1) * n + j + 1]);

            lines.push_back(v[i * n + j]);
            lines.push_back(v[(i + 1) * n + j]);

            lines.push_back(v[i * n + j + 1]);
            lines.push_back(v[(i + 1) * n + j + 1]);

            lines.push_back(v[i * n + j + 1]);
            lines.push_back(v[(i + 1) * n + j]);

            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            lines_colors.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        }
    }

    int size_triangles = triangles.size();
    int size_lines = lines.size();
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

        for (int i = 0; i < size_lines; i++) {
            lines.push_back(lines[i] * q);
            lines_colors.push_back(lines_colors[i]);
        }
    }
}

void Sphere::draw() {
    if (!d_draw) return;
    double julian_date = ((double) scene->time.get().time_since_epoch().count() / 1e9) / SECS_DAY + 2440587.5;
    double angle = (0.7790572732640 + 1.00273781191135448 * (julian_date - 2451545.0)) * (M_PI * 2);
    angle = fmod(angle, M_PI * 2);

    rota = glm::mat3(
        glm::vec3(cos(angle), 0, -sin(angle)),
        glm::vec3(0, 1, 0),
        glm::vec3(sin(angle), 0, cos(angle))
    );

    if (rebuild) {
        build();
        manage_buffers();
    }

    glm::mat4 mvp = scene->mvp;

    mesh.set_mat4("MVP", mvp);
    mesh.set_mat3("rota", rota);
    mesh.set_int("flip", 0);

    mesh.draw_cubemap(GL_TRIANGLES, 0, triangles.size() * 3);
}

void Sphere::manage_buffers() {
    mesh.set_buffer(0, triangles);
}

void Sphere::debug() {
    if (ImGui::CollapsingHeader("Sphere")) {
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
    }
}

glm::vec3 Sphere::get_pos() {
    glm::vec3 offset = glm::vec3(0, scene->frames * 3, 0);
    if (parent != nullptr) {
        return parent->get_pos() + offset;
    }
    return offset;
}