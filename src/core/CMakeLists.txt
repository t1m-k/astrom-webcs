add_library(amwcs_core STATIC core.c)

target_compile_definitions(amwcs_core PRIVATE
    AMWCS_VERSION="${PROJECT_VERSION}")

amwcs_configure_target(amwcs_core)

target_include_directories(amwcs_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

#target_link_libraries(amwcs_core PRIVATE fio)
