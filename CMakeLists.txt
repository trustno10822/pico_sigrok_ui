cmake_minimum_required(VERSION 3.13)
project(pico_sigrok_ui C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

pico_sdk_init()

add_executable(pico_sigrok_ui
    main.c
    src/input/ec11.c
)

target_include_directories(pico_sigrok_ui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(pico_sigrok_ui pico_stdlib hardware_gpio)

pico_add_extra_outputs(pico_sigrok_ui)