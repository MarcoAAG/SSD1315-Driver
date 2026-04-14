cmake_minimum_required(VERSION 3.22)

project(mod VERSION 1.0.0 LANGUAGES C)
set(LIBRARY ${PROJECT_NAME})

add_library(${LIBRARY} STATIC
    src/driver.c
)

target_include_directories(${LIBRARY}
    PUBLIC include/
    PRIVATE src/
)


