cmake_minimum_required(VERSION 3.26) # Set the minimum required CMake version

project(Larry) # Set the project name

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
# Add GLFW as a subdirectory
add_subdirectory(libraries/glfw)
add_subdirectory(libraries/spdlog)

set(SOURCE_FILES
    libraries/glad/src/glad.c

    Larry/src/main.cpp
    Larry/src/Application.cpp

    Larry/src/Renderer/TextureObject.cpp
    Larry/src/Renderer/Renderer.cpp
    Larry/src/Renderer/Shader.cpp
    Larry/src/Renderer/ShaderProgram.cpp
    Larry/src/Renderer/VertexArrayObject.cpp

    Larry/src/Utils/Log.cpp
)
set(HEADER_DIRECTORIES
    Larry/src
    Larry/src/Utils
    Larry/src/Renderer
)

if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS}) # OpenGL
    include_directories(${GLFW_INCLUDE_DIR}) # Add GLFW include directory
    include_directories(libraries/spdlog/include)  # SPDLOG
    include_directories(libraries/glad/include) # GLAD
    include_directories(libraries/stb_image)

    include_directories(${HEADER_DIRECTORIES}) # My Headers
    add_executable(Larry ${SOURCE_FILES})  # My executables
    target_link_libraries(Larry ${OPENGL_LIBRARIES} glfw spdlog) # Link GLFW
else()
    message(FATAL_ERROR "OpenGL not found")
endif()

