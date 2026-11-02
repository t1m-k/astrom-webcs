add_library(astromcs_core STATIC core.c)

astromcs_configure_target(astromcs_core)

target_include_directories(astromcs_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

#target_link_libraries(astromcs_core PRIVATE fio)
