set(CMAKE_MINIMUM_REQUIRED_VERSION 3.2)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

message("Build has been started!")
message(${CMAKE_SYSTEM})

include_directories(${PROJECT_SOURCE_DIR}/deps/include)
link_directories(${PROJECT_SOURCE_DIR}/deps/libs)

set(ENGINE_INCLUDES Engine/Scene.hpp Engine/Component.hpp Engine/Camera.hpp Engine/Math.hpp Engine/Core/Utils.hpp)
set(ENGINE_SOURCES Engine/Scene.cpp Engine/Component.cpp Engine/Camera.cpp)

add_executable(release Main.cpp ${ENGINE_INCLUDES} ${ENGINE_SOURCES})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(release glfw3 opengl32 gdi32)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(release glfw3 GL)
else ()
    message(FATAL_ERROR "FUCK YOU WITH SUCH SYSTEM!!!")
endif ()
