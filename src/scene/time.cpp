#include "scene/time.hpp"

using namespace std::chrono_literals;

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

double Time::get_julian() {
    double julian_date = ((double) get().time_since_epoch().count() / 1e9) / (24 * 3600) + 2440587.5;

    return julian_date;
}

void Time::debug() {
    ImGui::Begin("Time");
    ImGui::Text(fmt::format("Curr time: {}", get()).c_str());
    ImGui::Text(fmt::format("Julian days: {}", get_julian()).c_str());
    ImGui::DragInt("Rate", &rate);
    if (ImGui::Button("Now")) {
        auto now = high_resolution_clock::now();
        timestamp = now;
        last_sample = now;
    }
    ImGui::SameLine();
    if (paused) {
        if (ImGui::Button("Resume")) paused = false;
    } else {
        if (ImGui::Button("Pause")) paused = true;
    }

    const char *items[5] = {"second", "minute", "hour", "day", "year"};
    seconds durations[5] = {1s, 1min, 1h, 24h, 8736h};

    if (ImGui::Button("-100")) timestamp = timestamp - 100 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("-50")) timestamp = timestamp - 50 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("-25")) timestamp = timestamp - 25 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("-10")) timestamp = timestamp - 10 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("-5")) timestamp = timestamp - 5 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("-2")) timestamp = timestamp - 2 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("-1")) timestamp = timestamp - 1 * durations[combo_index]; ImGui::SameLine(); 
    ImGui::PushItemWidth(120);
    ImGui::Combo("##", &combo_index, items, 5); ImGui::SameLine();
    ImGui::PopItemWidth();
    if (ImGui::Button("+1")) timestamp = timestamp + 1 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("+2")) timestamp = timestamp + 2 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("+5")) timestamp = timestamp + 5 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("+10")) timestamp = timestamp + 10 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("+25")) timestamp = timestamp + 25 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("+50")) timestamp = timestamp + 50 * durations[combo_index]; ImGui::SameLine(); 
    if (ImGui::Button("+100")) timestamp = timestamp + 100 * durations[combo_index]; ImGui::SameLine(); 

    ImGui::End();
}