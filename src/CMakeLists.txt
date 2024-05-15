cmake_minimum_required(VERSION 3.27)
project(club)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SRC_FILES *.cpp)
file(GLOB HEADERS headers/*.h)
add_library(${PROJECT_NAME} ${SRC_FILES} ${HEADERS})