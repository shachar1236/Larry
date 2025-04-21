cmake_minimum_required(VERSION 3.26) # Set the minimum required CMake version

project(LarryEngine) # Set the project name

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_subdirectory(libraries/glfw)

set(SPDLOG_BUILD_SHARED ON)
add_subdirectory(libraries/spdlog)

add_subdirectory(Larry/LarryApi)

# addd gamescripts
# add_subdirectory(SandboxGame)

set(SOURCE_FILES
    libraries/glad/src/glad.c

    Larry/src/main.cpp
    Larry/src/Layers/Layer/Layer.cpp
    Larry/src/Application/Application.cpp
    Larry/src/Application/BackgroundLayer.cpp
    Larry/src/Application/GameLayer.cpp
    Larry/src/Application/UILayer.cpp
    Larry/src/Application/GUILayer.cpp

    Larry/src/Renderer/LarryWindow.cpp
    Larry/src/Renderer/Renderer.cpp
    Larry/src/Renderer/Shader.cpp
    Larry/src/Renderer/ShaderProgram.cpp
    Larry/src/Renderer/VertexArrayObject.cpp

    Larry/src/Input/Input.cpp

    Larry/src/Math/Math.cpp

    Larry/src/EventSystem/EventSystem.cpp

    Larry/src/Application/Systems/RenderQuad.cpp
    Larry/src/Application/Systems/ButtonSystem.cpp
    Larry/src/Application/Systems/ScriptsSystem.cpp

    # Larry/src/Scripts/Scripts.cpp
)
set(HEADER_DIRECTORIES
    Larry/src
    Larry/src/ECS
    Larry/src/Application
    Larry/src/Utils
    Larry/src/Input
    Larry/src/Math
    Larry/src/Renderer
    Larry/src/EventSystem
    Larry/src/Layers/Layer
    Larry/src/Layers
)

set(LarrySharedLibraries
    LarryApi
)

if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS}) # OpenGL
    include_directories(${GLFW_INCLUDE_DIR}) # Add GLFW include directory
    include_directories(libraries/glad/include) # GLAD
    include_directories(libraries/stb_image)
    include_directories(libraries)

    include_directories(${HEADER_DIRECTORIES}) # My Headers
    add_executable(LarryEngine ${SOURCE_FILES})  # My executables

    if(NOT (CMAKE_BUILD_TYPE STREQUAL "Release"))
        message(STATUS "Compiling with dev defination")
        target_compile_definitions(LarryEngine PRIVATE DEV)
    endif()
    if(Test)
        message(STATUS "Testing...")
        target_compile_definitions(LarryEngine PRIVATE LARRY_ENABLE_TESTING)
    endif()

    # target_precompile_headers(LarryEngine PRIVATE Larry/src/LarryApi.h)

    target_link_directories(LarryEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SandboxGame/build)
    target_link_libraries(LarryEngine ${LarrySharedLibraries} ${OPENGL_LIBRARIES} GameScripts glfw)

    # target_link_libraries(LarryEngine ${LarrySharedLibraries} ${OPENGL_LIBRARIES} glfw)
else()
    message(FATAL_ERROR "OpenGL not found")
endif()

