cmake_minimum_required(VERSION 3.22)

project(monoled VERSION 1.0.0 LANGUAGES C)
set(LIBRARY ${PROJECT_NAME})

add_library(${LIBRARY} STATIC
    src/monoled-hal.c
)

target_include_directories(${LIBRARY}
    PUBLIC include/
    PRIVATE src/
)


