# In my_project/core/CMakeLists.txt (or my_project/CMakeLists.txt)
cmake_minimum_required(VERSION 3.26)
project(LarryApi)

file(GLOB_RECURSE COMPONENT_HEADERS 
    "include/Components/**.h"
)

set(OUT_HPP "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/lua/LarryApiBase.hpp")
set(OUT_LUA "${CMAKE_CURRENT_SOURCE_DIR}/include/Scripts/lua/LarryApiBase.lua")

# 3. The custom command now uses the list of files
add_custom_command(
    OUTPUT "${OUT_HPP}" "${OUT_LUA}"
    # Use the variable ${COMPONENT_HEADERS} instead of the wildcard string
    COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/../../cpp_to_lua_binding_generator/bind_cpp_to_luajit.py" 
            ${COMPONENT_HEADERS} 
            -o "${OUT_HPP}" "${OUT_LUA}"
            -I "${CMAKE_CURRENT_SOURCE_DIR}/"
            -I "${CMAKE_CURRENT_SOURCE_DIR}/include"
            --ignore-classes \"convert<Larry::TextureLoader>\" 
            --exists Vec1 Vec2 Vec3 Vec4 Entity ECS_Entity
            --namespaces Larry Larry::Math Larry::ECS
            --skip-errors 
            --custom-ffi-cdef CDef
    DEPENDS ${COMPONENT_HEADERS}
    COMMENT "Generating Lua bindings from headers..."
    VERBATIM
)

add_custom_target(GenerateLuaBindings ALL
    DEPENDS "${OUT_HPP}" "${OUT_LUA}"
)

add_library(LarryApi SHARED
        Layers/LayerStack.cpp

        Utils/Log.cpp

        ECS/Internal/ECS.cpp
        ECS/Internal/TypesBitmap.cpp
        ECS/CPPApi/ECS.cpp
        ECS/CPPApi/ecs_test.cpp

        Math/Math.cpp

        TextureLoader/TextureObject.cpp
        TextureLoader/TextureLoader.cpp

        Input/Input.cpp

        Scripts/lua/LuaScripts.cpp
        Scripts/lua/LarryApiFunctions.cpp

        EventSystem/EventSystem.cpp
    )

target_include_directories(LarryApi PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/stb_image
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/luajit/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/sol2/include
)

target_include_directories(LarryApi
       PUBLIC
       
       ./
       include/
       include/Utils/
       include/ECS/
       include/ECS/Internal/
       include/ECS/CPPApi/
       include/Math/
       include/TextureLoader/
       include/Input/
       include/Layers/
       include/Scripts/lua
       include/Components
   )

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Release"))
    message(STATUS "Compiling with dev defination")
    target_compile_definitions(LarryApi PRIVATE DEV)
endif()
# target_precompile_headers(LarryApi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/LarryApi.h)
# target_precompile_headers(LarryApi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/common.h)
# target_precompile_headers(LarryApi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS_pch.h)

target_link_libraries(LarryApi PUBLIC spdlog yaml-cpp)
