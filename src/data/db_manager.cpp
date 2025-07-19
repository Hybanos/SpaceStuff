#include "data/db_manager.hpp"

DBManager::DBManager() {
    sqlite3_open("data.db", &db);
    init();
    update_debug_vectors();
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

    // groups 
    query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Groups';";
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    if (sqlite3_step(statement) != SQLITE_ROW) {
        
        query = R"(CREATE TABLE Groups (group_id INTEGER PRIMARY KEY, group_name TEXT, pull_time INTEGER DEFAULT 0);)";
        sqlite3_exec(db, query.data(), NULL, NULL, NULL);

            //  ('active'),
            //  ('gpz'),
            //  ('gpz-plus'),
        query = R"(INSERT INTO Groups (group_name) VALUES
             ('last-30-days'),
             ('stations'),
             ('visual'),
             ('analyst'),
             ('cosmos-1408-debris'),
             ('fengyun-1c-debris'),
             ('iridium-33-debris'),
             ('cosmos-2251-debris'),

             ('weather'),
             ('noaa'),
             ('goes'),
             ('resource'),
             ('sarsat'),
             ('dmc'),
             ('tdrss'),
             ('argos'),
             ('planet'),
             ('spire'),

             ('geo'),
             ('intelsat'),
             ('ses'),
             ('eutelsat'),
             ('telesat'),
             ('starlink'),
             ('oneweb'),
             ('qianfan'),
             ('hulianwang'),
             ('kuiper'),
             ('iridium-NEXT'),
             ('orbcomm'),
             ('globalstar'),
             ('amateur'),
             ('satnogs'),
             ('x-comm'),
             ('other-comm'),

             ('gnss'),
             ('gps-ops'),
             ('glo-ops'),
             ('galileo'),
             ('beidou'),
             ('sbas'),
             ('nnss'),
             ('musson'),

             ('science'),
             ('geodetic'),
             ('engineering'),
             ('education'),

             ('military'),
             ('radar'),
             ('cubesat'),
             ('other')
        ;)";
        sqlite3_exec(db, query.data(), NULL, NULL, NULL);
    }
    sqlite3_finalize(statement);

    // TODO: change group_name to group_id for joins
    query = R"(CREATE TABLE IF NOT EXISTS SatGroup (
        cat_number INTEGER,
        group_name TEXT,
        PRIMARY KEY (cat_number, group_name),
        FOREIGN KEY(cat_number) REFERENCES SatData(cat_number),
        FOREIGN KEY(group_name) REFERENCES Groups(group_name)
    );)";
    sqlite3_exec(db, query.data(), NULL, NULL, NULL);

    query = R"(CREATE TABLE IF NOT EXISTS MajorBodies (
        major_body_id INTEGER PRIMARY KEY,
        name TEXT,
        designation TEXT,
        alias TEXT,
        mass REAL,
        heliocentric_gravitational_constant REAL,
        radius REAL
    );)";
    sqlite3_exec(db, query.data(), NULL, NULL, NULL);
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

void DBManager::set_group_pull_time(std::string group, int val) {
    std::string query = fmt::format("UPDATE Groups SET pull_time={} WHERE group_name='{}';", val, group);
    std::cout << sqlite3_exec(db, query.data(), NULL, NULL, NULL) << std::endl;
}

int DBManager::get_group_pull_time(std::string group) {
    int ret;
    std::string query = fmt::format("SELECT pull_time FROM Groups WHERE group_name='{}';", group);
    sqlite3_stmt *statement;
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    if (sqlite3_step(statement) == SQLITE_ROW) {
        ret = sqlite3_column_int(statement, 0);
    } else {
        fmt::print("No group {} in table Groups\n", group);
    }
    sqlite3_finalize(statement);
    return ret;
}

// TODO: make good checks
void DBManager::ingest_tle_group(std::string group_name) {
    if (get_group_pull_time(group_name) > time(NULL) - 3600 * 24) {
        return;
    }

    cpr::Response r = cpr::Get(cpr::Url{fmt::format("https://celestrak.org/NORAD/elements/gp.php?GROUP={}&FORMAT=tle", group_name)}, cpr::Timeout(30000));
    set_group_pull_time(group_name, time(NULL));

    std::cout << r.url << std::endl;
    if (r.status_code == 200) {

        std::ofstream file;
        std::string filename = fmt::format("/tmp/{}.tle", group_name);
        file.open(filename);
        file << r.text;
        file.close();

        std::vector<TLE> tles = read_tle_file(filename);
        sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
        for (auto t : tles) {
            ingest_tle(t, group_name);
        }
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);

        signals.push_back(Signal::SAT_DATA_UPDATE);
        fmt::print("db manager: {} signals\n", signals.size());
    } else {
        fmt::print("Got status code {} from celestrak, using cached data", r.status_code);
    }
}

void DBManager::ingest_tle(TLE t, std::string group) {
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

    query = fmt::format(R"(INSERT OR REPLACE INTO SatGroup (
        cat_number,
        group_name
    ) VALUES (
        {},
        '{}'
    );)", t.cat_number, group);
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

void DBManager::ingest_major_bodies() {
    cpr::Response r = cpr::Get(cpr::Url("https://ssd.jpl.nasa.gov/api/horizons.api?format=text&COMMAND=MB"), cpr::Timeout(30000));

    if (r.status_code == 200) {
        std::vector<MajorBody> v = parse_major_bodies(r.text);

        sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

        std::string query;
        for (auto &b : v) {
            query = fmt::format(R"(INSERT OR REPLACE INTO MajorBodies (
                major_body_id,
                name,
                designation,
                alias,
                mass,
                heliocentric_gravitational_constant,
                radius
            ) VALUES ({}, '{}', '{}', '{}', {}, {}, {});)", 
                b.major_body_id,
                b.name,
                b.designation,
                b.alias,
                -1.0f,
                -1.0f,
                -1.0f
            );

            sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);
        }

        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);

    } else {
        fmt::print("Got status code {} from Horizons System.\n", r.status_code);
    }
}

void DBManager::update_debug_vectors() {
    group_names.clear();
    group_pull_times.clear();
    std::string query = "SELECT group_name, pull_time FROM Groups;";
    sqlite3_stmt *statement;
    sqlite3_prepare_v3(db, query.c_str(), query.size(), 0, &statement, NULL);
    while (sqlite3_step(statement) == SQLITE_ROW) {
        std::string s = reinterpret_cast<const char *>(sqlite3_column_text(statement, 0));
        group_names.push_back(s);
        group_pull_times.push_back(sqlite3_column_int(statement, 1));
    }
    sqlite3_finalize(statement);
}

void DBManager::debug() {
    ImGui::Begin("Data");
    
    for (int i = 0; i < group_names.size(); i++) {
        std::string s = group_names[i];
        int t = group_pull_times[i];
        ImGui::SeparatorText(s.c_str());
        if (t == 0) {
            ImGui::Text("Last pulled: ---");
        } else {
            int d = time(NULL) - t;
            ImGui::Text(fmt::format("Last pulled: {}s ago", d).c_str());
        }
        if (ImGui::Button(fmt::format("pull##{}", i).c_str())) {
            ingest_tle_group(s);
            update_debug_vectors();
        }
    }

    ImGui::End();
}