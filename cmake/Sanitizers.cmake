# Sanitizer support, opt-in via -DASTROM_SANITIZE=ON.
#
# Applied only to project targets. Third-party libraries stay uninstrumented:
# instrumenting them would report their internals, not our bugs.

function(astrom_enable_sanitizers target)
    if(NOT ASTROM_SANITIZE)
        return()
    endif()

    if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(WARNING
            "ASTROM_SANITIZE is only supported for GCC and Clang, "
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
