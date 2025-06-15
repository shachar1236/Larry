# In my_project/core/CMakeLists.txt (or my_project/CMakeLists.txt)
cmake_minimum_required(VERSION 3.26)
project(LarryApi)

# ---------- Larry Utils -------------
add_library(LarryApi SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Layers/LayerStack.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/Log.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/Internal/ECS.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/Internal/TypesBitmap.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/CPPApi/ECS.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/CPPApi/ecs_test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/Math.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/TextureLoader/TextureObject.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/TextureLoader/TextureLoader.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Input/Input.cpp
            )

target_include_directories(LarryApi PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/stb_image
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/luajit/src
    )

target_include_directories(LarryApi
               PUBLIC
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/Internal/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/CPPApi/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/TextureLoader/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Input/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Layers/
               )

# target_precompile_headers(LarryApi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/LarryApi.h)
# target_precompile_headers(LarryApi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/common.h)
# target_precompile_headers(LarryApi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS_pch.h)

target_link_libraries(LarryApi PUBLIC spdlog yaml-cpp)
