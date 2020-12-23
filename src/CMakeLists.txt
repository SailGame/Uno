# client  
list(APPEND CLIENT_SRCS uno_client.cc)
aux_source_directory(common CLIENT_SRCS)
aux_source_directory(game CLIENT_SRCS)
aux_source_directory(game/client_side CLIENT_SRCS)
# aux_source_directory(network CLIENT_SRCS)
list(APPEND CLIENT_SRCS network/client.cc)
aux_source_directory(ui CLIENT_SRCS)

add_executable(uno_client ${CLIENT_SRCS})

set_target_properties(uno_client
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(uno_client hello_proto_lib spdlog::spdlog)

# server
list(APPEND SERVER_SRCS uno_server.cc)
aux_source_directory(common SERVER_SRCS)
aux_source_directory(game SERVER_SRCS)
aux_source_directory(game/server_side SERVER_SRCS)
# aux_source_directory(network SERVER_SRCS)
list(APPEND SERVER_SRCS network/server.cc)

add_executable(uno_server ${SERVER_SRCS})

set_target_properties(uno_server
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(uno_server hello_proto_lib spdlog::spdlog)
