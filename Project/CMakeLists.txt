cmake_minimum_required(VERSION 3.26) # Set the minimum required CMake version

project(GameEngine) # Set the project name

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
# Add GLFW as a subdirectory
add_subdirectory(glfw)

if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    include_directories(${GLFW_INCLUDE_DIR}) # Add GLFW include directory
    include_directories(glad/include) 
    add_executable(GameEngine src/main.cpp glad/src/glad.c) # Create an executable named HelloWorld from main.cpp
    target_link_libraries(GameEngine   ${OPENGL_LIBRARIES} glfw) # Link GLFW
else()
    message(FATAL_ERROR "OpenGL not found")
endif()

