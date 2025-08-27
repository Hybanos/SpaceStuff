#include <ecs/ecs.hpp>

ECSTable::ECSTable() {
}

size_t ECSTable::request_entity() {
    bits.push_back(0);

    if (!archetype_map.contains(0)) archetype_map[0] = Archetype(0);
    size_t id = archetype_map[0].request_entity();
    virtual_to_local_ids.push_back(id);

    return bits.size() - 1;
}

void ECSTable::remove_entity(size_t entity_id) {
    archetype_map[bits[entity_id]].remove_entity(virtual_to_local_ids[entity_id]);
    
    bits.erase(bits.begin() + entity_id);
    virtual_to_local_ids.erase(virtual_to_local_ids.begin() + entity_id);
}

void ECSTable::copy_entity(bitset old_arch, size_t old_arch_id, bitset new_arch, size_t new_arch_id) {

    #define X(ENUM, TYPE) \
    if (old_arch.test(ENUM) && new_arch.test(ENUM)) { \
        TYPE old_val = archetype_map[old_arch].get_##TYPE(old_arch_id); \
        archetype_map[new_arch].set_##TYPE(new_arch_id, old_val); \
    }
    COMPONENTS
    #undef X

}

void ECSTable::set_component(size_t entity_id, Component component) {
    bitset b = bits[entity_id];
    b.set(component, true);
    set_bits(entity_id, b);
}

void ECSTable::remove_component(size_t entity_id, Component component) {
    bitset b = bits[entity_id];
    b.set(component, false);
    set_bits(entity_id, b);
}

void ECSTable::set_bits(size_t entity_id, bitset b) {
    bitset old_arch = bits[entity_id];
    size_t old_id = virtual_to_local_ids[entity_id];
    // bits[entity_id].set(component, val);
    bits[entity_id] = b;
    bitset new_arch = bits[entity_id];

    if (!archetype_map.contains(new_arch)) archetype_map[new_arch] = Archetype(new_arch);
    size_t new_id = archetype_map[new_arch].request_entity();

    copy_entity(old_arch, old_id, new_arch, new_id);
    virtual_to_local_ids[entity_id] = new_id;

    archetype_map[old_arch].remove_entity(old_id);
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
