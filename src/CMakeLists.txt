add_executable(main)

target_sources(main PRIVATE
    objects/grid.cpp
    objects/orbits.cpp
    objects/skybox.cpp
    objects/sphere.cpp
    objects/particle.cpp
    objects/barycenter.cpp
    objects/ring.cpp

    scene/camera.cpp
    scene/scene.cpp
    scene/time.cpp

    data/db_manager.cpp
    data/position.cpp

    render/shader.cpp
    render/mesh.cpp
    render/objects/orbits.cpp

    ecs/ecs.cpp
    ecs/systems.cpp

    main.cpp
)

target_sources(main PUBLIC 
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES

        ${PROJECT_SOURCE_DIR}/include/objects/grid.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/orbits.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/skybox.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/sphere.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/particle.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/barycenter.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/ring.hpp

        ${PROJECT_SOURCE_DIR}/include/scene/camera.hpp
        ${PROJECT_SOURCE_DIR}/include/scene/camera_anchor.hpp
        ${PROJECT_SOURCE_DIR}/include/scene/scene.hpp
        ${PROJECT_SOURCE_DIR}/include/scene/time.hpp
        ${PROJECT_SOURCE_DIR}/include/scene/signal.hpp

        ${PROJECT_SOURCE_DIR}/include/data/db_manager.hpp
        ${PROJECT_SOURCE_DIR}/include/data/tle.hpp
        ${PROJECT_SOURCE_DIR}/include/data/horizons.hpp
        ${PROJECT_SOURCE_DIR}/include/data/position.hpp

        ${PROJECT_SOURCE_DIR}/include/render/shader.hpp
        ${PROJECT_SOURCE_DIR}/include/render/mesh.hpp
        ${PROJECT_SOURCE_DIR}/include/render/objects/orbits.hpp

        ${PROJECT_SOURCE_DIR}/include/ecs/ecs.hpp
        ${PROJECT_SOURCE_DIR}/include/ecs/components.hpp
        ${PROJECT_SOURCE_DIR}/include/ecs/systems.hpp
)

target_sources(
    main
	PRIVATE
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

target_compile_options(main PRIVATE -DGLM_ENABLE_EXPERIMENTAL -DLIBGL_DEBUG -g3) 
target_link_libraries(main PRIVATE SDL3::SDL3 OpenGL::GL GLEW SQLite::SQLite3 fmt::fmt cpr::cpr nlohmann_json::nlohmann_json)
target_include_directories(main PRIVATE ${glm_SOURCE_DIR} ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_features(main PRIVATE cxx_std_20)