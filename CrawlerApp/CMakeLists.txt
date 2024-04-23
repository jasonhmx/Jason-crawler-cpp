add_library(
    lib_crawler
    src/Crawler.cpp
)

add_executable(
    crawler_app
    src/main.cpp
)

target_link_libraries(
    lib_crawler PUBLIC
    lexbor
)

target_link_libraries(
    crawler_app PUBLIC
    lib_crawler
)

target_include_directories(
    crawler_app PUBLIC
    ./include
)