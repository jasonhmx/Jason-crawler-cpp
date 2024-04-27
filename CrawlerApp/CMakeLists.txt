add_library(
    lib_crawler
    src/Crawler.cpp
)

target_include_directories(
    lib_crawler PUBLIC
    ./include
)

target_link_libraries(
    lib_crawler PUBLIC
    lexbor
    cpr
)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
add_executable(
    crawler_app
    src/main.cpp
)

target_link_libraries(
    crawler_app PUBLIC
    lib_crawler
)

target_include_directories(
    crawler_app PUBLIC
    ./include
)