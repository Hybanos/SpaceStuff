#pragma once

#include <glm/glm.hpp>

#define FREE_ENTITY NUM_COMPONENT

// Thanks github.com/natsu-anon/ecs-demo for the macro work
#define COMPONENTS \
    X(POSITION, Position) \
    X(ROTATION, Rotation) \
    X(SCALE, Scale) 

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
} Rotation;

