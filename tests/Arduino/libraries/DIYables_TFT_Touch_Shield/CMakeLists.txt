# Mock DIYables TFT Touch Shield using SDL3 for host testing

if(HOST_TOOLCHAIN)
    add_library(diyables_tft_touch_shield STATIC
        DIYables_TFT_Touch_Shield.cpp
    )
    
    target_include_directories(diyables_tft_touch_shield PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/tests/Arduino/core
        ${CMAKE_SOURCE_DIR}/tests/third_party/libs/include
    )
    
    target_link_libraries(diyables_tft_touch_shield PUBLIC
        core
        SDL3::SDL3
    )
    
    set_target_properties(diyables_tft_touch_shield PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endif()
