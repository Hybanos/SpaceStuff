#pragma once

#include <array>

#include "glm/glm.hpp"

#include "data/tle.hpp"

#define FREE_ENTITY NUM_COMPONENT

// Thanks github.com/natsu-anon/ecs-demo for the macro work
#define COMPONENTS \
    X(POSITION, Position) \
    X(ROTATION, Rotation) \
    X(SCALE, Scale) \
    X(TWO_LINE_ELEMENT, TLE) \
    X(ORBIT, Orbit) \
    X(EPOCH, Epoch) \
    X(TRUE_ANOMALY_INDEX, AnomalyIndex)

#define X(ENUM, TYPE) ENUM,
typedef enum Component {
    COMPONENTS

    NUM_COMPONENT,
} Component;
#undef X

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

