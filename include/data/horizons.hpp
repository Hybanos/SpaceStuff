#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct MajorBody {
    int major_body_id;
    std::string name;
    std::string designation;
    std::string alias;
    float mass;
    float heliocentric_gravitaional_constant;
    float radius;
};

inline std::vector<MajorBody> parse_major_bodies(std::string t) {
    std::istringstream text(t);
    std::vector<MajorBody> v;

    std::string l;
    while (std::getline(text, l)) {
        MajorBody b;
        try {
            b.major_body_id = std::stoi(l.substr(0, 9));
        } catch (std::exception) {
            continue;
        }
        b.name = l.substr(11, 35);
        b.designation = l.substr(46, 9);
        b.alias = l.substr(59);
        v.push_back(b);
        // std::cout << b.major_body_id << " - " << b.name << " - " << b.designation << " - " << b.alias << std::endl;
    }

    return v;
}