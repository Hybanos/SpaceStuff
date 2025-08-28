#pragma once

#include <vector>
#include <map>

#include <fmt/core.h>

#include "components.hpp"

typedef std::bitset<NUM_COMPONENT> bitset;

class Archetype {
    private:
        bitset bits;
        size_t _size = 0;
        #define X(ENUM, TYPE) \
        std::vector<TYPE> TYPE##_v;
        COMPONENTS
        #undef X
    public:
        Archetype(bitset b = 0) : bits{b} {}
        size_t size() {return _size;}
        size_t bytes();

        #define X(ENUM, TYPE) \
        TYPE &get_##TYPE(size_t entity_id);
        COMPONENTS
        #undef X

        #define X(ENUM, TYPE) \
        void set_##TYPE(size_t entity_id, TYPE value);
        COMPONENTS
        #undef X

        size_t request_entity();
        void remove_entity(size_t entity_id);
    };