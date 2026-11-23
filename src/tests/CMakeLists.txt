if(NOT TARGET fio)
    message(FATAL_ERROR
        "The `fio` target is missing - the root CMakeLists.txt was not used.")
endif()

if(NOT TARGET amwcs_core)
    message(FATAL_ERROR
        "The `amwcs_core` target is missing - src/core was not added.")
endif()

add_executable(amwcs_tests main.c daemon.c)

amwcs_configure_target(amwcs_tests)

target_link_libraries(amwcs_tests PRIVATE fio amwcs_core)

add_test(NAME amwcs_tests COMMAND amwcs_tests)
