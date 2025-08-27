#pragma once

#include <vector>
#include <map>

#include <fmt/core.h>

#include "components.hpp"

class Archetype {
    private:
        int bits;
        #define X(ENUM, TYPE) \
        std::vector<TYPE> TYPE##_v;
        COMPONENTS
        #undef X
    public:
        Archetype(int b = 0) : bits{b} {}

        #define X(ENUM, TYPE) \
        TYPE &get_##TYPE(size_t entity_id);
        COMPONENTS
        #undef X

        #define X(ENUM, TYPE) \
        void set_##TYPE(size_t entity_id, TYPE value);
        COMPONENTS
        #undef X

        size_t request_entity();
};