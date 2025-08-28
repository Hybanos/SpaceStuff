#include "ecs/archetype.hpp"

size_t Archetype::bytes() {
    size_t mult = 0;
    #define X(ENUM, TYPE) \
    if (bits.test(ENUM)) mult += sizeof(TYPE);
    COMPONENTS
    #undef X
    return _size * mult;
}

#define X(ENUM, TYPE) \
TYPE &Archetype::get_##TYPE(size_t entity_id) { \
return TYPE##_v[entity_id]; \
}
COMPONENTS
#undef X

#define X(ENUM, TYPE) \
void Archetype::set_##TYPE(size_t entity_id, TYPE value) { \
    TYPE##_v[entity_id] = value; \
}
COMPONENTS
#undef X

size_t Archetype::request_entity() {
    #define X(ENUM, TYPE) \
    if (bits.test(ENUM)) TYPE##_v.push_back(TYPE{});
    COMPONENTS
    #undef X
    return _size++;
}

void Archetype::remove_entity(size_t entity_id) {
    #define X(ENUM, TYPE) \
    if (bits.test(ENUM)) TYPE##_v.erase(TYPE##_v.begin() + entity_id);
    COMPONENTS
    #undef X

    _size--;
}