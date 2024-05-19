cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
cmake_policy(VERSION 3.5)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

message("Build has been started!")
message(${CMAKE_SYSTEM})
message(${CMAKE_SYSTEM_NAME})

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Includes)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/Libs)

set(ENGINE_DIR ${PROJECT_SOURCE_DIR}/Tiny3D)
set(CORE_DIR ${PROJECT_SOURCE_DIR}/Tiny3D/Core)

file(GLOB CORE_FILES ${CORE_DIR}/*.cpp ${CORE_DIR}/*.hpp)
file(GLOB ENGINE_FILES ${ENGINE_DIR}/*.cpp ${ENGINE_DIR}/*.hpp)

add_executable(main Main.cpp ${CORE_FILES} ${ENGINE_FILES})
target_compile_definitions(main PRIVATE "SOURCE_DIR=${PROJECT_SOURCE_DIR}")
target_include_directories(main PRIVATE ${INCLUDE_DIR})
target_link_directories(main PRIVATE ${LIB_DIR})
target_link_libraries(main glad glfw3 GL)
