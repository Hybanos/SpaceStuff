enable_testing()
add_executable(tests)

target_sources(tests PRIVATE 
    ecs_tests.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)