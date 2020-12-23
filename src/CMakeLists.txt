  
list(APPEND CLIENT_SRCS uno_client.cc)
aux_source_directory(common CLIENT_SRCS)
aux_source_directory(game CLIENT_SRCS)
aux_source_directory(game/client_side CLIENT_SRCS)
aux_source_directory(network CLIENT_SRCS)
aux_source_directory(ui CLIENT_SRCS)

add_executable(uno_client ${CLIENT_SRCS})

set_target_properties(uno_client
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(uno_client hello_proto_lib)
