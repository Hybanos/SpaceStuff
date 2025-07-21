#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <regex>
#include <algorithm> 
#include <cctype>

struct MajorBody {
    int major_body_id;
    std::string name;
    std::string designation;
    std::string alias;
    double mass;
    double heliocentric_gravitaional_constant;
    double radius;
};

struct EphemerisLine {
    int id;
    long timestamp;
    double julian_day_number;
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
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
        b.name.erase(std::find_if(b.name.rbegin(), b.name.rend(), [](unsigned char ch) {return !std::isspace(ch);}).base(), b.name.end());
        b.designation = l.substr(46, 9);
        b.alias = l.substr(59);
        b.mass = -1;
        b.heliocentric_gravitaional_constant = -1;
        b.radius = -1;
        v.push_back(b);
        // std::cout << b.major_body_id << " - " << b.name << " - " << b.designation << " - " << b.alias << std::endl;
    }

    return v;
}

static std::regex mass_regex(R"(Mass x10\^(\d+) \(kg\)=\s*([\d\.]+)\D)");
static std::regex hgc_regex(R"(GM, km\^3\/s\^2\s*=\s*([\d.]*)\D)");
static std::regex radius_regex(R"(adius,?\s*\(?km\)?\s*=\s*([\d.]*)\D)");

// scientific notation regex
static std::string sn = R"(-?\d+\.\d+(?:E[\+-]\d+)?)";
static std::string date = R"(.... \d\d\d\d-\w\w\w-\d\d \d\d:\d\d:\d\d\.\d\d\d\d)";
static std::regex ephemeris_line_regex(fmt::format(R"(({}),\s*({}),\s*({}),\s*({}),\s*({}),\s*({}),\s*({}),\s*({}))",
    sn, date, sn, sn, sn, sn, sn, sn
));
inline void parse_ephemeris(std::string t, MajorBody &body, std::vector<EphemerisLine> &lines) {
    std::istringstream text(t);
    std::string l;
   
    std::smatch matches;
    while (std::getline(text, l)) {
        // std::cout << l << std::endl;
        if (std::regex_search(l, matches, mass_regex) && body.mass < 0) {
            body.mass = std::stod(matches[2]) * std::pow(10, std::stoi(matches[1]));
        }
        if (std::regex_search(l, matches, hgc_regex) && body.heliocentric_gravitaional_constant < 0) {
            body.heliocentric_gravitaional_constant = std::stod(matches[1]) * 1e9; 
        }
        if (std::regex_search(l, matches, radius_regex) && body.radius < 0) {
            body.radius = std::stod(matches[1]);
        }
        if (std::regex_search(l, matches, ephemeris_line_regex)) {
            EphemerisLine e;
            e.id = body.major_body_id;
            // std::cout << matches[1] << std::endl;
            // std::cout << matches[3] << " " << matches[4] << " " << matches[5] << " " << matches[6] << " " << matches[7] << " " << matches[8] << std::endl;
            e.julian_day_number = std::stod(matches[1]);
            std::tm tm = {};
            // A.D. 2025-Jul-19 00:00:00.0000
            // std::cout << matches[2] << std::endl;
            std::stringstream(matches[2]) >> std::get_time(&tm, "A.D. %Y-%b-%d %H:%M:%S.0000");
            e.timestamp = std::chrono::high_resolution_clock::from_time_t(std::mktime(&tm)).time_since_epoch().count();
            e.x = std::stod(matches[3]);
            e.y = std::stod(matches[4]);
            e.z = std::stod(matches[5]);
            e.vx = std::stod(matches[6]);
            e.vy = std::stod(matches[7]);
            e.vz = std::stod(matches[8]);
            lines.push_back(e);
        }
    }
    // std::cout << b.mass << std::endl;
    // std::cout << b.radius << std::endl;
    // std::cout << b.heliocentric_gravitaional_constant << std::endl;
}