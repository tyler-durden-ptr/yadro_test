cmake_minimum_required(VERSION 3.27)
project(tests)

set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC
        club
        GTest::gtest GTest::gtest_main
)