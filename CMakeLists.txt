set(CMAKE_MINIMUM_REQUIRED_VERSION 3.2)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/deps/include)
link_directories(${PROJECT_SOURCE_DIR}/deps/libs)

add_executable(release Main.cpp)
target_link_libraries(release glfw3 GL)
# On Windows target_link_libraries(release glfw3 opengl32 gdi32)
