
cmake_minimum_required(VERSION 3.0)

# Basic settings
# ==============

project(cpppath)

# Version
# =======

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/cpppath.h" cpppath_version_defines
     REGEX "#define CPPPATH_VERSION_(MAJOR|MINOR|PATCH)")

foreach(ver ${cpppath_version_defines})
    if(ver MATCHES "#define CPPPATH_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(CPPPATH_VERSION_${CMAKE_MATCH_1}
            "${CMAKE_MATCH_2}"
            CACHE INTERNAL "")
    endif()
endforeach()

set(CPPPATH_VERSION
    ${CPPPATH_VERSION_MAJOR}.${CPPPATH_VERSION_MINOR}.${CPPPATH_VERSION_PATCH})

message(STATUS "Building cpppath v${CPPPATH_VERSION}")

# Set target
# ==========

find_package(xtensor REQUIRED)

add_library(cpppath INTERFACE)

target_include_directories(cpppath INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(cpppath INTERFACE xtensor)

# Installation
# ============

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/cpppath.h" DESTINATION include)

install(TARGETS cpppath EXPORT cpppath-targets)

install(
    EXPORT cpppath-targets
    FILE cpppathTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cpppath")

set(_CPPPATH ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cpppathConfigVersion.cmake"
    VERSION ${CPPPATH_VERSION}
    COMPATIBILITY AnyNewerVersion)

set(CMAKE_SIZEOF_VOID_P ${_CPPPATH})

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cpppathConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/cpppathConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cpppath")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cpppath.pc.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cpppath.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cpppath.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")
