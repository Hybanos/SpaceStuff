#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <regex>
#include <algorithm> 
#include <cctype>
#include <chrono>

struct MajorBody {
    int major_body_id;
    char name[40];
    char designation[40];
    char alias[40];
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
        MajorBody b = {0};
        try {
            b.major_body_id = std::stoi(l.substr(0, 9));
        } catch (std::exception) {
            continue;
        }
        std::string tmp;
        tmp = l.substr(11, 35);
        tmp.erase(std::find_if(tmp.rbegin(), tmp.rend(), [](unsigned char ch) {return !std::isspace(ch);}).base(), tmp.end());
        memcpy(&b.name, tmp.data(), tmp.size());
        tmp = l.substr(46, 9);
        memcpy(&b.designation, tmp.data(), tmp.size());
        tmp = l.substr(59);
        memcpy(&b.alias, tmp.data(), tmp.size());
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
            e.julian_day_number = std::stod(matches[1]);
            std::tm tm = {};
            std::stringstream(matches[2]) >> std::get_time(&tm, "A.D. %Y-%b-%d %H:%M:%S.0000");
            e.timestamp = std::chrono::high_resolution_clock::from_time_t(std::mktime(&tm)).time_since_epoch().count() + (3600l * 1'000'000'000);
            // NOTE: Y and Z axis are swapped
            e.x = std::stod(matches[3]);
            e.z = std::stod(matches[4]);
            e.y = std::stod(matches[5]);
            e.vx = std::stod(matches[6]);
            e.vz = std::stod(matches[7]);
            e.vy = std::stod(matches[8]);
            lines.push_back(e);
        }
    }
}

// I'm starting to really like this stuff
#define OE \
    X(EC) X(QR) X(IN) \
    X(OM) X(W) X(Tp) \
    X(N) X(MA) X(TA) \
    X(A) X(AD) X(PR)

#define X(NAME) R"([\s\n]*)" #NAME R"([\s\n]*=[\s\n]*({}))" 
static std::regex orbital_element_regex(fmt::format(R"(({})[\s\n]*TDB)" OE, date, sn, sn, sn, sn, sn, sn, sn, sn, sn, sn, sn, sn));
inline void parse_orbital_elements(std::string l, MajorBody &body, TLE &tle) {
    // TODO: also parse body info similar to parse_ephemeris()
    std::cout << fmt::format(R"(({})[\s\n]*)" OE, date, sn, sn, sn, sn, sn, sn, sn, sn, sn, sn, sn, sn) << std::endl;
    std::smatch matches;
    if (std::regex_search(l, matches, orbital_element_regex)) {
        tle.eccentricity = std::stod(matches[2]);
        tle.inclination = std::stod(matches[4]);
        tle.ascending_node_longitude = std::stod(matches[5]);
        tle.argument_of_perigee = std::stod(matches[6]);
        tle.mean_anomaly = std::stod(matches[8]);
        tle.revloutions_per_day = (24 * 3600) / std::stod(matches[13]);
    }
}
#undef X

#define ONO R"(*******************************************************************************
 Revised: April 12, 2021                 Earth                              399
 
 GEOPHYSICAL PROPERTIES (revised May 9, 2022):
  Vol. Mean Radius (km)    = 6371.01+-0.02   Mass x10^24 (kg)= 5.97219+-0.0006
  Equ. radius, km          = 6378.137        Mass layers:
  Polar axis, km           = 6356.752          Atmos         = 5.1   x 10^18 kg
  Flattening               = 1/298.257223563   oceans        = 1.4   x 10^21 kg
  Density, g/cm^3          = 5.51              crust         = 2.6   x 10^22 kg
  J2 (IERS 2010)           = 0.00108262545     mantle        = 4.043 x 10^24 kg
  g_p, m/s^2  (polar)      = 9.8321863685      outer core    = 1.835 x 10^24 kg
  g_e, m/s^2  (equatorial) = 9.7803267715      inner core    = 9.675 x 10^22 kg
  g_o, m/s^2               = 9.82022         Fluid core rad  = 3480 km
  GM, km^3/s^2             = 398600.435436   Inner core rad  = 1215 km
  GM 1-sigma, km^3/s^2     =      0.0014     Escape velocity = 11.186 km/s
  Rot. Rate (rad/s)        = 0.00007292115   Surface area:
  Mean sidereal day, hr    = 23.9344695944     land          = 1.48 x 10^8 km
  Mean solar day 2000.0, s = 86400.002         sea           = 3.62 x 10^8 km
  Mean solar day 1820.0, s = 86400.0         Love no., k2    = 0.299
  Moment of inertia        = 0.3308          Atm. pressure   = 1.0 bar
  Mean surface temp (Ts), K= 287.6           Volume, km^3    = 1.08321 x 10^12
  Mean effect. temp (Te), K= 255             Magnetic moment = 0.61 gauss Rp^3
  Geometric albedo         = 0.367           Vis. mag. V(1,0)= -3.86
  Solar Constant (W/m^2)   = 1367.6 (mean), 1414 (perihelion), 1322 (aphelion)
 HELIOCENTRIC ORBIT CHARACTERISTICS:
  Obliquity to orbit, deg  = 23.4392911  Sidereal orb period  = 1.0000174 y
  Orbital speed, km/s      = 29.79       Sidereal orb period  = 365.25636 d
  Mean daily motion, deg/d = 0.9856474   Hill's sphere radius = 234.9       
*******************************************************************************


*******************************************************************************
Ephemeris / WWW_USER Sat Aug 30 18:37:39 2025 Pasadena, USA      / Horizons
*******************************************************************************
Target body name: Earth (399)                     {source: DE441}
Center body name: Solar System Barycenter (0)     {source: DE441}
Center-site name: BODY CENTER
*******************************************************************************
Start time      : A.D. 2025-Jul-19 00:00:00.0000 TDB
Stop  time      : A.D. 2025-Jul-20 00:00:00.0000 TDB
Step-size       : 1440 minutes
*******************************************************************************
Center geodetic : 0.0, 0.0, 0.0                   {E-lon(deg),Lat(deg),Alt(km)}
Center cylindric: 0.0, 0.0, 0.0                   {E-lon(deg),Dxy(km),Dz(km)}
Center radii    : (undefined)
Keplerian GM    : 1.3288932302247902E+11 km^3/s^2
Output units    : KM-S, deg, Julian Day Number (Tp)
Calendar mode   : Mixed Julian/Gregorian
Output type     : GEOMETRIC osculating elements
Output format   : 10
Reference frame : Ecliptic of J2000.0
*******************************************************************************
JDTDB
   EC    QR   IN
   OM    W    Tp
   N     MA   TA
   A     AD   PR
*******************************************************************************
$$SOE
2460875.500000000 = A.D. 2025-Jul-19 00:00:00.0000 TDB 
 EC= 1.657799966839574E-02 QR= 1.479125496958443E+08 IN= 1.202557502970917E-02
 OM= 1.886054022131863E+02 W = 2.536264632436956E+02 Tp=  2461023.991216048598
 N = 1.132322198238105E-05 MA= 2.147271262487632E+02 TA= 2.136631227626399E+02
 A = 1.504059799821124E+08 AD= 1.528994102683806E+08 PR= 3.179307096161857E+07
2460876.500000000 = A.D. 2025-Jul-20 00:00:00.0000 TDB 
 EC= 1.664655557259596E-02 QR= 1.479013278323522E+08 IN= 1.185589588589258E-02
 OM= 1.913726415985342E+02 W = 2.505635380412194E+02 Tp=  2461023.674654979259
 N = 1.132332656011801E-05 MA= 2.160138228738351E+02 TA= 2.149108635296977E+02
 A = 1.504050539208448E+08 AD= 1.529087800093374E+08 PR= 3.179277733347012E+07
$$EOE
*******************************************************************************
 
TIME

  Barycentric Dynamical Time ("TDB" or T_eph) output was requested. This
continuous coordinate time is equivalent to the relativistic proper time
of a clock at rest in a reference frame co-moving with the solar system
barycenter but outside the system's gravity well. It is the independent
variable in the solar system relativistic equations of motion.

  TDB runs at a uniform rate of one SI second per second and is independent
of irregularities in Earth's rotation.
 
CALENDAR SYSTEM

  Mixed calendar mode was active such that calendar dates after AD 1582-Oct-15
(if any) are in the modern Gregorian system. Dates prior to 1582-Oct-5 (if any)
are in the Julian calendar system, which is automatically extended for dates
prior to its adoption on 45-Jan-1 BC.  The Julian calendar is useful for
matching historical dates. The Gregorian calendar more accurately corresponds
to the Earth's orbital motion and seasons. A "Gregorian-only" calendar mode is
available if such physical events are the primary interest.

REFERENCE FRAME AND COORDINATES

  Ecliptic at the standard reference epoch

    Reference epoch: J2000.0
    X-Y plane: adopted Earth orbital plane at the reference epoch
               Note: IAU76 obliquity of 84381.448 arcseconds wrt ICRF X-Y plane
    X-axis   : ICRF
    Z-axis   : perpendicular to the X-Y plane in the directional (+ or -) sense
               of Earth's north pole at the reference epoch.

  Symbol meaning:

    JDTDB    Julian Day Number, Barycentric Dynamical Time
      EC     Eccentricity, e
      QR     Periapsis distance, q (km)
      IN     Inclination w.r.t X-Y plane, i (degrees)
      OM     Longitude of Ascending Node, OMEGA, (degrees)
      W      Argument of Perifocus, w (degrees)
      Tp     Time of periapsis (Julian Day Number)
      N      Mean motion, n (degrees/sec)
      MA     Mean anomaly, M (degrees)
      TA     True anomaly, nu (degrees)
      A      Semi-major axis, a (km)
      AD     Apoapsis distance (km)
      PR     Sidereal orbit period (sec)

ABERRATIONS AND CORRECTIONS

 Geometric osculating elements have NO corrections or aberrations applied.

Computations by ...

    Solar System Dynamics Group, Horizons On-Line Ephemeris System
    4800 Oak Grove Drive, Jet Propulsion Laboratory
    Pasadena, CA  91109   USA

    General site: https://ssd.jpl.nasa.gov/
    Mailing list: https://ssd.jpl.nasa.gov/email_list.html
    System news : https://ssd.jpl.nasa.gov/horizons/news.html
    User Guide  : https://ssd.jpl.nasa.gov/horizons/manual.html
    Connect     : browser        https://ssd.jpl.nasa.gov/horizons/app.html#/x
                  API            https://ssd-api.jpl.nasa.gov/doc/horizons.html
                  command-line   telnet ssd.jpl.nasa.gov 6775
                  e-mail/batch   https://ssd.jpl.nasa.gov/ftp/ssd/horizons_batch.txt
                  scripts        https://ssd.jpl.nasa.gov/ftp/ssd/SCRIPTS
    Author      : Jon.D.Giorgini@jpl.nasa.gov
*******************************************************************************)"