#include "data/db_manager.hpp"

DBManager::DBManager() {
    sqlite3_open("data.db", &db);
    init();
}

DBManager::~DBManager() {
    sqlite3_close_v2(db);
}

void DBManager::init() {
    std::string query;
    sqlite3_stmt *statement;

    // check for meta table
    query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Meta';";
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    if (sqlite3_step(statement) != SQLITE_ROW) {
        sqlite3_finalize(statement);

        query = "CREATE TABLE Meta (metaID INTEGER PRIMARY KEY, key TEXT NOT NULL, val INTEGER DEFAULT 0);";
        sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
        sqlite3_step(statement);
        sqlite3_finalize(statement);

        query = fmt::format("INSERT INTO Meta (key, val) VALUES"
            "('CREATE_TIME', {}),"
            "('DATA_PULL_TIME', -1)"
        ";", time(NULL));
        sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
        sqlite3_step(statement);
    }
    sqlite3_finalize(statement);

    // satelite catalog
    query = "CREATE TABLE IF NOT EXISTS SatData ("
        "name TEXT,"
        "cat_number INTEGER PRIMARY KEY,"
        "classification TEXT,"
        "international_designator TEXT," 
        "epoch_year INTEGER,"
        "epoch_day INTEGER,"
        "epoch_frac REAL,"

        "inclination REAL,"
        "ascending_node_longitude REAL,"
        "eccentricity REAL,"
        "argument_of_perigee REAL,"
        "mean_anomaly REAL,"
        "revolutions_per_day REAL,"
        "revolutions_at_epoch REAL"
    ");";
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    sqlite3_step(statement);
    sqlite3_finalize(statement);

    // orbit_classification
    query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Class';";
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    if (sqlite3_step(statement) != SQLITE_ROW) {
        sqlite3_finalize(statement);
        query = "CREATE TABLE Class ("
            "classID INTEGER PRIMARY KEY,"
            "class TEXT"
        ");";
        sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
        sqlite3_step(statement);
        sqlite3_finalize(statement);

        query = "INSERT INTO Class (class) VALUES"
            "('leo'),"
            "('gps'),"
            "('geo'),"
            "('sso'),"
            "('circular'),"
            "('elliptical'),"
            "('low_incl'),"
            "('high_incl')"
        ";";
        sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
        sqlite3_step(statement);
    }
    sqlite3_finalize(statement);
}