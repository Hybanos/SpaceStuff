#pragma once

#include <cstddef>

#include <fmt/core.h>

#include <ecs/components.hpp>

class ECSTable {
    private:
        void **component_table;
        int *bits;
        size_t component_sizes[NUM_COMPONENT];
        size_t size = 0;
        size_t max = 1 << 20;

        void sort();
    public:
        ECSTable();
        ~ECSTable();

        size_t request_entity();
        // void remove_entity(size_t entity_id);
        void set_component(size_t entity_id, Component component);
        void commit() {sort();}
};