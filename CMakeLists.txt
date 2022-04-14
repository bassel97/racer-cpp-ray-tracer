cmake_minimum_required(VERSION 3.0.0)

project(RACER VERSION 0.8)

# These are the options we select for building GLFW as a library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)        # Don't build Documentation
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)       # Don't build Tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)    # Don't build Examples
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)           # Don't build Installation Information
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)     # Add variables to use High Performance Graphics Card if available
add_subdirectory(vendor/glfw)                       # Build the GLFW project to use later as a library
add_subdirectory(vendor/glad)                       # Add GLAD folder to be later used


# A variables with all the source files of Dear ImGui
set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_impl/imgui_impl_glfw.cpp
        vendor/imgui/imgui_impl/imgui_impl_opengl3.cpp
        )

# Vendor source files variable
set(VENDOR_SOURCES ${IMGUI_SOURCES})

set(SourceCode
    source/application/application.cc
    source/application/window.cc
    source/application/application-ui.cc

    source/data/scene.cc
    source/data/component.cc
    source/data/entity.cc

    source/data/components/transform.cc
    source/data/components/renderer/renderer-component.cc
    source/data/components/renderer/sphere-renderer.cc

    source/systems/real-time-renderer/real-time-renderer.cc
    source/systems/ray-tracing-renderer/ray-tracing-renderer.cc
    )

# add the executable
add_executable(RACER source/main.cc resources.rc ${VENDOR_SOURCES} ${SourceCode})

# Source Libraries
target_include_directories(RACER PUBLIC
    source/application
    source/common
    source/data
    source/game
    source/ui
    source/ecs
    source/systems
   )

# Vendor Libraries
target_include_directories(RACER
    PUBLIC vendor/glad/include
    PUBLIC vendor/imgui
    PUBLIC vendor/json
    PUBLIC vendor/glm
    PUBLIC vendor/stb
    PUBLIC vendor/tiny-obj-loader
    )

target_link_libraries(RACER
    PUBLIC glfw
    PUBLIC glad)