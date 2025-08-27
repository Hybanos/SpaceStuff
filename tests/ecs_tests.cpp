#include <gtest/gtest.h>
#include <ecs/ecs.hpp>

TEST(ECS_tests, entity_move) {
    ECSTable table;

    size_t e = table.request_entity();
    EXPECT_EQ(table.bits[e].to_ulong(), 0);
    EXPECT_EQ(table.get_archetype_size(0), 1);

    table.set_component(e, POSITION);
    EXPECT_EQ(table.bits[e].to_ulong(), (1 << POSITION));
    EXPECT_EQ(table.get_archetype_size(1 << POSITION), 1);

    table.set_component(e, ROTATION);
    EXPECT_EQ(table.bits[e].to_ulong(), (1 << POSITION) | (1 << ROTATION));
    EXPECT_EQ(table.get_archetype_size((1 << POSITION) | (1 << ROTATION)), 1);

    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.get_archetype_size(0), 0);

    table.set_bits(e, (1 << POSITION) | (1 << ROTATION));
    EXPECT_EQ(table.bits[e].to_ulong(), (1 << POSITION) | (1 << ROTATION));
    EXPECT_EQ(table.get_archetype_size((1 << POSITION) | (1 << ROTATION)), 1);
    table.set_bits(e, 0);
    EXPECT_EQ(table.bits[e].to_ulong(), 0);
    EXPECT_EQ(table.get_archetype_size(0), 1);
    EXPECT_EQ(table.get_archetype_size((1 << POSITION) | (1 << ROTATION)), 0);

    table.remove_entity(e);
    EXPECT_EQ(table.size(), 0);

    for (int i = 0; i < NUM_COMPONENT; i++) {
        EXPECT_EQ(table.get_archetype_size(1 << i), 0);
    }
}

TEST(ECS_tests, data_persist) {
    ECSTable table;
    Position p1(1, 2, 3);
    Rotation r1 = glm::mat3(1);

    size_t e1 = table.request_entity();

    table.set_component(e1, POSITION);
    table.set_Position(e1, p1);
    EXPECT_EQ(table.get_Position(e1), p1);

    table.set_component(e1, ROTATION);
    table.set_Rotation(e1, r1);
    EXPECT_EQ(table.get_Rotation(e1), r1);
    EXPECT_EQ(table.get_Position(e1), p1);

    table.remove_component(e1, ROTATION);
    EXPECT_EQ(table.get_Position(e1), p1);
}

TEST(ECS_tests, data_persist_mult_entities) {
    ECSTable table;
    Position p1(1, 2, 3);

    size_t e1 = table.request_entity();
    table.set_component(e1, POSITION);
    table.set_Position(e1, p1 * 1.0f);

    size_t e2 = table.request_entity();
    table.set_component(e2, POSITION);
    table.set_Position(e2, p1 * 2.0f);

    size_t e3 = table.request_entity();
    table.set_component(e3, POSITION);
    table.set_Position(e3, p1 * 3.0f);

    EXPECT_EQ(table.size(), 3);
    EXPECT_EQ(table.get_archetype_size((1 << POSITION)), 3);

    EXPECT_EQ(e1, 0ull);
    EXPECT_EQ(e2, 1ull);
    EXPECT_EQ(e3, 2ull);

    EXPECT_EQ(table.get_Position(e1), p1 * 1.0f);
    EXPECT_EQ(table.get_Position(e2), p1 * 2.0f);
    EXPECT_EQ(table.get_Position(e3), p1 * 3.0f);

    table.remove_entity(e2);

    EXPECT_EQ(table.get_Position(e1), p1 * 1.0f);
    EXPECT_EQ(table.get_Position(e3), p1 * 3.0f);

}