#include <ecs/ecs.hpp>

ECSTable::ECSTable() {
}

size_t ECSTable::request_entity() {
    bits.push_back(0);

    if (!archetype_map.contains(0)) archetype_map[0] = Archetype(0);
    size_t id = archetype_map[0].request_entity();
    // virtual_to_local_ids.emplace(bits.size(), id);
    virtual_to_local_ids.push_back(id);

    return bits.size() - 1;
}

void ECSTable::remove_entity(size_t entity_id) {
    bits[entity_id] = ~(1 << FREE_ENTITY);
}

void ECSTable::set_component(size_t entity_id, Component component) {
    bits[entity_id] |= (1 << component);
}

void ECSTable::set_bits(size_t entity_id, bitset b) {
    if (b == bits[entity_id]) return;


}

size_t ECSTable::get_first(bitset b) {
    size_t i = 0;
    while (1) {
        if ((bits[i] & b) == b) return i;
        i++;
    }
}

size_t ECSTable::get_last(bitset b) {
    size_t i = bits.size();
    while (1) {
        if ((bits[i-1] & b) == b) return i;
        i--;
    }
}

#define X(ENUM, TYPE) \
void ECSTable::set_##TYPE(size_t entity_id, TYPE value) { \
    archetype_map[bits[entity_id]].set_##TYPE(virtual_to_local_ids[entity_id], value); \
}
COMPONENTS
#undef X

#define X(ENUM, TYPE) \
TYPE &ECSTable::get_##TYPE(size_t entity_id) { \
    return archetype_map[bits[entity_id]].get_##TYPE(virtual_to_local_ids[entity_id]); \
}
COMPONENTS
#undef X
