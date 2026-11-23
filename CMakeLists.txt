cmake_minimum_required(VERSION 3.30)

project(astrom-webcs
    VERSION 0.0.1
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

option(AMWCS_BUILD_TESTS "Build tests" ON)
option(AMWCS_SANITIZE "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(AMWCS_WERROR "Treat warnings as errors" OFF)
option(AMWCS_COVERAGE "Enable coverage instrumentation" OFF)

# TLS provider. Two options, both compiled from the same header:
#   ON  - OpenSSL >= 3 (audited, TLS 1.2 + 1.3). Must be discoverable by CMake;
#         on Windows this normally means a toolchain file (CMakeUserPresets.json)
#         or -DCMAKE_TOOLCHAIN_FILE on the command line.
#   OFF - facil.io's built-in TLS 1.3 (no dependency, TLS 1.3 only).
option(AMWCS_TLS_OPENSSL
    "Use OpenSSL as the TLS provider instead of facil.io's built-in TLS 1.3" ON)

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
# Sanitizers
# ---------------------------------------------------------------------------
# Opt-in via -DAMWCS_SANITIZE=ON.
#
# Applied only to project targets. Third-party libraries stay uninstrumented:
# instrumenting them would report their internals, not our bugs.

function(amwcs_enable_sanitizers target)
    if(NOT AMWCS_SANITIZE)
        return()
    endif()

    if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(WARNING
            "AMWCS_SANITIZE is only supported for GCC and Clang, "
            "ignoring for target ${target}")
        return()
    endif()

    target_compile_options(${target} PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -fno-sanitize-recover=all)
    target_link_options(${target} PRIVATE
        -fsanitize=address,undefined)
endfunction()

# ---------------------------------------------------------------------------
# Warning flags and per-target configuration
# ---------------------------------------------------------------------------
# INTERFACE target: carries compile options only, is never compiled itself.
# Third-party code (the `fio` target) never links against it, so its warnings
# are left untouched - see the facil.io section below.

add_library(amwcs_warnings INTERFACE)

if(MSVC)
    # /W4 is the practical ceiling for MSVC project code: /Wall enables the
    # SDK's own headers and is unusable outside toy projects.
    target_compile_options(amwcs_warnings INTERFACE
        /W4
        /permissive-
        /utf-8
        $<$<BOOL:${AMWCS_WERROR}>:/WX>)
else()
    target_compile_options(amwcs_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wformat=2
        -Wundef
        -Wvla
        $<$<BOOL:${AMWCS_WERROR}>:-Werror>)
endif()

# Applied to every project target.
function(amwcs_configure_target target)
    set_target_properties(${target} PROPERTIES
        C_STANDARD 23
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF)

    if(MSVC)
        # MSVC honours C_STANDARD 23 as /std:c23. Keeping C_EXTENSIONS OFF
        # makes it /std:c23 rather than /std:c2x with Microsoft extensions;
        # MSVC has no separate "gnu" mode, so this is already the strictest
        # setting available.
    else()
        # Strict -std=c23 (__STRICT_ANSI__) makes glibc hide POSIX
        # declarations: unistd.h, unlink, strcasecmp, getopt, socket helpers.
        # Asking for them explicitly is more portable than switching the whole
        # build to gnu23.
        target_compile_definitions(${target} PRIVATE
            _DEFAULT_SOURCE
            _POSIX_C_SOURCE=200809L)
    endif()

    target_link_libraries(${target} PRIVATE amwcs_warnings)
    amwcs_enable_sanitizers(${target})
    if(AMWCS_COVERAGE)
        if(MSVC)
            message(WARNING
                "AMWCS_COVERAGE is not supported with MSVC; ignoring.")
        else()
            target_compile_options(${target} PRIVATE --coverage -O0 -g)
            target_link_options(${target} PRIVATE --coverage)
        endif()
    endif()
endfunction()

# ---------------------------------------------------------------------------
# Third-party dependencies
# ---------------------------------------------------------------------------
# facil.io is vendored as a single amalgamated header, `deps/fio-stl.h`, and
# committed to this repository. No git submodules, no FetchContent: one file
# is cheaper to vendor than to fetch, works offline and is reproducible.
#
# Updating is a deliberate two-step change, all in one commit:
#   1. replace deps/fio-stl.h
#   2. update FIO_STL_COMMIT and FIO_STL_SHA256 below
# The hash check below makes an accidental local edit a hard configure error.

find_package(Threads REQUIRED)

set(FIO_STL_COMMIT "3099b7f93e0f7e7f37c88ba23972658e2ac011df")
set(FIO_STL_SHA256 "038100bb9c6a0cfc8dab8212968164e50a8813fee2c985c2498dffe871889ec7")
set(FIO_STL_HDR "${PROJECT_SOURCE_DIR}/deps/fio-stl.h")

if(NOT EXISTS "${FIO_STL_HDR}")
    message(FATAL_ERROR
        "deps/fio-stl.h is missing. It is vendored and must be committed.")
endif()

file(SHA256 "${FIO_STL_HDR}" fio_stl_actual_sha)
if(NOT fio_stl_actual_sha STREQUAL FIO_STL_SHA256)
    message(FATAL_ERROR
        "deps/fio-stl.h does not match the pinned upstream commit.\n"
        "  expected (commit ${FIO_STL_COMMIT}): ${FIO_STL_SHA256}\n"
        "  actual:                              ${fio_stl_actual_sha}\n"
        "The file is auto-generated upstream and must not be edited by hand. "
        "If the update was intentional, replace the file and update "
        "FIO_STL_COMMIT and FIO_STL_SHA256 in CMakeLists.txt and README.md in the same commit.")
endif()

# The implementation TU. `FIO_EXTERN` + `FIO_EXTERN_COMPLETE` emit every
# definition exactly once; all other translation units include the header with
# `FIO_EXTERN` only and see declarations. This split is the upstream-sanctioned
# scheme and works under MSVC as well (upstream PR #35, in the pinned commit).
add_library(fio STATIC ${PROJECT_SOURCE_DIR}/src/fio_impl.c)

target_include_directories(fio SYSTEM PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/deps>)

# Foreign code: its own standard, its own warning level. Our strict C23,
# -Wpedantic and -Werror must not be applied here - the header is C11 with
# GNU-isms and platform detection that trips sanitizers and pedantic checks.
set_target_properties(fio PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON)

if(MSVC)
    target_compile_options(fio PRIVATE /W0)
else()
    target_compile_options(fio PRIVATE -w)
endif()

if(AMWCS_TLS_OPENSSL)
    find_package(OpenSSL 3 REQUIRED)
    message(STATUS "  TLS provider:  OpenSSL ${OpenSSL_VERSION}")
    target_compile_definitions(fio PRIVATE HAVE_OPENSSL)
    target_link_libraries(fio PUBLIC OpenSSL::SSL OpenSSL::Crypto)
else()
    message(STATUS "  TLS provider:  facil.io built-in TLS 1.3")
endif()

target_link_libraries(fio PUBLIC Threads::Threads)

if(WIN32)
    # winsock + the Windows crypto primitives the header uses for its own
    # RNG / SHA-256 backend when OpenSSL is not selected.
    target_link_libraries(fio PUBLIC ws2_32 crypt32 bcrypt)
endif()

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------

if(EXISTS ${PROJECT_SOURCE_DIR}/src/core/CMakeLists.txt)
    add_subdirectory(src/core)
else()
    message(STATUS "core/CMakeLists.txt not found, skipping core target")
endif()

# The application is the product: built always. Tests are optional.
add_subdirectory(src/app)

if(AMWCS_BUILD_TESTS)
    if(EXISTS ${PROJECT_SOURCE_DIR}/src/tests/CMakeLists.txt)
        enable_testing()
        add_subdirectory(src/tests)
    else()
        message(STATUS "tests/CMakeLists.txt not found, no tests configured")
    endif()
endif()

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------

if(CMAKE_CONFIGURATION_TYPES)
    set(_amwcs_config_summary "${CMAKE_CONFIGURATION_TYPES} (multi-config)")
else()
    set(_amwcs_config_summary "${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "")
message(STATUS "astrom-webcs ${PROJECT_VERSION}")
message(STATUS "  Configuration: ${_amwcs_config_summary}")
message(STATUS "  C compiler:    ${CMAKE_C_COMPILER} (${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION})")
message(STATUS "  Generator:     ${CMAKE_GENERATOR}")
message(STATUS "  Tests:         ${AMWCS_BUILD_TESTS}")
message(STATUS "  Sanitizers:    ${AMWCS_SANITIZE}")
message(STATUS "  Warnings as errors: ${AMWCS_WERROR}")
message(STATUS "")
