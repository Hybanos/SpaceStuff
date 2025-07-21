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
#include "data/horizons.hpp"
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
        // sat
        void set_group_pull_time(std::string group, int val);
        int get_group_pull_time(std::string group);
        void ingest_tle_group(std::string group_name);
        void ingest_tle(TLE t, std::string group);
        std::vector<TLE> get_all_tle();

        // ssd
        // returning with body ids of -1 means none foundk
        void ingest_major_bodies();
        void ingest_major_body(MajorBody body);
        void ingest_ephemeris(EphemerisLine line);
        std::vector<MajorBody> get_all_major_bodies();
        MajorBody get_major_body(int id);
        EphemerisLine get_ephemeris_line(int id);
        void download_ephemeris_line(int id);

        void debug();
};