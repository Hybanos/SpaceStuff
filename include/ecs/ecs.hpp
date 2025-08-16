#pragma once

#include <cstddef>
#include <unordered_map>
#include <functional>

#include "fmt/core.h"

#include "ecs/components.hpp"

typedef std::function<void (void)> system_function_t;

class ECSTable {
    private:
    public:
        void **component_table;
        int *bits;
        // size_t component_sizes[NUM_COMPONENT];
        size_t size = 0;
        size_t max = 1 << 16;

        // std::unordered_map<int, system_function_t> systems;
        // void sort();
        ECSTable();
        ~ECSTable();

        size_t request_entity();
        void remove_entity(size_t entity_id);
        void set_component(size_t entity_id, Component component);
        // void commit() {sort();}

        // void request_system(int bits, system_function_t);

        #define X(ENUM, TYPE) \
        void set_##TYPE(size_t entity_id, TYPE value); 
        COMPONENTS
        #undef X
};