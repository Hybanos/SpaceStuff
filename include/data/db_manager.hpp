#pragma once

#include <string>
#include <iostream>

#include "sqlite3.h"
#include "fmt/core.h"

class DBManager {
    private:
        sqlite3 *db;
        void init();
    public:
        DBManager();
        ~DBManager();
};