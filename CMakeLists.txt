cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

message("Build has been started!")
message(${CMAKE_SYSTEM})
message(${CMAKE_SYSTEM_NAME})

# TODO(andrew): build glfw, gtest, gmock

set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/External)
set(ENGINE_DIR ${PROJECT_SOURCE_DIR}/Tiny3D)
set(CORE_DIR ${ENGINE_DIR}/Core)

file(GLOB CORE_FILES ${CORE_DIR}/*.cpp ${CORE_DIR}/*.hpp)
file(GLOB ENGINE_FILES ${ENGINE_DIR}/*.cpp ${ENGINE_DIR}/*.hpp)

add_executable(test_suit Main.cpp ${CORE_FILES} ${ENGINE_FILES})
target_include_directories(test_suit PRIVATE ${EXTERNAL_DIR}/glm/)
