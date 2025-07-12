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

        query = "CREATE TABLE Meta (key TEXT NOT NULL PRIMARY KEY, val INTEGER DEFAULT 0);";
        sqlite3_exec(db, query.data(), NULL, NULL, NULL);

        query = fmt::format("INSERT INTO Meta (key, val) VALUES"
            "('CREATE_TIME', {}),"
            "('DATA_PULL_TIME', -1)"
        ";", time(NULL));
        sqlite3_exec(db, query.data(), NULL, NULL, NULL);
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
    sqlite3_exec(db, query.data(), NULL, NULL, NULL);

    // orbit_classification
    query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Class';";
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    if (sqlite3_step(statement) != SQLITE_ROW) {
        
        query = "CREATE TABLE Class ("
            "classID INTEGER PRIMARY KEY,"
            "class TEXT"
        ");";
        sqlite3_exec(db, query.data(), NULL, NULL, NULL);

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
        sqlite3_exec(db, query.data(), NULL, NULL, NULL);
    }
    sqlite3_finalize(statement);

    ingest_tle_group("haha");
}

void DBManager::set_meta(std::string key, int val) {
    std::string query = fmt::format("INSERT OR REPLACE into Meta (key, val) VALUES ('{}', {});", key, val);
    sqlite3_exec(db, query.data(), NULL, NULL, NULL);
}

int DBManager::get_meta(std::string key) {
    int ret;
    std::string query = fmt::format("SELECT val FROM Meta WHERE key='{}';", key);
    sqlite3_stmt *statement;
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    if (sqlite3_step(statement) == SQLITE_ROW) {
        ret = sqlite3_column_int(statement, 0);
    } else {
        fmt::print("No key {} in table Meta\n", key);
    }
    sqlite3_finalize(statement);
    return ret;
}

// TODO: make good checks
void DBManager::ingest_tle_group(std::string group_name) {
    if (get_meta("DATA_PULL_TIME") > time(NULL) - 3600 * 24) {
        fmt::print("TLE data too recent, using cache...\n");
        return;
    }
    cpr::Response r = cpr::Get(cpr::Url{"http://141.145.210.183:30050/files/f?f=haha.tle.back"});
    set_meta("DATA_PULL_TIME", time(NULL));

    std::ofstream file;
    std::string filename = fmt::format("/tmp/{}.tle", group_name);
    file.open(filename);
    file << r.text;
    file.close();

    std::vector<TLE> tles = read_tle_file(filename);
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    for (auto t : tles) {
        ingest_tle(t);
    }
    sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
}

void DBManager::ingest_tle(TLE t) {
    std::string query = fmt::format("INSERT OR REPLACE INTO SatData ("
        "name,"
        "cat_number,"
        "classification,"
        "international_designator,"
        "epoch_year,"
        "epoch_day,"
        "epoch_frac,"

        "inclination,"
        "ascending_node_longitude,"
        "eccentricity,"
        "argument_of_perigee,"
        "mean_anomaly,"
        "revolutions_per_day,"
        "revolutions_at_epoch"
        ") VALUES ('{}', {}, '{}', '{}', {}, {}, {}, {}, {}, {}, {}, {}, {}, {})"
    ";",
        t.name,
        t.cat_number,
        t.classification,
        t.international_designator,
        t.epoch_year,
        t.epoch_day,
        t.epoch_frac,

        t.inclination,
        t.ascending_node_longitude,
        t.eccentricity,
        t.argument_of_perigee,
        t.mean_anomaly,
        t.revloutions_per_day,
        t.revloutions_per_day
    );
    sqlite3_exec(db, query.data(), NULL, NULL, NULL);
}

std::vector<TLE> DBManager::get_all_tle() {
    std::vector<TLE> TLEs;

    std::string query = "SELECT * FROM SatData;";
    sqlite3_stmt *statement;
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    while(sqlite3_step(statement) == SQLITE_ROW) {
        TLE t;
        std::stringstream buf;
        t.name = reinterpret_cast<const char *>(sqlite3_column_text(statement, 0));

        t.cat_number = sqlite3_column_int(statement, 1);
        t.classification = sqlite3_column_text(statement, 2)[0];
        t.international_designator = reinterpret_cast<const char *>(sqlite3_column_text(statement, 3));
        t.epoch_year = sqlite3_column_int(statement, 4);
        t.epoch_day = sqlite3_column_int(statement, 5);
        t.epoch_frac = sqlite3_column_double(statement, 6);

        t.inclination = sqlite3_column_double(statement, 7);
        t.ascending_node_longitude = sqlite3_column_double(statement, 8);
        t.eccentricity = sqlite3_column_double(statement, 9);
        t.argument_of_perigee = sqlite3_column_double(statement, 10);
        t.mean_anomaly = sqlite3_column_double(statement, 11);
        t.revloutions_per_day = sqlite3_column_double(statement, 12);
        t.revolutions_at_epoch = sqlite3_column_double(statement, 13);

        TLEs.push_back(t);
    }
    sqlite3_finalize(statement);
    return TLEs;
}