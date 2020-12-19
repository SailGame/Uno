  
aux_source_directory(. APP_DIR)

add_executable(uno ${APP_DIR} ${proto_srcs} ${grpc_srcs})

set_target_properties(uno
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(uno ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
