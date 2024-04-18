cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0135 NEW)

project(CrawlerCpp VERSION 1.0)
enable_testing()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(CrawlerApp)
add_subdirectory(CrawlerTest)

include(FetchContent)
FetchContent_Declare(
    googletest 
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)