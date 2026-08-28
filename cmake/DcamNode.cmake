function(dcam_configure_node target)
    if(NOT ANDROID OR NOT ANDROID_ABI STREQUAL "arm64-v8a")
        message(FATAL_ERROR "Dcam Node SDK supports only Android arm64-v8a targets.")
    endif()

    target_compile_features(${target} PRIVATE c_std_11)
    target_compile_options(${target} PRIVATE
        -O3
        -DNDEBUG
        -flto=thin
        -ffunction-sections
        -fdata-sections
        -fvisibility=hidden
        -Wall
        -Wextra
        -Werror
    )
    target_compile_options(${target} PRIVATE -march=armv8-a+simd)
    target_compile_options(${target} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>
    )
    target_link_options(${target} PRIVATE
        -flto=thin
        -Wl,--gc-sections
        -Wl,--icf=safe
        -Wl,--version-script=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/dcam_node.map
    )
    set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endfunction()
