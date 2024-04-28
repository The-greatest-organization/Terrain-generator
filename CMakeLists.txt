cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
cmake_policy(VERSION 3.5)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake_External/sanitizers/cmake)

message("Build has been started!")
message(${CMAKE_SYSTEM})
message(${CMAKE_SYSTEM_NAME})

# TODO(andrew): build glfw, gtest, gmock

find_package(Sanitizers)

set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/External)
set(ENGINE_DIR ${PROJECT_SOURCE_DIR}/Tiny3D)
set(CORE_DIR ${ENGINE_DIR}/Core)

# asan, ubsan, msan
set(SANITIZE_ADDRESS TRUE)
set(SANITIZE_UNDEFINED TRUE)
set(SANITIZE_MEMORY TRUE)

file(GLOB CORE_FILES ${CORE_DIR}/*.cpp ${CORE_DIR}/*.hpp)
file(GLOB ENGINE_FILES ${ENGINE_DIR}/*.cpp ${ENGINE_DIR}/*.hpp)

add_executable(test_suit Main.cpp ${CORE_FILES} ${ENGINE_FILES})
add_sanitizers(test_suit)
target_include_directories(test_suit PRIVATE ${EXTERNAL_DIR}/glm ${GLFW_INCLUDE_PATH})
target_link_directories(test_suit PRIVATE ${PROJECT_SOURCE_DIR}/Libs)
target_link_libraries(test_suit glfw3 GL)
