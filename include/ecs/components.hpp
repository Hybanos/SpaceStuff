#pragma once

#include "glm/glm.hpp"

#include "data/tle.hpp"

#define FREE_ENTITY NUM_COMPONENT

// Thanks github.com/natsu-anon/ecs-demo for the macro work
#define COMPONENTS \
    X(POSITION, Position) \
    X(ROTATION, Rotation) \
    X(SCALE, Scale) \
    X(TWO_LINE_ELEMENT, TLE) \
    X(ORBIT, Orbit)

#define X(ENUM, TYPE) ENUM,
typedef enum Component {
    COMPONENTS

    NUM_COMPONENT,
} Component;
#undef X

typedef struct Position {
    float x;
    float y;
    float z;
} Position;

typedef struct Rotation {
    glm::mat3 rota;
} Rotation;

typedef struct Scale {
    float scale;
} Scale;

typedef struct Orbit {
    float semi_major_axis;
    float semi_minor_axis;
    double epoch;
    float true_anomaly;
    float true_anomaly_index[360];
    glm::vec3 offset;
    float flag;
} Orbit;

