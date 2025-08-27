enable_testing()
add_executable(tests)

target_sources(tests PRIVATE 
    ../src/ecs/ecs.cpp
    ../src/ecs/archetype.cpp

    ecs_tests.cpp
)

target_sources(tests PUBLIC 
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES

        ${PROJECT_SOURCE_DIR}/include/ecs/ecs.hpp
        ${PROJECT_SOURCE_DIR}/include/ecs/archetype.hpp
        ${PROJECT_SOURCE_DIR}/include/ecs/components.hpp
)

target_compile_options(tests PRIVATE -DGLM_ENABLE_EXPERIMENTAL)
target_link_libraries(tests PRIVATE GTest::gtest_main fmt::fmt nlohmann_json::nlohmann_json)
target_include_directories(tests PRIVATE ${glm_SOURCE_DIR})
target_compile_features(tests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(tests)