function(libd_add_compileflag flag location)
    add_compile_options(${flag})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flag}")
    if(D_DEBUG)
        message(STATUS "${location} - HelperFunctions.cmake (d_add_compileflag): update compile flags: ${CMAKE_CXX_FLAGS_DEBUG}")
    else()
        message(STATUS "${location} - HelperFunctions.cmake: update compile flags (d_add_compileflag): ${CMAKE_CXX_FLAGS_RELEASE}")

    endif()
endfunction()

function(libd_add_versiontarget)
endfunction()

