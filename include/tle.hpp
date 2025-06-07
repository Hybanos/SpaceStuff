#ifndef SPACESTUFF_TLE_HPP
#define SPACESTUFF_TLE_HPP

#include "glm/glm.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <vector>

struct TLE {
    std::string name;

    // line 1
    int cat_number;
    char classification;
    std::string international_designator;
    int epoch_year;
    int epoch_day;
    float epoch_frac;

    // line 2
    float inclination;
    float ascending_node_longitude;
    float eccentricity;
    float argument_of_perigee;
    float mean_anomaly;
    float revloutions_per_day;
    int revolutions_at_epoch;
};

inline TLE read_tle(std::string &title, std::string &l_1, std::string &l_2) {
    TLE t;
    t.name = title;

    t.cat_number = stoi(l_1.substr(2, 5));
    t.classification = l_1[7];
    t.international_designator = l_1.substr(9, 7);
    t.epoch_year = stoi(l_1.substr(18, 2));
    t.epoch_year += t.epoch_year < 50 ? 2000 : 1900;
    t.epoch_day = stoi(l_1.substr(20, 3));
    t.epoch_frac = stof(l_1.substr(24, 8)) / 100000000;
    
    t.inclination = glm::radians(stof(l_2.substr(8, 8)));
    // TODO: why offset from pi*2
    t.ascending_node_longitude = (M_PI * 2) - glm::radians(stof(l_2.substr(17, 9)));
    t.eccentricity = stof(l_2.substr(26, 7)) / 10000000;
    t.argument_of_perigee = (M_PI * 2) - glm::radians(stof(l_2.substr(34, 8)));
    t.mean_anomaly = glm::radians(stof(l_2.substr(43, 8)));
    t.revloutions_per_day = stof(l_2.substr(52, 11));
    t.revolutions_at_epoch = stoi(l_2.substr(63, 5));

    return t;
}

inline std::vector<TLE> read_tle_file(std::string path) {
    std::ifstream file(path);
    std::string title, l_1, l_2;

    std::vector<TLE> v;

    while (getline(file, title)) {
        getline(file, l_1);
        getline(file, l_2);

        TLE t = read_tle(title, l_1, l_2);
        v.push_back(t);
    }

    file.close();
    return v;
}

#endif