# In my_project/core/CMakeLists.txt (or my_project/CMakeLists.txt)
cmake_minimum_required(VERSION 3.0)
project(LarryApi)

# ---------- Larry Utils -------------
add_library(LarryApi SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/glad/src/glad.c
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/Log.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/ECS.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/Math.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/TextureLoader/TextureObject.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/TextureLoader/TextureLoader.cpp
            )

target_include_directories(LarryApi PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/stb_image
    )

target_include_directories(LarryApi
               PUBLIC
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/TextureLoader/
               )

target_link_libraries(LarryApi PUBLIC spdlog)
