cmake_minimum_required(VERSION 3.30)

project(astrom-webcs
    VERSION 0.1.0
    DESCRIPTION "Web interface for control system of SAO RAS complex Astro-M"
    LANGUAGES C)

# ---------------------------------------------------------------------------
# Sanity checks
# ---------------------------------------------------------------------------

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR
        "In-source build is not allowed. "
        "Use: cmake -S . -B build")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif()

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------

option(ASTROM_BUILD_TESTS "Build tests" ON)
option(ASTROM_SANITIZE "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ASTROM_WERROR "Treat warnings as errors" OFF)
option(ASTROM_COVERAGE "Enable coverage instrumentation" OFF)

# ---------------------------------------------------------------------------
# Output directories
# ---------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(config IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# ---------------------------------------------------------------------------
# Build helpers and third-party dependencies
# ---------------------------------------------------------------------------

include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/Dependencies.cmake)

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------

if(EXISTS ${PROJECT_SOURCE_DIR}/src/CMakeLists.txt)
    add_subdirectory(src)
else()
    message(STATUS "src/CMakeLists.txt not found, skipping application target")
endif()

if(ASTROM_BUILD_TESTS)
    if(EXISTS ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)
        enable_testing()
        add_subdirectory(tests)
    else()
        message(STATUS "tests/CMakeLists.txt not found, no tests configured")
    endif()
endif()

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------

message(STATUS "")
message(STATUS "astrom-webcs ${PROJECT_VERSION}")
message(STATUS "  Build type:    ${CMAKE_BUILD_TYPE}")
message(STATUS "  C compiler:    ${CMAKE_C_COMPILER} (${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION})")
message(STATUS "  Generator:     ${CMAKE_GENERATOR}")
message(STATUS "  Tests:         ${ASTROM_BUILD_TESTS}")
message(STATUS "  Sanitizers:    ${ASTROM_SANITIZE}")
message(STATUS "  Warnings as errors: ${ASTROM_WERROR}")
message(STATUS "")
