#include <ecs/ecs.hpp>

ECSTable::ECSTable() {
    component_table = (void **) malloc(sizeof(void *) * NUM_COMPONENT);
    #define X(ENUM, TYPE) \
    component_table[ENUM] = (TYPE *) malloc(sizeof(TYPE) * max);
    COMPONENTS
    #undef X
    bits = (int *) malloc(sizeof(int) * max);
}

ECSTable::~ECSTable() {
    for (int i = 0; i < NUM_COMPONENT; i++) {
        free(component_table[i]);
    }
    free(component_table);
    free(bits);
}

// void ECSTable::sort() {
//     size_t first, last;

    // remove free entities first
    // while (bits[first] & ~(1 << FREE_ENTITY)) {
    //     first++;
    // }

    // last = first + 1;
    // while (bits[last] & (1 << FREE_ENTITY)) {
    //     last++;
    // }
// }

size_t ECSTable::request_entity() {
    if (size == max) {
        fmt::print("Entity limit reached!");
        exit(1);
    }

    bits[size] = 0;
     
    return size++;
}

void ECSTable::remove_entity(size_t entity_id) {
    bits[entity_id] = ~(1 << FREE_ENTITY);
}

void ECSTable::set_component(size_t entity_id, Component component) {
    bits[entity_id] |= (1 << component);
    // component_sizes[component]++;
}

// void ECSTable::request_system(int bits, system_function_t fn) {
//     systems.emplace(bits, fn);
// }

#define X(ENUM, TYPE) \
void ECSTable::set_##TYPE(size_t entity_id, TYPE value) { \
    ((TYPE *) component_table[ENUM])[entity_id] = value; \
}
COMPONENTS
#undef X
