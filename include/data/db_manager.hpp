#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include "sqlite3.h"
#include "fmt/core.h"
#include "fmt/chrono.h"
#include "cpr/cpr.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

#include "data/tle.hpp"
#include "scene/signal.hpp"

class DBManager {
    private:
        sqlite3 *db;
        std::vector<std::string> group_names;
        std::vector<int> group_pull_times;
        void init();
        void update_debug_vectors();
    public:
        std::vector<Signal> signals;

        DBManager();
        ~DBManager();

        void set_meta(std::string key, int val);
        int get_meta(std::string key);
        void set_group_pull_time(std::string group, int val);
        int get_group_pull_time(std::string group);
        void ingest_tle_group(std::string group_name);
        void ingest_tle(TLE t, std::string group);
        std::vector<TLE> get_all_tle();
        void debug();
};