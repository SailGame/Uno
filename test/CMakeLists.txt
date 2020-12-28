aux_source_directory(. TEST_SRCS)
aux_source_directory(../src/common TEST_SRCS)
aux_source_directory(../src/game TEST_SRCS)

add_executable(uno_test ${TEST_SRCS})
target_link_libraries(uno_test PRIVATE uno_proto_lib spdlog::spdlog gtest gmock)

set_target_properties(uno_test
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
