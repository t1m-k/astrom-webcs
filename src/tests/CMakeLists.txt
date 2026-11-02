if(NOT TARGET fio)
    message(FATAL_ERROR
        "The `fio` target is missing - the root CMakeLists.txt was not used.")
endif()

if(NOT TARGET astromcs_core)
    message(FATAL_ERROR
        "The `astromcs_core` target is missing - src/core was not added.")
endif()

add_executable(astromcs_tests main.c daemon.c)

target_compile_definitions(astromcs_tests PRIVATE
    ASTROMCS_VERSION="${PROJECT_VERSION}")

astromcs_configure_target(astromcs_tests)

target_link_libraries(astromcs_tests PRIVATE fio astromcs_core)

add_test(NAME astromcs_tests COMMAND astromcs_tests)
