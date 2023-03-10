cmake_minimum_required(VERSION 3.24)
project(cubes)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-DGRAPHICS_API_OPENGL_33)

# raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
    )
    FetchContent_MakeAvailable(raylib)
endif ()


set(SOURCES main.cpp)
add_executable(${PROJECT_NAME}  ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC raylib)