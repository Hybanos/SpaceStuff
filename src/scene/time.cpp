#include "scene/time.hpp"

Time::Time() {
    last_sample = high_resolution_clock::now();
    timestamp = last_sample;
}

time_point<high_resolution_clock> Time::get() {
    auto now = high_resolution_clock::now();
    duration delta = now - last_sample;
    last_sample = now; 
    if (!paused) timestamp = timestamp + delta * rate;
    return timestamp;
}

void Time::debug() {
    ImGui::Begin("Time");
    ImGui::Text(fmt::format("Curr time: {}", get()).c_str());
    ImGui::DragInt("Rate", &rate);
    if (ImGui::Button("Now")) {
        auto now = high_resolution_clock::now();
        timestamp = now;
        last_sample = now;
    }
    if (paused) {
        if (ImGui::Button("Resume")) paused = false;
    } else {
        if (ImGui::Button("Pause")) paused = true;
    }
    ImGui::End();
}