if(NOT TARGET fio)
    message(FATAL_ERROR
        "The `fio` target is missing - the root CMakeLists.txt was not used.")
endif()

if(NOT TARGET amwcs_core)
    message(FATAL_ERROR
        "The `amwcs_core` target is missing - src/core was not added.")
endif()

add_executable(amwcs_app main.c webcs.c daemon.c)

amwcs_configure_target(amwcs_app)

target_link_libraries(amwcs_app PRIVATE fio amwcs_core)
