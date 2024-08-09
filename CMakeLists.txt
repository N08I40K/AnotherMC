cmake_minimum_required(VERSION 3.29)

set(VCPKG_PARENT_DIR "${CMAKE_SOURCE_DIR}")
set(VCPKG_TARGET_TRIPLET "x64-windows")
include(cmake/vcpkg.cmake)

project(AMC)

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
