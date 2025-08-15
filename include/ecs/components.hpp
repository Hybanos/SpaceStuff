#pragma once

#include <glm/glm.hpp>

#define FREE_ENTITY NUM_COMPONENT

typedef enum Component {
    POSITION,
    ROTATION,
    SCALE,
    VELOCITY,

    NUM_COMPONENT,
};

typedef struct Position {
    float x;
    float y;
    float z;
} Position;

typedef struct Rotation {
    glm::mat3 rota;
} Rotation;

typedef struct Rotation {
    float scale;
} Rotation;

typedef struct Velocity {
    float x;
    float y;
    float z;
} Velocity;
