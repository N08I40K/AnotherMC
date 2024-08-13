project(another-mc-proto)

file(GLOB_RECURSE PROJECT_PROTO ${CMAKE_CURRENT_SOURCE_DIR}/src/*.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROJECT_PROTO})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROJECT_PROTO})

add_library(${PROJECT_NAME} STATIC)
target_link_libraries(${PROJECT_NAME} PUBLIC protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${PROTO_SRCS}
        PUBLIC
        ${PROTO_HDRS}
)