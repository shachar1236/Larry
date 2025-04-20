# In my_project/core/CMakeLists.txt (or my_project/CMakeLists.txt)
cmake_minimum_required(VERSION 3.0)
project(LarryApi)

# ---------- Larry Utils -------------
add_library(LarryApi SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/Log.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/ECS.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/Math.cpp
            )

target_include_directories(LarryApi PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/
    )

target_include_directories(LarryApi
               PUBLIC
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/
               ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/
               )

target_link_libraries(LarryApi PUBLIC spdlog)
