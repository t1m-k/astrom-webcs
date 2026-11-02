# Warning flags for project targets.
#
# INTERFACE target: carries compile options only, is never compiled itself.
# Third-party subdirectories never link against it, so their warnings are
# left untouched.

add_library(astrom_warnings INTERFACE)

target_compile_options(astrom_warnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat=2
    -Wundef
    -Wvla
    $<$<BOOL:${ASTROM_WERROR}>:-Werror>)

# Applied to every project target via astrom_configure_target().
function(astrom_configure_target target)
    set_target_properties(${target} PROPERTIES
        C_STANDARD 23
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF)

    # Strict -std=c23 (__STRICT_ANSI__) makes glibc hide POSIX declarations:
    # unistd.h, unlink, strcasecmp, getopt, socket helpers. Asking for them
    # explicitly is more portable than switching the whole build to gnu23.
    if(NOT WIN32)
        target_compile_definitions(${target} PRIVATE
            _DEFAULT_SOURCE
            _POSIX_C_SOURCE=200809L)
    endif()

    target_link_libraries(${target} PRIVATE astrom_warnings)
    astrom_enable_sanitizers(${target})
    if(ASTROM_COVERAGE)
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    endif()
endfunction()
