cmake_minimum_required(VERSION 3.29)

if (WIN32)
    string(REPLACE "\\" "/" VCPKG_PARENT_DIR "$ENV{USERPROFILE}")
else ()
    message(FATAL_ERROR "TODO: Указать папку в которой будет храниться vcpkg на других ОС")
endif ()

project(another-mc VERSION 0.0.0)

include(cmake/vcpkg.cmake)
vcpkg_add_package(glfw3)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE PROJECT_SOURCE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCE_HEADERS}
        ${PROJECT_SOURCES}
)
target_include_directories(${PROJECT_NAME} PRIVATE src/)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif ()
