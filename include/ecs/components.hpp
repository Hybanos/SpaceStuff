#pragma once

#include <array>

#include "glm/glm.hpp"
#include "nlohmann/json.hpp"

#include "data/tle.hpp"
#include "data/horizons.hpp"

#define FREE_ENTITY NUM_COMPONENT

// Thanks github.com/natsu-anon/ecs-demo for the macro work
#define COMPONENTS \
    X(PARENT, Parent) \
    X(POSITION, Position) \
    X(ROTATION, Rotation) \
    X(SCALE, Scale) \
    X(TWO_LINE_ELEMENT, TLE) \
    X(ORBIT, Orbit) \
    X(EPOCH, Epoch) \
    X(TRUE_ANOMALY_INDEX, AnomalyIndex) \
    X(MAJOR_BODY, MajorBody) \
    X(EPHEMERIS, Ephemeris) \
    X(ROTATION_INFO, RotationInfo) \

#define X(ENUM, TYPE) ENUM,
typedef enum Component {
    COMPONENTS

    NUM_COMPONENT,
} Component;
#undef X

#define DRAWABLE_ORBIT ((1 << POSITION) | (1 << ROTATION) | (1 << TWO_LINE_ELEMENT) | (1 << ORBIT) | (1 << EPOCH) | (1 << TRUE_ANOMALY_INDEX))
#define DRAWABLE_SPHERE ((1 << POSITION) | (1 << ROTATION) | (1 << SCALE) | (1 << MAJOR_BODY) | (1 << EPHEMERIS) | (1 << ROTATION_INFO))

typedef size_t Parent;
typedef glm::vec3 Position;
typedef glm::mat3 Rotation;
typedef float Scale;

typedef struct Orbit {
    glm::vec3 offset;
    float semi_major_axis;
    float semi_minor_axis;
    float true_anomaly;
    float flag;
} Orbit;

typedef double Epoch;
typedef std::array<float, 360> AnomalyIndex;
typedef std::array<EphemerisLine, 365> Ephemeris;
typedef nlohmann::ordered_json RotationInfo;