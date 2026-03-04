cmake_minimum_required(VERSION 3.22)

project(ssd1315 VERSION 1.0.0 LANGUAGES C)
set(SSD1315_LIBRARY ${PROJECT_NAME})

add_library(${SSD1315_LIBRARY} STATIC
    src/ssd1315.c
)

target_include_directories(${SSD1315_LIBRARY}
    PUBLIC include/
    PRIVATE src/
)


