cmake_minimum_required(VERSION 3.0.0)

project(RACER VERSION 0.1.0)

# These are the options we select for building GLFW as a library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)        # Don't build Documentation
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)       # Don't build Tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)    # Don't build Examples
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)           # Don't build Installation Information
set(GLFW_USE_HYBRID_HPG ON CACHE BOOL "" FORCE)     # Add variables to use High Performance Graphics Card if available
add_subdirectory(vendor/glfw)                       # Build the GLFW project to use later as a library

# A variables with all the source files of Dear ImGui
set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_impl/imgui_impl_glfw.cpp
        vendor/imgui/imgui_impl/imgui_impl_opengl3.cpp
        )

# A variables with all the source files of Dear ImGui
set(RACER_SOURCES
        source/common/shapes/sphere/sphere-shape.cpp
        )

include_directories(
    source/common
    vendor/glm/
    vendor/glfw/include
    vendor/glad/include
    vendor/imgui
    vendor/utils
    vendor/JSON
)

# Source files of GLAD
set(GLAD_SOURCE vendor/glad/src/gl.c)

# Vendor source files variable
set(VENDOR_SOURCES ${GLAD_SOURCE} ${IMGUI_SOURCES})

# add the executable
add_executable(RACER source/main.cpp ${VENDOR_SOURCES} ${RACER_SOURCES})
target_link_libraries(RACER glfw)