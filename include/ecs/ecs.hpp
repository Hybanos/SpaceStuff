#pragma once

#include <map>
#include <functional>
#include <bitset>

#include "fmt/core.h"

#include "ecs/components.hpp"
#include "ecs/archetype.hpp"

typedef std::bitset<NUM_COMPONENT> bitset;

class ECSTable {
    private:
        std::unordered_map<bitset, Archetype> archetype_map;

        void copy_entity(bitset old_arch, size_t old_arch_id, bitset new_arch, size_t new_arch_id);
        void collapse_local_ids(bitset arch, size_t deleted_entity);
    public:
        std::vector<bitset> bits;
        // maps the "virtual" entity ids given once at entity creation
        // with their local ids (meaning index in the archetype vectors)
        // std::map<size_t, size_t> virtual_to_local_ids;
        std::vector<size_t> virtual_to_local_ids;

        ECSTable();

        size_t request_entity();
        void remove_entity(size_t entity_id);
        void set_component(size_t entity_id, Component component);
        void remove_component(size_t entity_id, Component component);
        void set_bits(size_t entity, bitset b);
        size_t get_first(bitset b);
        size_t get_last(bitset b);
        size_t size() {return bits.size();}
        size_t get_archetype_size(bitset b) {return archetype_map[b].size();}

        #define X(ENUM, TYPE) \
        void set_##TYPE(size_t entity_id, TYPE value); 
        COMPONENTS
        #undef X

        #define X(ENUM, TYPE) \
        TYPE &get_##TYPE(size_t entity_id); 
        COMPONENTS
        #undef X
};