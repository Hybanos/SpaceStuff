add_executable(main)

target_sources(main PRIVATE
    objects/grid.cpp
    objects/object_cubemap.cpp
    objects/object_faces.cpp
    objects/object_mesh.cpp
    objects/orbit_line.cpp
    objects/orbits.cpp
    objects/skybox.cpp
    objects/sphere.cpp
    objects/particle.cpp

    scene/camera.cpp
    scene/scene.cpp

    main.cpp
)

target_sources(main PUBLIC 
    FILE_SET HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR}/include
    FILES

        ${PROJECT_SOURCE_DIR}/include/objects/grid.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/object_cubemap.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/object_faces.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/object_mesh.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/orbit_line.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/orbits.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/skybox.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/sphere.hpp
        ${PROJECT_SOURCE_DIR}/include/objects/particle.hpp

        ${PROJECT_SOURCE_DIR}/include/scene/camera.hpp
        ${PROJECT_SOURCE_DIR}/include/scene/scene.hpp
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
target_link_libraries(main PRIVATE SDL3::SDL3 OpenGL::GL GLEW)
target_include_directories(main PRIVATE ${glm_SOURCE_DIR} ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_compile_features(main PRIVATE cxx_std_20)