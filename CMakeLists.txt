set(CMAKE_MINIMUM_REQUIRED_VERSION 3.2)
project(TerrainGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#include_directories(${PROJECT_SOURCE_DIR}/deps/include)
#link_directories(${PROJECT_SOURCE_DIR}/deps/libs)

add_executable(release Application/Main.cpp
        Application/Application.cpp
        Application/Window.cpp)

#if (CMAKE_SYSTEM_NAME EQUAL "Windows")
#    target_link_libraries(release glfw3 opengl32 gdi32)
#endif ()
#
#if (CMAKE_SYSTEM_NAME EQUAL "Linux")
#    target_link_libraries(release glfw3 GL)
#endif ()

add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(release PRIVATE glfw)

add_subdirectory(external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(release PRIVATE glad)

set(IMGUI_INCLUDES
		external/imgui/imgui.h
		external/imgui/backends/imgui_impl_glfw.h
		external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
		external/imgui/imgui.cpp
		external/imgui/backends/imgui_impl_glfw.cpp
		external/imgui/backends/imgui_impl_opengl3.cpp
		external/imgui/imgui_demo.cpp
		external/imgui/imgui_tables.cpp
		external/imgui/imgui_widgets.cpp
		external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
		${IMGUI_INCLUDES}
		${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC external)
target_include_directories(ImGui PUBLIC external/imgui)
target_link_libraries(ImGui PRIVATE glfw)

target_link_libraries(release PRIVATE ImGui)
