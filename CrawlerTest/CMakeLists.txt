add_executable(
    test_crawler_app
    ./CrawlerTest.cpp
)

target_link_libraries(
    test_crawler_app PRIVATE
    lib_crawler
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_crawler_app)