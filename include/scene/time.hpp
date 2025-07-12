#pragma once

#include <iostream>
#include <chrono>

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"
#include "fmt/core.h"
#include "fmt/chrono.h"

using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::time_point;

class Time {
    private:
        time_point<high_resolution_clock> last_sample;
        time_point<high_resolution_clock> timestamp;
        int rate = 1;
        bool paused = false;


        int combo_index = 0;
    public:
        Time();
        time_point<high_resolution_clock> get();
        void debug();
};