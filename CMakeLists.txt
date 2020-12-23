# enable x.y version format
cmake_policy(SET CMP0048 NEW)
project(Uno VERSION 0.1)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(Threads REQUIRED)

# assume that gRPC and all its dependencies are already installed
# on this system, so they can be located by find_package().

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Proto file
get_filename_component(proto_path "${CMAKE_CURRENT_SOURCE_DIR}/hello.proto" ABSOLUTE)
get_filename_component(proto_dir "${proto_path}" DIRECTORY)

# Generated sources
set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/hello.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hello.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/hello.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/hello.grpc.pb.h")

add_custom_command(
    OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
    --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
    -I "${proto_dir}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${proto_path}"
    DEPENDS "${proto_path}"# "${_PROTOBUF_PROTOC}" "${_GRPC_CPP_PLUGIN_EXECUTABLE}"
)

add_library(hello_proto_lib ${proto_srcs} ${grpc_srcs})
target_link_libraries(hello_proto_lib ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# a mock server
# add_executable(hello_server hello_server.cc)
# target_link_libraries(hello_server hello_proto_lib)

# uno
add_subdirectory(src)

# lib
add_subdirectory(lib)