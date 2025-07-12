#pragma once

#include <string>
#include <iostream>

#include "sqlite3.h"
#include "fmt/core.h"
#include "cpr/cpr.h"

#include "tle.hpp"

class DBManager {
    private:
        sqlite3 *db;
        void init();
    public:
        DBManager();
        ~DBManager();

        void set_meta(std::string key, int val);
        int get_meta(std::string key);
        void ingest_tle_group(std::string group_name);
        void ingest_tle(TLE t);
        std::vector<TLE> get_all_tle();
};