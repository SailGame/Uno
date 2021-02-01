aux_source_directory(. TEST_SRCS)

add_executable(uno_test ${TEST_SRCS})
target_link_libraries(uno_test uno_core gtest gmock)

include(GoogleTest)
gtest_discover_tests(uno_test PROPERTIES LABELS UnoTest)

set_target_properties(uno_test
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
