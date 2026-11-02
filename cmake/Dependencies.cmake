# Third-party dependencies from deps/ submodules.
#
# All dependencies are built statically: the result is a single self-contained
# binary without shared libraries to ship alongside.

find_package(Threads REQUIRED)

# libuv calls include(GNUInstallDirs), which on some MinGW setups cannot detect
# the target architecture (CMAKE_SIZEOF_VOID_P stays undefined when CMake's
# compiler ABI probe fails) and emits an author warning. We never install
# anything, so pin the values instead of relying on detection.
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Install libdir" FORCE)
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Install includedir" FORCE)

# wslay declares cmake_minimum_required(VERSION 2.8). CMake 4.0 removed
# compatibility with policies below 3.5 and fails hard on such projects.
# This tells CMake to treat the request as 3.5 instead of erroring out.
if(CMAKE_VERSION VERSION_GREATER_EQUAL 4.0)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

# ---------------------------------------------------------------------------
# libuv
# ---------------------------------------------------------------------------
# LIBUV_BUILD_SHARED must be set before add_subdirectory, otherwise both
# targets appear and the libuv::libuv alias may resolve to the shared one.
# LIBUV_BUILD_TESTS and LIBUV_BUILD_BENCH need no handling: they are declared
# with cmake_dependent_option gated on "CMAKE_SOURCE_DIR STREQUAL
# PROJECT_SOURCE_DIR", which is false when libuv is not the root project.

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(LIBUV_BUILD_SHARED OFF CACHE BOOL "" FORCE)

# libuv does include(CTest), which injects a global BUILD_TESTING option.
# Its own tests are already disabled by the root-project check, but the stray
# cache entry confuses anyone reading CMakeCache.txt. Set it before libuv.
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(deps/libuv EXCLUDE_FROM_ALL)

# ---------------------------------------------------------------------------
# wslay
# ---------------------------------------------------------------------------

set(WSLAY_STATIC ON CACHE BOOL "" FORCE)
set(WSLAY_SHARED OFF CACHE BOOL "" FORCE)
set(WSLAY_TESTS OFF CACHE BOOL "" FORCE)
set(WSLAY_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WSLAY_CONFIGURE_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(deps/wslay EXCLUDE_FROM_ALL)

# deps/wslay/lib/CMakeLists.txt prepends
#   -Wall -Wextra -Werror -pedantic-errors -Wno-long-long
# to the directory-scoped CMAKE_C_FLAGS. Our options come later on the target
# and override them: GCC keeps the last flag of each group.
if(TARGET wslay)
    target_compile_options(wslay PRIVATE -Wno-error -Wno-pedantic)
endif()

# ---------------------------------------------------------------------------
# picohttpparser
# ---------------------------------------------------------------------------
# Upstream ships no CMakeLists.txt, only picohttpparser.c and .h.
# Kept at C99: C23 is our own choice and should not be forced on foreign code.

add_library(picohttpparser STATIC
    ${PROJECT_SOURCE_DIR}/deps/picohttpparser/picohttpparser.c)

# PROJECT_SOURCE_DIR, not CMAKE_CURRENT_SOURCE_DIR: survives a switch from
# include() to add_subdirectory(). BUILD_INTERFACE keeps the target exportable
# if install() is ever added.
target_include_directories(picohttpparser PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/deps/picohttpparser>)

set_target_properties(picohttpparser PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON)
