
cmake_minimum_required(VERSION 3.0)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(cpppath-test)
    find_package(cpppath REQUIRED CONFIG)
endif()

find_package(Catch2 REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Catch2::Catch2 cpppath)
