aux_source_directory(. CORE_SRCS)
list(REMOVE_ITEM CORE_SRCS ./uno_server.cc)

# if build uno_core as STATIC library, there exists a problem:
# specializations of Transition are defined in state_machine.cc.o,
# which will be included in libuno_core.a if use static build.
# and when we link uno_server with libuno_core.a, we can find
# definition of Transition in uno_server.cc.o itself,
# although it's just a primary template without any specialization
# so state_machine.cc.o in libuno_core.a won't get linked,
# and the primary template will always get invoked.
# That's why we use OBJECT library here: always link all objects.
add_library(uno_core OBJECT ${CORE_SRCS})
target_link_libraries(uno_core PUBLIC uno_lib spdlog::spdlog)

add_executable(uno_server uno_server.cc)
target_link_libraries(uno_server uno_core)

set_target_properties(uno_server
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
