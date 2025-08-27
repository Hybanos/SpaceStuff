#include "archetype.hpp"

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
    if (bits & (1 << ENUM)) TYPE##_v.push_back(TYPE{});
    COMPONENTS
    #undef X
}