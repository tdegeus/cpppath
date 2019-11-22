
cmake_minimum_required(VERSION 3.0)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(cpppath-test)
    find_package(cpppath REQUIRED CONFIG)
    set(CPPPATH_INCLUDE_DIR ${cpppath_INCLUDE_DIRS})
endif()

option(WARNINGS "Show build warnings" ON)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WARNINGS)
    if(MSVC)
        if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
            string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-unknown-pragmas")
    endif()
endif()

find_package(Catch2 REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main Catch2::Catch2)
target_include_directories(main PRIVATE ${CPPPATH_INCLUDE_DIR})
