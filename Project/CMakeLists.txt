cmake_minimum_required(VERSION 3.26) # Set the minimum required CMake version

project(LarryEngine) # Set the project name

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_subdirectory(libraries/glfw)

# Include FetchContent
include(FetchContent)

# Define yaml-cpp content
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        0.8.0 # Use a specific tag/version for stability
  # You can also use a commit hash: GIT_REV <commit_hash>
)

# Populate yaml-cpp (download and configure)
FetchContent_MakeAvailable(yaml-cpp)

set(SPDLOG_BUILD_SHARED ON)
add_subdirectory(libraries/spdlog)

add_subdirectory(Larry/LarryApi)

find_library(LUAJIT_LIBRARY
             NAMES luajit-5.1 luajit-5.1.a luajit # Common library names
             HINTS
                 $ENV{LUAJIT_HOME}/lib
                 /usr/local/lib
                 /usr/lib
                 /opt/local/lib
             DOC "Path to LuaJIT library file")

# add gamescripts
add_subdirectory(SandboxGame)

set(SOURCE_FILES
    libraries/glad/src/glad.c

    Larry/src/main.cpp
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

    Larry/src/Application/Systems/RenderQuad.cpp
    Larry/src/Application/Systems/ButtonSystem.cpp
    Larry/src/Application/Systems/ScriptsSystem.cpp
    Larry/src/Application/Systems/RelationsSystem.cpp

    Larry/src/Layer/Layer.cpp

    Larry/src/Scripts/ScriptsManager.cpp
    # Larry/src/Scripts/Scripts.cpp
)
set(HEADER_DIRECTORIES
    Larry/src
    Larry/src/Application
    Larry/src/Renderer
    Larry/src/Scripts
    Larry/src/Layer
)

set(LarrySharedLibraries
    LarryApi
)

set(LarryLuaScripts
    ${CMAKE_CURRENT_SOURCE_DIR}/Larry/LarryApi/Scripts/lua/LarryApiBase.lua
    ${CMAKE_CURRENT_SOURCE_DIR}/Larry/LarryApi/Scripts/lua/LarryApi.lua
    ${CMAKE_CURRENT_SOURCE_DIR}/Larry/LarryApi/Scripts/lua/CDef.lua
    ${CMAKE_CURRENT_SOURCE_DIR}/Larry/LarryApi/Scripts/lua/LarryKeyCodes.lua
    ${CMAKE_CURRENT_SOURCE_DIR}/Larry/LarryApi/Scripts/lua/TestLuaScript.lua
)

if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS}) # OpenGL
    include_directories(${GLFW_INCLUDE_DIR}) # Add GLFW include directory
    include_directories(libraries/glad/include) # GLAD
    include_directories(libraries/yaml-cpp/include)
    include_directories(libraries/stb_image)
    include_directories(libraries/luajit/src)
    include_directories(libraries/sol2/include)
    include_directories(libraries)

    include_directories(Larry/LarryApi/include)
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

    # target_precompile_headers(LarryEngine PRIVATE Larry/src/ECS_pch.h)
    # target_precompile_headers(LarryEngine PRIVATE Larry/src/common.h)
    # target_precompile_headers(LarryEngine PRIVATE Larry/src/LarryApi.h)

    # target_link_directories(LarryEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SandboxGame/build)

    target_link_libraries(LarryEngine ${LarrySharedLibraries} ${OPENGL_LIBRARIES} ${LUAJIT_LIBRARY} GameScripts glfw yaml-cpp)
    # target_link_libraries(LarryEngine ${LarrySharedLibraries} ${OPENGL_LIBRARIES} glfw)

    # file(COPY ${LarryLuaScripts} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    # add_custom_target(copyLuaScripts ALL
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different
                # ${LarryLuaScripts}
                # ${CMAKE_CURRENT_BINARY_DIR}
    # )
    # add_custom_target(copyLuaLib ALL
        # COMMAND ${CMAKE_COMMAND} -E copy_directory
        # ${CMAKE_CURRENT_SOURCE_DIR}/Larry/LarryApi/Scripts/lua/lib
                # ${CMAKE_CURRENT_BINARY_DIR}/lib
    # )
    # add_dependencies(copyLuaScripts)
else()
    message(FATAL_ERROR "OpenGL not found")
endif()

