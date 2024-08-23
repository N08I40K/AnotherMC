cmake_minimum_required(VERSION 3.29)

if (WIN32)
    string(REPLACE "\\" "/" VCPKG_PARENT_DIR "$ENV{USERPROFILE}")
else ()
    message(FATAL_ERROR "TODO: Указать папку в которой будет храниться vcpkg на других ОС")
endif ()

set(PROJECT_NAME "another-mc")
set(PROJECT_VERSION 0.0.0)
include(cmake/vcpkg.cmake)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

vcpkg_add_package(opengl)
vcpkg_add_package(glfw3)
vcpkg_add_package(glew)
vcpkg_add_package(soil)
vcpkg_add_package(glm)
vcpkg_add_package(range-v3)
vcpkg_add_package(boost)
vcpkg_add_package(protobuf)
vcpkg_add_package(stduuid)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE PROJECT_SOURCE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME})

# Compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif ()

# Compiler definitions
string(REPLACE "\\" "/" ASSERT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
target_compile_definitions(${PROJECT_NAME} PRIVATE "ASSERT_DIR=\"${ASSERT_DIR}\"")
target_compile_definitions(${PROJECT_NAME} PRIVATE "UUID_SYSTEM_GENERATOR")

# Sources
target_sources(${PROJECT_NAME} PRIVATE
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCE_HEADERS}
        ${PROJECT_SOURCES}
)
target_include_directories(${PROJECT_NAME} PRIVATE src/)

# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

find_package(soil CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE soil)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

find_package(range-v3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE range-v3::meta range-v3::concepts range-v3::range-v3)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
if (MSVC)
    add_compile_definitions(_WIN32_WINNT=0x0601)
endif ()

find_package(Boost REQUIRED COMPONENTS system thread)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

find_package(protobuf CONFIG REQUIRED)
add_subdirectory("dependencies/proto")
target_link_libraries(${PROJECT_NAME} PRIVATE another-mc-proto)

find_package(stduuid CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE stduuid)