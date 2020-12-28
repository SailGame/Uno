list(APPEND SERVER_SRCS uno_server.cc)
aux_source_directory(common SERVER_SRCS)
aux_source_directory(game SERVER_SRCS)

add_executable(uno_server ${SERVER_SRCS})
target_link_libraries(uno_server PRIVATE uno_proto_lib spdlog::spdlog)

set_target_properties(uno_server
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
